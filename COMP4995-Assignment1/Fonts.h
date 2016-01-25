#ifndef __Fonts_h__
#define __Fonts_h__

HRESULT LoadAlphabet(TCHAR* strPathName, int LetterWidth, int LetterHeight);
HRESULT UnloadAlphabet();
void PrintChar(int x, int y, char Character, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch);
void PrintString(int x, int y, char* String, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch);
void PrintFrameRate(int x, int y, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch);
#endif