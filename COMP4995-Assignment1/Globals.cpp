#include "IncludeFiles.h"

LPDIRECT3D9 g_pD3D;//COM object
LPDIRECT3DDEVICE9 g_pDevice;//graphics device
LPDIRECT3DSURFACE9 g_pBackSurface = 0;//backbuffer
PDIRECT3DSURFACE9 g_BitmapSurface = 0;//surface for the bitmap
HWND g_hWndMain;//handle to main window
INT64 g_Frequency = 0;//holds number of ticks per second
INT64 g_FrameRate = 0;//holds current frame rate calculated
INT64 g_FrameCount = 0;//holds current number of frames counted before 1 second has passed

D3DPRESENT_PARAMETERS g_SavedPresParams; //holds current 3d setup

int g_DeviceHeight;
int g_DeviceWidth;

int g_AlphabetWidth = 0;			// The width of the Alphabet bitmap
int g_AlphabetHeight = 0;			// The height of the Alphabet bitmap
int g_AlphabetLetterWidth = 0;		// The width of a letter
int g_AlphabetLetterHeight = 0;		// The height of a letter
int g_AlphabetLettersPerRow = 0;	// The number of letters per row

									// The surface holding the alphabet bitmap
LPDIRECT3DSURFACE9 g_pAlphabetSurface = 0;

// Has the alphabet bitmap been loaded yet?
BOOL g_bAlphabetLoaded = FALSE;

//Vertex Buffer
LPDIRECT3DVERTEXBUFFER9 g_pVB = 0;

/***********************************************/