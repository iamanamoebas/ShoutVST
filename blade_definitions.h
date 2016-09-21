#pragma once
#define _BLADEDLL

#ifdef _WIN32
#include <windows.h>
#endif

#ifndef _WINDEF_
typedef int INT;
typedef unsigned int UINT;
typedef unsigned int* PUINT;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef float FLOAT;
typedef FLOAT* PFLOAT;
typedef BYTE* PBYTE;
typedef DWORD* PDWORD;
typedef char CHAR;
typedef CHAR* LPSTR;
typedef const CHAR* LPCSTR;
typedef short SHORT;
typedef long LONG;
typedef SHORT* PSHORT;
typedef LONG* PLONG;
#define VOID void
#define FALSE 0
#define TRUE 1
#endif

#ifndef MAX_PATH
#ifdef __linux__
#include <linux/limits.h>
#define MAX_PATH PATH_MAX
#endif
#endif