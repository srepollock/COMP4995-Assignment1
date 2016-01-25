#include "IncludeFiles.h"

// Used to load an alphabet bitmap into memory
HRESULT LoadAlphabet(TCHAR* strPathName, int LetterWidth, int LetterHeight)
{
	// Make sure a valid path was specified
	if (!strPathName)
		return E_FAIL;

	// Make sure the size of the letters is greater than 0
	if (!LetterWidth || !LetterHeight)
		return E_FAIL;

	HRESULT r = 0;

	// Load the bitmap into memory
	r = LoadBitmapToSurface(strPathName, &g_pAlphabetSurface, g_pDevice);
	if (FAILED(r))
	{
		SetError(_T("Unable to load alphabet bitmap"));
		return E_FAIL;
	}

	// Holds information about the alpahbet surface
	D3DSURFACE_DESC d3dsd;

	// Get information about the alphabet surface
	g_pAlphabetSurface->GetDesc(&d3dsd);

	// Update globals with the letter dimensions
	g_AlphabetWidth = d3dsd.Width;
	g_AlphabetHeight = d3dsd.Height;
	g_AlphabetLetterWidth = LetterWidth;
	g_AlphabetLetterHeight = LetterHeight;

	// Compute the number of letters in a row
	g_AlphabetLettersPerRow = g_AlphabetWidth / g_AlphabetLetterWidth;

	// Set the loaded flag to TRUE
	g_bAlphabetLoaded = TRUE;

	return S_OK;
}

// Unloads the alphabet from memory
HRESULT UnloadAlphabet()
{
	// Check if the alphabet exists
	if (g_pAlphabetSurface)
	{
		// Release the surface
		g_pAlphabetSurface->Release();
		// NULL the pointer
		g_pAlphabetSurface = 0;
		// Set the loaded flag to FALSE
		g_bAlphabetLoaded = FALSE;
	}

	return S_OK;
}

// Print a character to a surface using the loaded alphabet
void PrintChar(int x, int y, char Character, BOOL bTransparent,
	D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch)
{
	HRESULT r = 0;

	div_t Result;	// Holds the result of divisions

					// The offset into the alphabet image
	int OffsetX = 0, OffsetY = 0;

	POINT LetterDestPoint = { 0, 0 };	// The destination point for the letter
	RECT LetterRect = { 0, 0, 0, 0 };	// The source rectangle for the letter

										// If the alphabet has not been loaded yet then exit
	if (!g_bAlphabetLoaded)
		return;

	// The characters are specified in ASCII code, which begins at 32 so
	// we want to decrement this value by 32 to make it zero based
	Character -= 32;

	// Avoid divide by 0 errors
	if (Character == 0)
		return;

	// Divide the character code by the number of letters per row.
	// The quotient will help get the vertical offset and the
	// remainder will help get the horizontal offset
	Result = div(Character, g_AlphabetLettersPerRow);

	// Get the horizontal offset by multiplying the remainder
	// by the width of the Letter
	OffsetX = Result.rem * g_AlphabetLetterWidth;
	// Get the vertical offset by multiplying the quotient
	// by the height of the letter
	OffsetY = Result.quot * g_AlphabetLetterHeight;

	// Fill in the source rectangle with the computed offsets
	SetRect(&LetterRect, OffsetX, OffsetY,
		OffsetX + g_AlphabetLetterWidth, OffsetY + g_AlphabetLetterHeight);

	// Fill in the destination point
	LetterDestPoint.x = x;
	LetterDestPoint.y = y;

	D3DLOCKED_RECT LockedAlphabet;	// Holds info about the alphabet surface

									// Lock the source surface
	r = g_pAlphabetSurface->LockRect(&LockedAlphabet, 0, D3DLOCK_READONLY);
	if (FAILED(r))
	{
		SetError(_T("Couldnt lock alphabet surface for PrintChar()"));
		return;
	}

	// Get a DWORD pointer to each surface
	DWORD* pAlphaData = (DWORD*)LockedAlphabet.pBits;

	// Convert the BYTE pitch pointer to a DWORD ptr
	LockedAlphabet.Pitch /= 4;
	DestPitch /= 4;

	// Compute the offset into the alphabet
	int AlphaOffset = OffsetY * LockedAlphabet.Pitch + OffsetX;
	// Compute the offset into the destination surface
	int DestOffset = y * DestPitch + x;

	// Loop for each row in the letter
	for (int cy = 0; cy < g_AlphabetLetterHeight; cy++)
	{
		// Loop for each column in the letter
		for (int cx = 0; cx < g_AlphabetLetterWidth; cx++)
		{
			if (bTransparent)
			{
				// If this alphabet pixel is not transparent
				if (pAlphaData[AlphaOffset] != ColorKey)
				{
					// Then copy the pixel to the destination
					pDestData[DestOffset] = pAlphaData[AlphaOffset];
				}

				// Increment the offsets to the next pixel
				AlphaOffset++;
				DestOffset++;
			}
			else
				pDestData[DestOffset] = pAlphaData[AlphaOffset];
		}

		// Move the offsets to the start of the next row
		DestOffset += DestPitch - g_AlphabetLetterWidth;
		AlphaOffset += LockedAlphabet.Pitch - g_AlphabetLetterWidth;
	}

	// Unlock the surface
	g_pAlphabetSurface->UnlockRect();

}



void PrintString(int x, int y, char* String, BOOL bTransparent,
	D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch)
{
	// Loop for each character in the string
	for (UINT i = 0; i < strlen(String); i++)
	{
		// Print the current character
		PrintChar(x + (g_AlphabetLetterWidth * i), y, String[i],
			bTransparent, ColorKey, pDestData, DestPitch);
	}
}

// Prints the frame rate to the screen
void PrintFrameRate(int x, int y, BOOL bTransparent, D3DCOLOR ColorKey, DWORD* pDestData, int DestPitch)
{
	char string[10];	// String to hold the frame rate

						// Zero out the string
	ZeroMemory(&string, sizeof(string));

	// Convert the frame rate to a string
	_itoa_s((int)g_FrameRate, string, 10);

	// Output the string to the back surface
	PrintString(x, y, string, TRUE, D3DCOLOR_ARGB(255, 255, 0, 255), pDestData, DestPitch);
}

