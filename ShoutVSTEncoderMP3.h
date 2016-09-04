#pragma once
#include "BladeMP3EncDLL.h"
#include "ShoutVSTEncoder.h"

class ShoutVSTEncoderMP3 : public ShoutVSTEncoder {
 public:
  explicit ShoutVSTEncoderMP3(ShoutVST* p);
  bool Preload();
  bool Initialize();
  bool Close();
  bool Process(float** inputs, VstInt32 sampleFrames);

 private:
  HBE_STREAM hbeStream;
  DWORD dwSamples;
  DWORD dwMP3Buffer;
  DWORD dwSamplesSoFar;
  BYTE* pMP3Buffer;
  SHORT* pWAVBuffer;

  bool bInitialized;
};
