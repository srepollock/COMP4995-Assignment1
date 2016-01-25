#include "IncludeFiles.h"

int GameInit() {
	HRESULT r = 0;//return values
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurface = 0;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (g_pD3D == NULL) {
		SetError(_T("Could not create IDirect3D9 object"));
		return E_FAIL;
	}
												// FALSE = full screen TRUE = windowed
	r = InitDirect3DDevice(g_hWndMain, 640, 480, FALSE, D3DFMT_X8R8G8B8, g_pD3D, &g_pDevice);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(_T("Initialization of the device failed"));
		return E_FAIL;
	}

	// This is where I setup the surface
	r = g_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	pSurface->GetDesc(&desc);

	r = g_pDevice->CreateOffscreenPlainSurface(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &g_BitmapSurface, NULL);

	r = LoadBitmapToSurface(_T("tiger.bmp"), &pSurface, g_pDevice);
	if (FAILED(r))
		SetError(_T("Error loading bitmap"));

	r = D3DXLoadSurfaceFromSurface(g_BitmapSurface, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
	if (FAILED(r))
		SetError(_T("did not copy surface"));

	LoadAlphabet(_T("Alphabet vSmall.bmp"), 8, 16);
	InitTiming();

	return S_OK;
}

int GameLoop() {

	FrameCount();
	Render();

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}

int GameShutdown() {

	UnloadAlphabet();

	if (g_pVB)
		g_pVB->Release();

	//release resources. First display adapter because COM object created it, then COM object
	if (g_pBackSurface)
		//g_pBackSurface->Release();

		if (g_pDevice)
			g_pDevice->Release();

	if (g_pD3D)
		g_pD3D->Release();

	return S_OK;
}

int Render() {
	HRESULT r;
	g_pBackSurface = 0;

	if (!g_pDevice) {
		SetError(_T("Cannot render because there is no device"));
		return E_FAIL;
	}
	g_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 55), 1.0f, 0);
	r = ValidateDevice();
	if (FAILED(r)) {
		return E_FAIL;
	}
	r = g_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &g_pBackSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
												// NULL for fullscreen
	r = g_pDevice->UpdateSurface(g_BitmapSurface, NULL, g_pBackSurface, NULL);
	if (FAILED(r)) {
		SetError(_T("Error setting surface to back surface"));
	}

	D3DLOCKED_RECT Locked;
	g_pBackSurface->LockRect(&Locked, 0, 0);
	PrintFrameRate(40, 50, TRUE, D3DCOLOR_ARGB(255, 255, 0, 255), (DWORD*)Locked.pBits, Locked.Pitch);
	g_pBackSurface->UnlockRect();
	
	//Start to render in 3D
	g_pDevice->BeginScene();
	//finish rendering
	g_pDevice->EndScene();
	
	//console work
	g_pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

void Draw(int Pitch, DWORD* pData) {
	DWORD Offset = 100 * Pitch / 4 + 200;
	pData[Offset] = D3DCOLOR_XRGB(255, 0, 0);

	//SimpleBitmapDraw();
}
void SimpleBitmapDraw(TCHAR* PathName, LPDIRECT3DSURFACE9 pBackSurf, int dpx, int dpy) {
	LPDIRECT3DSURFACE9 pSurface = 0;

	LoadBitmapToSurface(PathName, &pSurface, g_pDevice);

	D3DSURFACE_DESC d3dsd;
	pSurface->GetDesc(&d3dsd);//get info about surface

	POINT DestPoint = { dpx, dpy };
	RECT rect = { 0,0, d3dsd.Width, d3dsd.Height };//source dimensions

												   //g_pDevice->CopyRects(pSurface, &rect, 1, pBackSurf, &DestPoint);//copy surface to buffer (like a bitblt)in D3D8
	g_pDevice->StretchRect(pSurface, &rect, pBackSurf, NULL, D3DTEXF_NONE);//replaces copyrects for D3D9
																		   //	pSurface->Release();
																		   //	pSurface = 0;

																		   //	pBackSurf->Release();
																		   //	pBackSurf = 0;
																		   //
																		   //	g_pDevice->Present(NULL, NULL, NULL, NULL);//put it on the primary surface
}

int LoadBitmapToSurface(TCHAR* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		SetError(_T("Unable to load bitmap"));
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

						  //create surface for bitmap
	r = pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, ppSurface, NULL);

	if (FAILED(r)) {
		SetError(_T("Unable to create surface for bitmap load"));
		return E_FAIL;
	}
	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		SetError(_T("Unable to laod file to surface"));
		return E_FAIL;
	}

	return S_OK;
}
