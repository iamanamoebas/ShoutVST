#pragma once

#include <shout/shout.h>
#include <mutex>
#include <string>

using std::string;
using std::recursive_mutex;
using std::lock_guard;

class LibShoutWrapper {
 private:
  typedef std::lock_guard<std::recursive_mutex> guard;
  recursive_mutex mtx;
  shout_t *pShout = nullptr;

 public:
  ~LibShoutWrapper();
  bool LibShoutWrapper::InitializeICECasting(
      const string &hostname, const string &protocol, unsigned short port,
      const string &streamname, const string &streamurl,
      const string &streamgenre, const string &streamdescription,
      const string &bitrate, const string &targetsamplerate,
      const string &artist, const string &title, const string &username,
      const string &password, const string &mountpoint, const string &format);

  void LibShoutWrapper::StopICECasting();

  bool LibShoutWrapper::SendDataToICE(unsigned char *pData, size_t nLen);

  void LibShoutWrapper::UpdateMetadata(const char *sz);
};