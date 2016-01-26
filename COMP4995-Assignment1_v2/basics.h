#pragma once
#ifndef __BASICS
#define __BASICS
#define BITMAP_PICTURE "tiger.bmp"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
void SetError(TCHAR*, ...);
HRESULT RestoreGraphics();
#endif