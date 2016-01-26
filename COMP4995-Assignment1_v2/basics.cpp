#include "Includes.h"

// Outputs an error to the command line based on the input
void SetError(TCHAR* szFormat, ...) {
	TCHAR szBuffer[1024];
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf_s(szBuffer, sizeof(szBuffer) / sizeof(char), szFormat, pArgList);

	va_end(pArgList);

	OutputDebugString(szBuffer);
	OutputDebugString(_T("\n"));
}

// Returns that the graphcis were ok
HRESULT RestoreGraphics() {
	return S_OK;
}