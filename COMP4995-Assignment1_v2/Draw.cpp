#include "Includes.h"

// Default constructor
Draw::Draw() {

}

// Default destructor
Draw::~Draw() {

}

// draw method of the Draw object. This will set the pixels on the screen relative
// to the positions of the start and end
void Draw::draw(int Pitch, DWORD* pData) {
	int length = SCREEN_WIDTH * SCREEN_HEIGHT;
	for (double i = 0; i < length; i++) {
		int x = pStart.x + (i / length) * (pEnd.x - pStart.x);
		int y = pStart.y + (i / length) * (pEnd.y - pStart.y);
		DWORD Offset = y * Pitch / 4 + x;
		pData[Offset] = D3DCOLOR_XRGB(255, 0, 0);
	}
}

// Sets the start position
void Draw::setPStart(POINT p) {
	pStart = p;
}
// Sets the end position
void Draw::setPEnd(POINT p) {
	pEnd = p;
}
// Gets the start position
POINT Draw::getPStart() {
	return pStart;
}
// Gets the end position
POINT Draw::getPEnd() {
	return pEnd;
}