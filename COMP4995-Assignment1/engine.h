#ifndef __engine__h
#define __engine__h

/*Forward Declarations*/
int GameInit();
int GameLoop();
int GameShutdown();
void SetError(TCHAR*, ...);
int Render();
void Draw(int, DWORD*);
int LoadBitmapToSurface(TCHAR*, LPDIRECT3DSURFACE9*, LPDIRECT3DDEVICE9);
void SimpleBitmapDraw(TCHAR*, LPDIRECT3DSURFACE9, int, int);
HRESULT InitTiming();
int InitDirect3DDevice(HWND, int, int, BOOL, D3DFORMAT, LPDIRECT3D9, LPDIRECT3DDEVICE9*);
#endif
