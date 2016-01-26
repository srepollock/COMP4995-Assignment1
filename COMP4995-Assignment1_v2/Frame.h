#pragma once
#ifndef __FRAME
#define __FRAME
class Frame {
private:
	int frame_counter = 0;
	int FrameRate = 0;
	int frequency = 0;

	LPDIRECT3DDEVICE9 pDevice = 0;

	int g_AlphabetWidth = 0;
	int g_AlphabetHeight = 0;
	int g_AlphabetLetterWidth = 0;
	int g_AlphabetLetterHeight = 0;
	int g_AlphabetLettersPerRow = 0;
	LPDIRECT3DSURFACE9 g_pAlphabetSurface = 0;
	BOOL g_bAlphabetLoaded = FALSE;
public:
	Frame();
	Frame(LPDIRECT3DDEVICE9);
	~Frame();
	void FrameCount();
	void incFrame_Counter();
	HRESULT InitTiming();
	void setFrame_Counter(int);
	int getFrame_Counter();
	void setFrameRate(int);
	void setFrequency(int);
	HRESULT LoadAlphabet(TCHAR*, int, int);
	HRESULT UnloadAlphabet();
	void PrintChar(int, int, char, BOOL, D3DCOLOR, DWORD*, int);
	void PrintString(int, int, char*, BOOL, D3DCOLOR, DWORD*, int);
	void PrintFrameRate(int, int, BOOL, D3DCOLOR, DWORD*, int);
	int LoadBitmapToSurface(TCHAR*, LPDIRECT3DSURFACE9*, LPDIRECT3DDEVICE9);
};

#endif
