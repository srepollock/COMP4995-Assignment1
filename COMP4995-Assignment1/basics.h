#ifndef __basics_h__
#define __basics_h__
#define DRVERTEX_FLAGS (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
struct DRVERTEX {
	float x, y, z, rhw;
	DWORD DiffuseColor;
};

HRESULT InitTiming();
HRESULT ValidateDevice();
HRESULT RestoreGraphics();
void FrameCount();
int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
void SetError(TCHAR* szFormat, ...);
#endif