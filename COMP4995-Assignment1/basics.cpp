#include "IncludeFiles.h"

HRESULT InitTiming() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency);

	if (g_Frequency == 0) {
		SetError(_T("The system does not support high resolution timing"));
		return E_FAIL;
	}

	return S_OK;
}

void FrameCount() {
	INT64 NewCount = 0;
	static INT64 LastCount = 0;
	INT64 Difference = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&NewCount);

	if (NewCount == 0)
		SetError(_T("The system does not support high resolution timing"));

	g_FrameCount++;

	Difference = NewCount - LastCount;

	if (Difference >= g_Frequency) {
		g_FrameRate = g_FrameCount;
		g_FrameCount = 0;

		LastCount = NewCount;
	}
}

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
	//	d3dpp.FullScreen_PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		SetError(_T("Could not create the render device"));
		return E_FAIL;
	}

	g_DeviceHeight = Height;
	g_DeviceWidth = Width;

	g_SavedPresParams = d3dpp;
	return S_OK;
}

void SetError(TCHAR* szFormat, ...) {
	TCHAR szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);

	va_end(pArgList);

	OutputDebugString(szBuffer);
	OutputDebugString(_T("\n"));
}

HRESULT ValidateDevice() {
	HRESULT r = 0;
	//Test current state of device
	r = g_pDevice->TestCooperativeLevel();
	if (FAILED(r)) {
		//if device is lost then return failure
		if (r == D3DERR_DEVICELOST)
			return E_FAIL;
		//if device is ready to be reset then try
		if (r == D3DERR_DEVICENOTRESET) {
			//release back surface
			g_pBackSurface->Release();
			//reset device
			r = g_pDevice->Reset(&g_SavedPresParams);
			if (FAILED(r)) {
				//device was not ready to be reset
				SetError(_T("Could not reset device"));
				PostQuitMessage(E_FAIL);
				return E_FAIL;
			}
			//reacquire a pointer to new back buffer
			r = g_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &g_pBackSurface);
			if (FAILED(r)) {
				SetError(_T("Unable to reacquire back buffer"));
				PostQuitMessage(0);
				return E_FAIL;
			}
			g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
			RestoreGraphics();
		}
	}
	return S_OK;
}

HRESULT RestoreGraphics() {
	return S_OK;
}
