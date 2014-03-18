#ifndef _mainWindowFuncs_H
#define _mainWindowFuncs_H
#include <windows.h>
#include <time.h>
#include <Tchar.h>//_T()函数在此头文件\

#include "c3dAL.h"
#include "c3dGL.h"

//c3d
#include "c3dGestureAnalyzer.h"
#include "c3dSceneManger.h"
#include "c3dAutoreleasePool.h"
#include "c3dDeviceAndOSInfo.h"
#include "c3dAppState.h"
#include "c3dTextureCache.h"
#include "c3dAudioCache.h"
#include "c3dTimeCounter.h"
#include "c3dAssert.h"
//
#include "initGame.h"
//
#include "resource.h "
//
#include "mainWindowGlobal.h"
//
void keyDown(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void keyUp(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void lbuttonDown(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void lbuttonUp(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void mouseMove(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void mouseLeave(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void winSize(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
void command(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void render(HDC hDC);
void initOpenGL();
void initOpenAL();
void teardownOpenAL();
void initWithFrame();
bool IsExtensionSupported( char* szTargetExtension ) ;
void createAndInitConsole();

#endif