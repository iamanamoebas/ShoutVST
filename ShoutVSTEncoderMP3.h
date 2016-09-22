#pragma once
#include "BladeMP3EncDLL.h"
#include "ShoutVSTEncoder.h"

class ShoutVSTEncoderMP3 : public ShoutVSTEncoder {
 public:
  ShoutVSTEncoderMP3(LibShoutWrapper& ls);
  ~ShoutVSTEncoderMP3();
  bool Initialize(const int bitrate, const int samplerate,
                  const int target_samplerate);
  bool Close();
  bool Process(float** inputs, VstInt32 sampleFrames);

 private:
  HBE_STREAM hbeStream;
  DWORD dwSamples = 0;
  DWORD dwMP3Buffer = 0;
  BYTE* pMP3Buffer = nullptr;
  SHORT* pWAVBuffer = nullptr;
};
