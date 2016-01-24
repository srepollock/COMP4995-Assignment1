//put all include files here
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9")
#include <stdio.h>
#include <d3d10.h>
#include <d3d9.h>
#include <d3dx9tex.h>

/*Forward Declarations*/
int GameInit();
int GameLoop();
int GameShutdown();
void SetError(TCHAR*);
int Render();
void Draw(int, DWORD*);
int LoadBitmapToSurface(TCHAR*, LPDIRECT3DSURFACE9*, LPDIRECT3DDEVICE9);
void SimpleBitmapDraw(TCHAR*, LPDIRECT3DSURFACE9, int, int);

//int InitDirect3DDevice(HWND, int, int, BOOL, D3DFORMAT, LPDIRECT3D8, LPDIRECT3DDEVICE8*);

/************GLOBALS*****************************/
LPDIRECT3D9 g_pD3D = 0;//COM object
LPDIRECT3DDEVICE9 g_pDevice = 0;//graphics device
HWND g_hWndMain;//handle to main window
				/***********************************************/

int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(r)) {
		SetError(_T("Could not get display adapter information"));
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		SetError(_T("Could not create the render device"));
		return E_FAIL;
	}

	//	g_DeviceHeight = Height;
	//	g_DeviceWidth = Width;

	//	g_SavedPresParams = d3dpp;
	return S_OK;
}

void SetError(TCHAR* szFormat) {
	MessageBox(NULL, szFormat, MB_OK, NULL);
	/*
	char szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);

	va_end(pArgList);

	OutputDebugString(szBuffer);
	OutputDebugString("\n");
	printf("error!!");
	*/
}