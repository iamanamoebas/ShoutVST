// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "ShoutVSTEditorFL.h"

void ShoutVSTEditorFL::cb_OK_i(Fl_Button*, void*) {
  closeAbout();
}
void ShoutVSTEditorFL::cb_OK(Fl_Button* o, void* v) {
  ((ShoutVSTEditorFL*)(o->parent()->user_data()))->cb_OK_i(o,v);
}

void ShoutVSTEditorFL::cb_Source_i(Fl_Button*, void*) {
  fl_open_uri("https://github.com/R-Tur/ShoutVST");
}
void ShoutVSTEditorFL::cb_Source(Fl_Button* o, void* v) {
  ((ShoutVSTEditorFL*)(o->parent()->user_data()))->cb_Source_i(o,v);
}

void ShoutVSTEditorFL::cb_button_connect_i(Fl_Button*, void*) {
  connectCallback();
}
void ShoutVSTEditorFL::cb_button_connect(Fl_Button* o, void* v) {
  ((ShoutVSTEditorFL*)(o->parent()->parent()->user_data()))->cb_button_connect_i(o,v);
}

void ShoutVSTEditorFL::cb_button_disconnect_i(Fl_Button*, void*) {
  disconnectCallback();
}
void ShoutVSTEditorFL::cb_button_disconnect(Fl_Button* o, void* v) {
  ((ShoutVSTEditorFL*)(o->parent()->parent()->user_data()))->cb_button_disconnect_i(o,v);
}

void ShoutVSTEditorFL::cb__i(Fl_Button*, void*) {
  showAbout();
}
void ShoutVSTEditorFL::cb_(Fl_Button* o, void* v) {
  ((ShoutVSTEditorFL*)(o->parent()->user_data()))->cb__i(o,v);
}

void ShoutVSTEditorFL::cb_Send_i(Fl_Button*, void*) {
  metadataCallback();
}
void ShoutVSTEditorFL::cb_Send(Fl_Button* o, void* v) {
  ((ShoutVSTEditorFL*)(o->parent()->parent()->user_data()))->cb_Send_i(o,v);
}

ShoutVSTEditorFL::ShoutVSTEditorFL() {
  buffer_about = new Fl_Text_Buffer();
  string aboutText = "";
  aboutText = "Version: "+string(SHOUTVST_FULL_VERSION_STR)+"\n\n";
  aboutText += ("Built on " + string(__DATE__) + " (" + string(__TIME__) + ")\n");
  aboutText += ("CMake version: " + string(CMAKE_VERSION) + "\n");
  aboutText += ("Compiler: " + string(CMAKE_GENERATOR) + "\n");
  aboutText += "\n";
  aboutText += "Original idea:\nhttps://github.com/Gargaj/ShoutVST\n";
  buffer_about->text(aboutText.c_str());
  { about_window = new Fl_Double_Window(403, 288, "About ShoutVST");
    about_window->user_data((void*)(this));
    about_window->align(Fl_Align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE));
    about_window->hotspot(about_window);
    { Fl_Button* o = new Fl_Button(215, 225, 160, 45, "OK");
      o->callback((Fl_Callback*)cb_OK);
    } // Fl_Button* o
    { Fl_Text_Display* o = new Fl_Text_Display(1, 40, 385, 155, "ShoutVST");
      o->box(FL_NO_BOX);
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelsize(34);
      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      o->buffer(buffer_about);
    } // Fl_Text_Display* o
    { Fl_Button* o = new Fl_Button(20, 225, 160, 45, "Source Code (GitHub)");
      o->callback((Fl_Callback*)cb_Source);
      o->labelcolor(FL_DARK_BLUE);
    } // Fl_Button* o
    about_window->size_range(403, 288, 403, 288);
    about_window->end();
  } // Fl_Double_Window* about_window
  { fl_window = new Fl_Double_Window(677, 480);
    fl_window->color(FL_BACKGROUND2_COLOR);
    fl_window->user_data((void*)(this));
    fl_window->align(Fl_Align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE));
    fl_window->hotspot(fl_window);
    { group_connection = new Fl_Group(10, 30, 280, 220, "Connection:");
      group_connection->box(FL_GTK_DOWN_BOX);
      group_connection->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      { connection_hostname = new Fl_Input(95, 45, 175, 25, "Hostname:");
        connection_hostname->box(FL_GTK_DOWN_BOX);
        connection_hostname->when(FL_WHEN_CHANGED);
        connection_hostname->value("localhost");
      } // Fl_Input* connection_hostname
      { connection_port = new Fl_Input(95, 75, 175, 25, "Port:");
        connection_port->box(FL_GTK_DOWN_BOX);
        connection_port->value("8000");
      } // Fl_Input* connection_port
      { connection_username = new Fl_Input(95, 105, 175, 25, "UserName:");
        connection_username->box(FL_GTK_DOWN_BOX);
        connection_username->value("source");
      } // Fl_Input* connection_username
      { connection_password = new Fl_Input(95, 135, 175, 25, "Password:");
        connection_password->box(FL_GTK_DOWN_BOX);
        connection_password->value("hackme");
      } // Fl_Input* connection_password
      { connection_mountpoint = new Fl_Input(95, 165, 175, 25, "Mountpoint:");
        connection_mountpoint->box(FL_GTK_DOWN_BOX);
        connection_mountpoint->value("/live");
      } // Fl_Input* connection_mountpoint
      { Fl_Group* o = new Fl_Group(20, 195, 185, 55);
        { proto_http = new Fl_Round_Button(20, 195, 50, 25, "http");
          proto_http->type(102);
          proto_http->down_box(FL_ROUND_DOWN_BOX);
          proto_http->value(1);
        } // Fl_Round_Button* proto_http
        { proto_xaudiocast = new Fl_Round_Button(70, 195, 95, 25, "xaudiocast");
          proto_xaudiocast->type(102);
          proto_xaudiocast->down_box(FL_ROUND_DOWN_BOX);
        } // Fl_Round_Button* proto_xaudiocast
        { proto_icy = new Fl_Round_Button(20, 215, 45, 25, "icy");
          proto_icy->type(102);
          proto_icy->down_box(FL_ROUND_DOWN_BOX);
        } // Fl_Round_Button* proto_icy
        { proto_roaraudio = new Fl_Round_Button(70, 215, 90, 25, "roaraudio");
          proto_roaraudio->type(102);
          proto_roaraudio->down_box(FL_ROUND_DOWN_BOX);
        } // Fl_Round_Button* proto_roaraudio
        o->end();
      } // Fl_Group* o
      group_connection->end();
    } // Fl_Group* group_connection
    { group_encoding = new Fl_Group(10, 270, 280, 130, "Encoding:");
      group_encoding->box(FL_GTK_DOWN_BOX);
      group_encoding->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      { encoding_bitrate = new Fl_Input(95, 280, 175, 25, "Bitrate:");
        encoding_bitrate->box(FL_GTK_DOWN_BOX);
        encoding_bitrate->value("96");
      } // Fl_Input* encoding_bitrate
      { encoding_samplerate = new Fl_Input(95, 315, 175, 25, "Samplerate:");
        encoding_samplerate->box(FL_GTK_DOWN_BOX);
        encoding_samplerate->value("44100");
      } // Fl_Input* encoding_samplerate
      { Fl_Group* o = new Fl_Group(30, 350, 120, 40);
        { radio_mp3 = new Fl_Round_Button(35, 355, 50, 30, "mp3");
          radio_mp3->type(102);
          radio_mp3->down_box(FL_ROUND_DOWN_BOX);
          radio_mp3->value(1);
          radio_mp3->when(FL_WHEN_CHANGED);
        } // Fl_Round_Button* radio_mp3
        { radio_ogg = new Fl_Round_Button(95, 355, 45, 30, "ogg");
          radio_ogg->type(102);
          radio_ogg->down_box(FL_ROUND_DOWN_BOX);
          radio_ogg->when(FL_WHEN_CHANGED);
        } // Fl_Round_Button* radio_ogg
        o->end();
      } // Fl_Group* o
      group_encoding->end();
    } // Fl_Group* group_encoding
    { group_stream = new Fl_Group(305, 30, 350, 290, "Stream Info:");
      group_stream->box(FL_GTK_DOWN_BOX);
      group_stream->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      { stream_name = new Fl_Input(390, 45, 245, 25, "Name:");
        stream_name->box(FL_GTK_DOWN_BOX);
        stream_name->value("ShoutVST Stream");
      } // Fl_Input* stream_name
      { stream_url = new Fl_Input(390, 75, 245, 25, "URL:");
        stream_url->box(FL_GTK_DOWN_BOX);
        stream_url->value("https://github.com/R-Tur/ShoutVST");
      } // Fl_Input* stream_url
      { stream_genre = new Fl_Input(390, 105, 245, 25, "Genre:");
        stream_genre->box(FL_GTK_DOWN_BOX);
      } // Fl_Input* stream_genre
      { stream_artist = new Fl_Input(390, 135, 245, 25, "Artist:");
        stream_artist->box(FL_GTK_DOWN_BOX);
      } // Fl_Input* stream_artist
      { stream_title = new Fl_Input(390, 165, 245, 25, "Title:");
        stream_title->box(FL_GTK_DOWN_BOX);
      } // Fl_Input* stream_title
      { stream_description = new Fl_Text_Editor(390, 210, 245, 100, "Description:");
        stream_description->box(FL_GTK_DOWN_BOX);
        stream_description->align(Fl_Align(FL_ALIGN_LEFT_TOP));
        Fl_Text_Buffer   *buff = new Fl_Text_Buffer();
        stream_description->buffer(buff);
      } // Fl_Text_Editor* stream_description
      group_stream->end();
    } // Fl_Group* group_stream
    { Fl_Group* o = new Fl_Group(15, 430, 260, 40);
      { button_connect = new Fl_Button(15, 430, 125, 40, "Connect");
        button_connect->callback((Fl_Callback*)cb_button_connect);
      } // Fl_Button* button_connect
      { button_disconnect = new Fl_Button(150, 430, 125, 40, "Disconnect");
        button_disconnect->callback((Fl_Callback*)cb_button_disconnect);
      } // Fl_Button* button_disconnect
      o->end();
    } // Fl_Group* o
    { Fl_Button* o = new Fl_Button(640, 0, 25, 25, "?");
      o->callback((Fl_Callback*)cb_);
    } // Fl_Button* o
    { Fl_Group* o = new Fl_Group(320, 335, 330, 145);
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(305, 340, 350, 135, "Metadata:");
      o->box(FL_GTK_DOWN_BOX);
      o->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      { stream_metadata = new Fl_Text_Editor(315, 355, 245, 100);
        stream_metadata->box(FL_GTK_DOWN_BOX);
        stream_metadata->align(Fl_Align(FL_ALIGN_LEFT_TOP));
        Fl_Text_Buffer   *buff = new Fl_Text_Buffer();
        stream_metadata->buffer(buff);
      } // Fl_Text_Editor* stream_metadata
      { Fl_Button* o = new Fl_Button(570, 420, 75, 40, "Send");
        o->callback((Fl_Callback*)cb_Send);
      } // Fl_Button* o
      o->end();
    } // Fl_Group* o
    fl_window->clear_border();
    fl_window->size_range(667, 486, 667, 486);
    fl_window->end();
  } // Fl_Double_Window* fl_window
}

bool ShoutVSTEditorFL::show() {
  fl_window->show();
  return true;
}

void ShoutVSTEditorFL::hide() {
  fl_window->hide();
}

string ShoutVSTEditorFL::getEncodingFormat() {
  if(radio_mp3->value()){
    return "mp3";
  } 
  if(radio_ogg->value()){
    return "ogg";
  } 
  return "";
}

string ShoutVSTEditorFL::getHostName() {
  const char *p = connection_hostname->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getPort() {
  const char *p = connection_port->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getUserName() {
  const char *p = connection_username->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getPassword() {
  const char *p = connection_password->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getMountPoint() {
  const char *p = connection_mountpoint->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getBitRate() {
  const char *p = encoding_bitrate->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getSampleRate() {
  const char *p = encoding_samplerate->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamName() {
  const char *p = stream_name->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamURL() {
  const char *p = stream_url->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamGenre() {
  const char *p = stream_genre->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamArtist() {
  const char *p = stream_artist->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamTitle() {
  const char *p = stream_title->value();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamDescription() {
  const char *p = stream_description->buffer()->text();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getStreamMetaData() {
  const char *p = stream_metadata->buffer()->text();
  string result;
  if(p){
    result = p;
  }
  return result;
}

string ShoutVSTEditorFL::getProtocol() {
  if(proto_http->value()){
    return "http";
  } 
  if(proto_xaudiocast->value()){
    return "xaudiocast";
  } 
  if(proto_icy->value()){
    return "icy";
  } 
  if(proto_roaraudio->value()){
    return "roaraudio";
  } 
  return "http";
}

void ShoutVSTEditorFL::setDisable(bool disable) {
  if(disable){
    group_connection->deactivate();
    group_encoding->deactivate();
    group_stream->deactivate();
    button_connect->deactivate();
    button_disconnect->activate();
  }
  else{
    group_connection->activate();
    group_encoding->activate();
    group_stream->activate();
    button_connect->activate();
    button_disconnect->deactivate();
  }
}

void ShoutVSTEditorFL::showAbout() {
  about_window->show();
}

void ShoutVSTEditorFL::closeAbout() {
  about_window->hide();
}

ShoutVSTEditorFL::~ShoutVSTEditorFL() {
  closeAbout();
  delete about_window;
  about_window = nullptr;
  
  hide();
  delete fl_window;
  fl_window = nullptr;
}
