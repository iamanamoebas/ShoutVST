#include "ShoutVST.h"
#include <algorithm>
#include <string>
#include "ShoutVSTEncoderMP3.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// note to self
#define STEREO 2

ShoutVSTEncoderMP3::ShoutVSTEncoderMP3(ShoutVST* p) : ShoutVSTEncoder(p) {
  pMP3Buffer = NULL;
  pWAVBuffer = NULL;
  bInitialized = false;
}

ShoutVSTEncoderMP3::~ShoutVSTEncoderMP3() {
	Close();
}

bool ShoutVSTEncoderMP3::Initialize() {
  guard lock(mtx_);
  if (bInitialized) {
	  return false;
  }
  bInitialized = false;

  hbeStream = NULL;

  //BE_VERSION v;
  //beVersion(&v);

  BE_CONFIG cfg;
  memset(&cfg, 0, sizeof(BE_CONFIG));

  cfg.dwConfig = BE_CONFIG_LAME;

  cfg.format.LHV1.dwStructVersion = 1;
  cfg.format.LHV1.dwStructSize = sizeof(BE_CONFIG);
  cfg.format.LHV1.dwSampleRate = pVST->updateSampleRate();
  cfg.format.LHV1.nMode = BE_MP3_MODE_JSTEREO;
  cfg.format.LHV1.dwBitrate = pVST->GetBitrate();
  cfg.format.LHV1.dwReSampleRate = pVST->GetTargetSampleRate();
  cfg.format.LHV1.dwMpegVersion = MPEG1;
  cfg.format.LHV1.nPreset = LQP_CBR;

  if (beInitStream(&cfg, &dwSamples, &dwMP3Buffer, &hbeStream) !=
      BE_ERR_SUCCESSFUL) {
    return false;
  }
  pMP3Buffer = new BYTE[dwMP3Buffer];
  pWAVBuffer = new SHORT[dwSamples];
  dwSamplesSoFar = 0;
  dwSamples /= STEREO;

  bInitialized = true;

  return true;
}

bool ShoutVSTEncoderMP3::Close() {
  guard lock(mtx_);
  if (!bInitialized) {
	  return true;
  }
  bInitialized = false;
  DWORD dwWrite = 0;
  beDeinitStream(hbeStream, pMP3Buffer, &dwWrite);
  beCloseStream(hbeStream);
  pVST->SendDataToICE(pMP3Buffer, dwWrite);
  delete[] pWAVBuffer;
  delete[] pMP3Buffer;
  return true;
}

bool ShoutVSTEncoderMP3::Process(float** inputs, VstInt32 sampleFrames) {
  guard lock(mtx_);
  if (!bInitialized) return false;

  int nSamplesToProcess = sampleFrames;
  int nInputPointer = 0;
  while (nSamplesToProcess > 0) {
    int nSampleRoom = dwSamples - dwSamplesSoFar;
    int nSamplesProcessed = std::min(nSampleRoom, nSamplesToProcess);

    SHORT* p = pWAVBuffer + dwSamplesSoFar * STEREO;
    for (int i = 0; i < nSamplesProcessed; i++) {
      *(p++) = (SHORT)(
          std::min(1.0f, std::max(-1.0f, inputs[0][nInputPointer])) * 32767.0f);
      *(p++) = (SHORT)(
          std::min(1.0f, std::max(-1.0f, inputs[1][nInputPointer])) * 32767.0f);
      nInputPointer++;
    }
    dwSamplesSoFar += nSamplesProcessed;

    if (dwSamplesSoFar >= dwSamples) {
      DWORD dwWrite = 0;
      if (beEncodeChunk(hbeStream, dwSamples * STEREO, pWAVBuffer, pMP3Buffer,
                        &dwWrite) != BE_ERR_SUCCESSFUL) {
        beCloseStream(hbeStream);
        return false;
      }

      if (!pVST->SendDataToICE(pMP3Buffer, dwWrite)) return false;
      dwSamplesSoFar = 0;
    }

    nSamplesToProcess -= nSamplesProcessed;
  }

  return true;
}
