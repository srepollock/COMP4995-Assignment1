
#ifndef __globals_h__
#define __globals_h__

#include <d3d9.h>

/************GLOBALS*****************************/
extern LPDIRECT3D9 g_pD3D;//COM object
extern LPDIRECT3DDEVICE9 g_pDevice;//graphics device
extern PDIRECT3DSURFACE9 g_pBackSurface;//backbuffer
extern HWND g_hWndMain;//handle to main window
extern INT64 g_Frequency;//holds number of ticks per second
extern INT64 g_FrameRate;//holds current frame rate calculated
extern INT64 g_FrameCount;//holds current number of frames counted before 1 second has passed

extern int g_AlphabetWidth;			// The width of the Alphabet bitmap
extern int g_AlphabetHeight;			// The height of the Alphabet bitmap
extern int g_AlphabetLetterWidth;		// The width of a letter
extern int g_AlphabetLetterHeight;		// The height of a letter
extern int g_AlphabetLettersPerRow;	// The number of letters per row

extern D3DPRESENT_PARAMETERS g_SavedPresParams; //holds current 3d setup

extern int g_DeviceHeight;
extern int g_DeviceWidth;


// The surface holding the alphabet bitmap
extern LPDIRECT3DSURFACE9 g_pAlphabetSurface;

// Has the alphabet bitmap been loaded yet?
extern BOOL g_bAlphabetLoaded;

//Vertext Buffer
extern LPDIRECT3DVERTEXBUFFER9 g_pVB;
/***********************************************/

#endif