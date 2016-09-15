#include "LibShoutWrapper.h"

LibShoutWrapper::~LibShoutWrapper() { StopICECasting(); }

bool LibShoutWrapper::InitializeICECasting(
    const string &hostname, const string &protocol, unsigned short port,
    const string &streamname, const string &streamurl,
    const string &streamgenre, const string &streamdescription,
    const string &bitrate, const string &targetsamplerate, const string &artist,
    const string &title, const string &username, const string &password,
    const string &mountpoint, const string &format) {
  guard lock(mtx);
  if (pShout) {
    return false;
  }
  shout_init();
  shout_t *shout = nullptr;
  if (!(shout = shout_new())) {
    return false;
  }

  if (shout_set_host(shout, hostname.c_str()) != SHOUTERR_SUCCESS) {
    return false;
  }

  int shout_proto = SHOUT_PROTOCOL_HTTP;
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

  if (shout_set_port(shout, port) != SHOUTERR_SUCCESS) {
    return false;
  }

  shout_set_agent(shout, "ShoutVST");

  if (!streamname.empty()) shout_set_name(shout, streamname.c_str());

  if (!streamurl.empty()) shout_set_url(shout, streamurl.c_str());

  if (!streamgenre.empty()) shout_set_genre(shout, streamgenre.c_str());

  if (!streamdescription.empty())
    shout_set_description(shout, streamdescription.c_str());

  shout_set_audio_info(shout, SHOUT_AI_BITRATE, bitrate.c_str());

  shout_set_audio_info(shout, SHOUT_AI_SAMPLERATE, targetsamplerate.c_str());

  if (!artist.empty()) shout_set_audio_info(shout, "artist", artist.c_str());

  if (!title.empty()) shout_set_audio_info(shout, "title", title.c_str());

  if (shout_set_user(shout, username.c_str()) != SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_set_password(shout, password.c_str()) != SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_set_mount(shout, mountpoint.c_str()) != SHOUTERR_SUCCESS) {
    return false;
  }

  shout_set_public(shout, 1);

  unsigned int shout_format = 0;

  if (format == "mp3") {
    shout_format = SHOUT_FORMAT_MP3;
  }

  if (format == "ogg") {
    shout_format = SHOUT_FORMAT_OGG;
  }

  if (shout_set_format(shout, shout_format) != SHOUTERR_SUCCESS) {
    return false;
  }

  if (shout_open(shout) != SHOUTERR_SUCCESS) {
    return false;
  }
  pShout = shout;

  return true;
}

void LibShoutWrapper::StopICECasting() {
  shout_shutdown();
  guard lock(mtx);
  if (pShout) {
    shout_close(pShout);
    shout_free(pShout);
    pShout = nullptr;
  }
}

bool LibShoutWrapper::SendDataToICE(unsigned char *pData, size_t nLen) {
  if (!pShout) {
    return false;
  }
  guard lock(mtx);
  if (!pShout) {
    return false;
  }
  if (!pData || nLen <= 0) {
    return true;
  }
  shout_sync(pShout);
  if (shout_send(pShout, pData, nLen) < 0) {
    return false;
  }
  return true;
}

void LibShoutWrapper::UpdateMetadata(const char *sz) {
  if (!pShout) return;
  guard lock(mtx);
  if (!pShout) return;
  shout_metadata_t *meta = shout_metadata_new();
  if (!meta) return;

  shout_metadata_add(meta, "song", sz);

  shout_set_metadata(pShout, meta);

  shout_metadata_free(meta);
}
