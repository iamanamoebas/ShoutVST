#include "ShoutVST.h"

typedef std::lock_guard<std::recursive_mutex> guard;

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
  return new ShoutVST(audioMaster);
}

VstInt32 ShoutVST::getVendorVersion() { return 2100; }

VstPlugCategory ShoutVST::getPlugCategory() { return kPlugCategEffect; }

ShoutVST::ShoutVST(audioMasterCallback audioMaster)
    : AudioEffectX(audioMaster, 1, 1)

{
  setNumInputs(2);
  setNumOutputs(2);
  setUniqueID(CCONST('b', 'q', '9', 'e'));
  canProcessReplacing();
  canDoubleReplacing(false);
  encMP3 = new ShoutVSTEncoderMP3(this);
  encOGG = new ShoutVSTEncoderOGG(this);
  bCanDoMP3 = encMP3->Preload();
  encSelected = encMP3;
  pEditor = new ShoutVSTEditor(this);
  setEditor(pEditor);
}

ShoutVST::~ShoutVST() {
  StopICECasting();
  delete encMP3;
  delete encOGG;
  setEditor(nullptr);
  delete pEditor;
}

bool ShoutVST::InitializeICECasting() {
  guard lock(shout_mtx);
  if (pShout) {
    return false;
  }
  shout_init();
  shout_t* shout = nullptr;
  if (!(shout = shout_new())) {
    return false;
  }

  if (shout_set_host(shout, pEditor->getHostName().c_str()) !=
      SHOUTERR_SUCCESS) {
    return false;
  }

  int shout_proto = SHOUT_PROTOCOL_HTTP;
  const string protocol = pEditor->getProtocol();
  if (protocol == "http") {
    shout_proto = SHOUT_PROTOCOL_HTTP;
  } else if (protocol == "xaudiocast") {
    shout_proto = SHOUT_PROTOCOL_XAUDIOCAST;
  } else if (protocol == "icy") {
    shout_proto = SHOUT_PROTOCOL_ICY;
  } else if (protocol == "roaraudio") {
    shout_proto = SHOUT_PROTOCOL_ROARAUDIO;
  }

  if (shout_set_protocol(shout, shout_proto) != SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_set_port(shout, pEditor->getPort()) != SHOUTERR_SUCCESS) {
    return false;
  }

  shout_set_agent(shout, "ShoutVST");

  if (!pEditor->getStreamName().empty())
    shout_set_name(shout, pEditor->getStreamName().c_str());

  if (!pEditor->getStreamURL().empty())
    shout_set_url(shout, pEditor->getStreamURL().c_str());

  if (!pEditor->getStreamGenre().empty())
    shout_set_genre(shout, pEditor->getStreamGenre().c_str());

  if (!pEditor->getStreamDescription().empty())
    shout_set_description(shout, pEditor->getStreamDescription().c_str());

  shout_set_audio_info(shout, SHOUT_AI_BITRATE, pEditor->GetBitrate().c_str());

  shout_set_audio_info(shout, SHOUT_AI_SAMPLERATE,
                       pEditor->GetTargetSampleRate().c_str());

  if (!pEditor->getStreamArtist().empty())
    shout_set_audio_info(shout, "artist", pEditor->getStreamArtist().c_str());

  if (!pEditor->getStreamTitle().empty())
    shout_set_audio_info(shout, "title", pEditor->getStreamTitle().c_str());

  if (shout_set_user(shout, pEditor->getUserName().c_str()) !=
      SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_set_password(shout, pEditor->getPassword().c_str()) !=
      SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_set_mount(shout, pEditor->getMountPoint().c_str()) !=
      SHOUTERR_SUCCESS) {
    return false;
  }

  shout_set_public(shout, 1);

  unsigned int format = 0;

  if (pEditor->getEncodingFormat() == "mp3") {
    format = SHOUT_FORMAT_MP3;
  }

  if (pEditor->getEncodingFormat() == "ogg") {
    format = SHOUT_FORMAT_OGG;
  }

  if (shout_set_format(shout, format) != SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_open(shout) != SHOUTERR_SUCCESS) {
    return false;
  }
  pShout = shout;

  return true;
}

void ShoutVST::StopICECasting() {
  guard lock(shout_mtx);
  if (pShout) shout_close(pShout);
  if (pShout) shout_free(pShout);
  pShout = nullptr;
  shout_shutdown();
}

bool ShoutVST::IsConnected() { return bStreamConnected; }

bool ShoutVST::SendDataToICE(unsigned char* pData, VstInt32 nLen) {
  guard lock(shout_mtx);
  if (!pData || nLen <= 0) {
    return true;
  }
  if (!pShout) {
    return false;
  }
  shout_sync(pShout);
  if (shout_send(pShout, pData, nLen) < 0) {
    return false;
  }
  return true;
}

bool ShoutVST::CanDoMP3() { return bCanDoMP3; }

int ShoutVST::GetBitrate() { return std::stoi(pEditor->GetBitrate()); }

int ShoutVST::GetTargetSampleRate() {
  return std::stoi(pEditor->GetTargetSampleRate());
}

void ShoutVST::processReplacing(float** inputs, float** outputs,
                                VstInt32 sampleFrames) {
  if (!inputs || !outputs || sampleFrames <= 0) {
    return;
  }

  if (IsConnected()) {
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
    if (!InitializeICECasting()) {
      StopICECasting();
      return;
    }
    if (!encSelected->Initialize()) {
      StopICECasting();
      return;
    }

  } else {
    encSelected->Close();
    StopICECasting();
  }

  bStreamConnected = bShouldConnect;

  pEditor->DisableAccordingly();
}

float ShoutVST::getParameter(VstInt32 index) {
  return bStreamConnected ? 1.0f : 0.0f;
}

void ShoutVST::getParameterDisplay(VstInt32 index, char* text) {
  if (!text) {
    return;
  }
  if (bStreamConnected) {
    vst_strncpy(text, "Yes", kVstMaxParamStrLen);
  } else {
    vst_strncpy(text, "No", kVstMaxProductStrLen);
  }
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

void ShoutVST::UpdateMetadata(const char* sz) {
  if (!pShout) return;
  guard lock(shout_mtx);
  shout_metadata_t* meta = shout_metadata_new();
  if (!meta) return;

  if (shout_metadata_add(meta, "song", sz) != SHOUTERR_SUCCESS) {
    // Log("Error adding metadata: %s\r\n", shout_get_error(pShout));
  }

  if (shout_set_metadata(pShout, meta) != SHOUTERR_SUCCESS) {
    // Log("Error setting metadata: %s\r\n", shout_get_error(pShout));
  }

  shout_metadata_free(meta);

  // Log("Set metadata to: %s\r\n", sz);
}