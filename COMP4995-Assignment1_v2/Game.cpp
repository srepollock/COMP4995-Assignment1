#include "Includes.h"

// Default constructor for the game object
Game::Game() {
	d = Draw();
}

// Constructor for the game object with a handle to the main window
Game::Game(HWND hWnd) {
	this->hWndMain = hWnd;
	d = Draw();
}

// Default destructor for the game object
Game::~Game() {

}

// Sets the handle
void Game::setHandle(HWND hWnd) {
	hWndMain = hWnd;
}

// Gets the handle
HWND Game::getHandle() {
	return hWndMain;
}

// Static WndProc to be used when initializing. This will setup the class to call
// the regular WndProc
LRESULT CALLBACK Game::StaticWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	Game* ptr;
	ptr = (Game*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	if (ptr != nullptr) {
		return ptr->WndProc(hWnd, uMessage, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
}

// Windows Proc for the window
LRESULT CALLBACK Game::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	D3DLOCKED_RECT rect;
	DWORD* pData;
	POINT pS, pE;
	switch (uMessage) {
	case WM_CREATE: {
		return 0;
	}
	case WM_PAINT:{
		ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
		return 0;
	}
	case WM_CHAR: {
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		return 0;
	}
	case WM_LBUTTONDOWN:
		GetCursorPos(&pS);
		d.setPStart(pS);
		d.setPEnd(pS);
		return 0;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			// holding down the left button
			GetCursorPos(&pE);
			d.setPEnd(pE);
		}
		return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	}
}

// Game initialization. This will setup the directx defice and window
// Loads in the alphabet for the frame counter
int Game::GameInit() {
	HRESULT r = 0;//return values
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurface = 0;

	this->pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (this->pD3D == NULL) {
		SetError(_T("Could not create IDirect3D9 object"));
		return E_FAIL;
	}
												  // FALSE = full screen TRUE = windowed
	r = InitDirect3DDevice(this->hWndMain, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE, D3DFMT_X8R8G8B8, this->pD3D, &this->pDevice);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(_T("Initialization of the device failed"));
		return E_FAIL;
	}

	// This is where I setup the surface
	r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	pSurface->GetDesc(&desc);

	r = this->pDevice->CreateOffscreenPlainSurface(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &this->BitmapSurface, NULL);

	r = LoadBitmapToSurface(_T(BITMAP_PICTURE), &pSurface, this->pDevice);
	if (FAILED(r))
		SetError(_T("Error loading bitmap"));

	r = D3DXLoadSurfaceFromSurface(this->BitmapSurface, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
	if (FAILED(r))
		SetError(_T("did not copy surface"));

	frameController = Frame(pDevice);

	frameController.LoadAlphabet(_T("Alphabet vSmall.bmp"), 8, 16);
	frameController.InitTiming();

	return S_OK;
}

// Game loop for the directx counter, and gets the updated frame count to render fps
int Game::GameLoop() {
	frameController.FrameCount();
	this->Render();

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}

// Shutsdown the game and releases the device back to windows
int Game::GameShutdown() {

	frameController.UnloadAlphabet();

	if (this->pVB)
		this->pVB->Release();

	//release resources. First display adapter because COM object created it, then COM object
	if (this->pBackSurface)
		if (this->pDevice)
			this->pDevice->Release();

	if (this->pD3D)
		this->pD3D->Release();

	return S_OK;
}

// Loads bitmap from memory to the surface.
// IMAGE_BITMAP is defined in basics for ease of change
int Game::LoadBitmapToSurface(TCHAR* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
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

// Initializes the directx device and sets up buffers
int Game::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
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

	this->DeviceHeight = Height;
	this->DeviceWidth = Width;

	this->SavedPresParams = d3dpp;
	return S_OK;
}

// Updates the device's surface. This is called each time the game loops run and gets the surface
// currently stored on the back surface buffer, then will start loading the back surface buffer
// before finishing
int Game::Render() {
	HRESULT r;
	this->pBackSurface = 0;

	if (!this->pDevice) {
		SetError(_T("Cannot render because there is no device"));
		return E_FAIL;
	}
	this->pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 55), 1.0f, 0);
	r = ValidateDevice();
	if (FAILED(r)) {
		return E_FAIL;
	}
	r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->pBackSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	// NULL for fullscreen
	r = this->pDevice->UpdateSurface(this->BitmapSurface, NULL, this->pBackSurface, NULL);
	if (FAILED(r)) {
		SetError(_T("Error setting surface to back surface"));
	}

	D3DLOCKED_RECT Locked;
	this->pBackSurface->LockRect(&Locked, 0, 0);
	frameController.PrintFrameRate(40, 50, TRUE, D3DCOLOR_ARGB(255, 255, 0, 255), (DWORD*)Locked.pBits, Locked.Pitch);	
	this->pBackSurface->UnlockRect();

	this->pBackSurface->LockRect(&Locked, 0, 0);
	d.draw(Locked.Pitch, (DWORD*)Locked.pBits);
	this->pBackSurface->UnlockRect();

	//Start to render in 3D
	this->pDevice->BeginScene();
	//finish rendering
	this->pDevice->EndScene();

	//console work
	this->pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}

// Validates that the device has been setup and is useable
HRESULT Game::ValidateDevice() {
	HRESULT r = 0;
	//Test current state of device
	r = this->pDevice->TestCooperativeLevel();
	if (FAILED(r)) {
		//if device is lost then return failure
		if (r == D3DERR_DEVICELOST)
			return E_FAIL;
		//if device is ready to be reset then try
		if (r == D3DERR_DEVICENOTRESET) {
			//release back surface
			this->pBackSurface->Release();
			//reset device
			r = this->pDevice->Reset(&this->SavedPresParams);
			if (FAILED(r)) {
				//device was not ready to be reset
				SetError(_T("Could not reset device"));
				PostQuitMessage(E_FAIL);
				return E_FAIL;
			}
			//reacquire a pointer to new back buffer
			r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->pBackSurface);
			if (FAILED(r)) {
				SetError(_T("Unable to reacquire back buffer"));
				PostQuitMessage(0);
				return E_FAIL;
			}
			this->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
			this->RestoreGraphics();
		}
	}
	return S_OK;
}

// Returns an ok message
HRESULT Game::RestoreGraphics() {
	return S_OK;
}