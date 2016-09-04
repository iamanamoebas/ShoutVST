#pragma once

#include <aeffect.h>
class ShoutVST;

#include <mutex>
using std::recursive_mutex;

class ShoutVSTEncoder {
 public:
  explicit ShoutVSTEncoder(ShoutVST*);
  virtual bool Initialize() = 0;
  virtual bool Close() = 0;
  virtual bool Process(float** inputs, VstInt32 sampleFrames) = 0;
  virtual ~ShoutVSTEncoder();

 protected:
  typedef std::lock_guard<std::recursive_mutex> guard;
  recursive_mutex mtx_;
  ShoutVST* pVST;
};
