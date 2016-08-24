#pragma once
#include <windows.h>
#include <aeffeditor.h>
#include "audioeffectx.h"

class ShoutVST;

class ShoutVSTEditor :
  public AEffEditor
{
public:
  ShoutVSTEditor(AudioEffectX *effect);
  virtual ~ShoutVSTEditor();
  virtual bool open(void *ptr);
  virtual bool getRect(ERect **erect);

  INT_PTR DialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  void AppendLog(char *);

  char szHostname[MAX_PATH];
  int nPort;
  char szUsername[MAX_PATH];
  char szPassword[MAX_PATH];
  char szMountpoint[MAX_PATH];
  int nProtocol;

  int nEncoder;

  int GetQuality();
  void SetQuality( int q );

  void RefreshData();
  void DisableAccordingly();

protected:
  ShoutVST * pVST;
  HWND hwndParent;
  HWND hwndDialog;

  int nQuality;

  char * szLog;
};
