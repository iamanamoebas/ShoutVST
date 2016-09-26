#include "ShoutVSTEncoderMP3.h"
#include <algorithm>
#include <string>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

ShoutVSTEncoderMP3::ShoutVSTEncoderMP3(LibShoutWrapper& ls)
    : ShoutVSTEncoder(ls) {}

ShoutVSTEncoderMP3::~ShoutVSTEncoderMP3() { Close(); }

bool ShoutVSTEncoderMP3::Initialize(const int bitrate, const int samplerate,
                                    const int target_samplerate) {
  guard lock(mtx_);
  if (bInitialized) {
    return false;
  }
  bInitialized = false;
  k=0;
  hbeStream = NULL;

  // BE_VERSION v;
  // beVersion(&v);

  BE_CONFIG cfg;
  memset(&cfg, 0, sizeof(BE_CONFIG));

  cfg.dwConfig = BE_CONFIG_LAME;

  cfg.format.LHV1.dwStructVersion = 1;
  cfg.format.LHV1.dwStructSize = sizeof(BE_CONFIG);
  cfg.format.LHV1.dwSampleRate = samplerate;
  cfg.format.LHV1.nMode = BE_MP3_MODE_JSTEREO;
  cfg.format.LHV1.dwBitrate = bitrate;
  cfg.format.LHV1.dwReSampleRate = target_samplerate;
  cfg.format.LHV1.dwMpegVersion = MPEG1;
  cfg.format.LHV1.nPreset = LQP_CBR;

  if (beInitStream(&cfg, &dwSamples, &dwMP3Buffer, &hbeStream) !=
      BE_ERR_SUCCESSFUL) {
    return false;
  }
  pMP3Buffer = new BYTE[dwMP3Buffer];
  pWAVBuffer = new SHORT[dwSamples];

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
  libshout.SendDataToICE(pMP3Buffer, dwWrite);
  delete[] pWAVBuffer;
  delete[] pMP3Buffer;
  return true;
}

bool ShoutVSTEncoderMP3::Process(float** inputs, VstInt32 sampleFrames) {
  guard lock(mtx_);
  if (!bInitialized) return false;
  for (VstInt32 i(0); i < sampleFrames; i++) {
    pWAVBuffer[k++] = (SHORT)(std::min(1.0f, std::max(-1.0f, inputs[0][i])) * 32767.0f);
    pWAVBuffer[k++] = (SHORT)(std::min(1.0f, std::max(-1.0f, inputs[1][i])) * 32767.0f);
    if (k >= dwSamples) {
      k = 0;
      DWORD dwWrite = 0;
      if (beEncodeChunk(hbeStream, dwSamples, pWAVBuffer, pMP3Buffer, &dwWrite) != BE_ERR_SUCCESSFUL) {
        return false;
      }
      if (!libshout.SendDataToICE(pMP3Buffer, dwWrite)) {
        return false;
      }
    }
  }
  return true;
}
