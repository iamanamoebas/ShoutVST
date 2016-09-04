#pragma once
#include <audioeffectx.h>
#include <shout/shout.h>
#include "ShoutVSTEditor.h"
#include "ShoutVSTEncoderMP3.h"
#include "ShoutVSTEncoderOGG.h"

#include <mutex>

using std::recursive_mutex;
using std::lock_guard;

class ShoutVST : public AudioEffectX {
 public:
  explicit ShoutVST(audioMasterCallback audioMaster);
  ~ShoutVST();
  virtual void processReplacing(float** inputs, float** outputs,
                                VstInt32 sampleFrames) override;
  virtual void setParameter(VstInt32 index, float value) override;
  virtual float getParameter(VstInt32 index) override;
  virtual void getParameterDisplay(VstInt32 index, char* text) override;
  virtual void getParameterName(VstInt32 index, char* text) override;
  virtual bool getEffectName(char* name) override;
  virtual bool getVendorString(char* text) override;
  virtual bool getProductString(char* text) override;
  virtual VstPlugCategory getPlugCategory() override;
  virtual VstInt32 getVendorVersion() override;
  bool IsConnected();
  bool SendDataToICE(unsigned char*, VstInt32);
  bool CanDoMP3();
  int GetBitrate();
  int GetTargetSampleRate();
  void UpdateMetadata(const char* sz);

 private:
  recursive_mutex shout_mtx;
  bool InitializeICECasting();
  void StopICECasting();
  bool bCanDoMP3 = false;
  bool bStreamConnected = false;
  ShoutVSTEncoder* encSelected = nullptr;
  ShoutVSTEncoderOGG* encOGG = nullptr;
  ShoutVSTEncoderMP3* encMP3 = nullptr;
  ShoutVSTEditor* pEditor = nullptr;
  shout_t* pShout = nullptr;
};
