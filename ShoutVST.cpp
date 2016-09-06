#include "ShoutVST.h"

typedef std::lock_guard<std::recursive_mutex> guard;

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
  return new ShoutVST(audioMaster);
}

VstInt32 ShoutVST::getVendorVersion() { return 2100; }

VstPlugCategory ShoutVST::getPlugCategory() { return kPlugCategEffect; }

ShoutVST::ShoutVST(audioMasterCallback audioMaster)
    : AudioEffectX(audioMaster, 1, 1) {
  setNumInputs(2);
  setNumOutputs(2);
  setUniqueID(CCONST('b', 'q', '9', 'e'));
  canProcessReplacing();
  canDoubleReplacing(false);
  encMP3 = new ShoutVSTEncoderMP3(libShoutWrapper);
  encOGG = new ShoutVSTEncoderOGG(libShoutWrapper);
  encSelected = encMP3;
  pEditor = new ShoutVSTEditor(this);
  setEditor(pEditor);
}

ShoutVST::~ShoutVST() {
  setParameter(0, 0.0);
  pEditor->close();
  setEditor(nullptr);
  delete encMP3;
  delete encOGG;
  delete pEditor;
}

bool ShoutVST::IsConnected() {
  guard lock(mtx);
  return bStreamConnected;
}

int ShoutVST::GetBitrate() { return std::stoi(pEditor->GetBitrate()); }

int ShoutVST::GetTargetSampleRate() {
  return std::stoi(pEditor->GetTargetSampleRate());
}

void ShoutVST::processReplacing(float** inputs, float** outputs,
                                VstInt32 sampleFrames) {
  if (!inputs || !outputs || sampleFrames <= 0) {
    return;
  }
  guard lock(mtx);
  if (bStreamConnected) {
    if (!encSelected->Process(inputs, sampleFrames)) {
      setParameter(0, 0.0);
    }
  }
  float* in1 = inputs[0];
  float* in2 = inputs[1];
  float* out1 = outputs[0];
  float* out2 = outputs[1];
  for (VstInt32 i(0); i < sampleFrames; ++i) {
    out1[i] = in1[i];
    out2[i] = in1[i];
  }
}

void ShoutVST::setParameter(VstInt32 index, float value) {
  guard lock(mtx);
  bool bShouldConnect = (value >= 0.5);

  if (bShouldConnect == bStreamConnected) {
    return;
  }

  if (pEditor->getEncodingFormat() == "mp3") {
    encSelected = encMP3;
  }

  if (pEditor->getEncodingFormat() == "ogg") {
    encSelected = encOGG;
  }

  if (bShouldConnect) {
    if (!libShoutWrapper.InitializeICECasting(
            pEditor->getHostName(), pEditor->getProtocol(), pEditor->getPort(),
            pEditor->getStreamName(), pEditor->getStreamURL(),
            pEditor->getStreamGenre(), pEditor->getStreamDescription(),
            pEditor->GetBitrate(), pEditor->GetTargetSampleRate(),
            pEditor->getStreamArtist(), pEditor->getStreamTitle(),
            pEditor->getUserName(), pEditor->getPassword(),
            pEditor->getMountPoint(), pEditor->getEncodingFormat())) {
      libShoutWrapper.StopICECasting();
      return;
    }
    if (!encSelected->Initialize(GetBitrate(), (const int)updateSampleRate(),
                                 GetTargetSampleRate())) {
      libShoutWrapper.StopICECasting();
      return;
    }

  } else {
    bStreamConnected = false;
    encSelected->Close();
    libShoutWrapper.StopICECasting();
  }

  bStreamConnected = bShouldConnect;

  pEditor->DisableAccordingly();
}

float ShoutVST::getParameter(VstInt32 index) {
  return IsConnected() ? 1.0f : 0.0f;
}

void ShoutVST::getParameterDisplay(VstInt32 index, char* text) {
  if (!text) {
    return;
  }
  if (IsConnected()) {
    vst_strncpy(text, "Yes", kVstMaxParamStrLen);
  } else {
    vst_strncpy(text, "No", kVstMaxProductStrLen);
  }
}

void ShoutVST::UpdateMetadata(const string& metadata) {
  libShoutWrapper.UpdateMetadata(metadata.c_str());
}

void ShoutVST::getParameterName(VstInt32 index, char* text) {
  if (!text) {
    return;
  }
  vst_strncpy(text, "Streaming", kVstMaxParamStrLen);
}

bool ShoutVST::getEffectName(char* name) {
  if (!name) {
    return false;
  }
  vst_strncpy(name, "ShoutVST", kVstMaxEffectNameLen);
  return true;
}

bool ShoutVST::getVendorString(char* text) {
  if (!text) {
    return false;
  }
  vst_strncpy(text, "https://github.com/R-Tur/ShoutVST", kVstMaxVendorStrLen);
  return true;
}

bool ShoutVST::getProductString(char* text) {
  if (!text) {
    return false;
  }
  vst_strncpy(text, "ShoutVST", kVstMaxProductStrLen);
  return true;
}
