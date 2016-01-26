#include "Includes.h"

Draw::Draw() {

}

Draw::~Draw() {

}

void Draw::draw(int Pitch, DWORD* pData) {
	int length = SCREEN_WIDTH * SCREEN_HEIGHT;
	for (double i = 0; i < length; i++) {
		int x = pStart.x + (i / length) * (pEnd.x - pStart.x);
		int y = pStart.y + (i / length) * (pEnd.y - pStart.y);
		DWORD Offset = y * Pitch / 4 + x;
		pData[Offset] = D3DCOLOR_XRGB(255, 0, 0);
	}
}

void Draw::setPStart(POINT p) {
	pStart = p;
}
void Draw::setPEnd(POINT p) {
	pEnd = p;
}
POINT Draw::getPStart() {
	return pStart;
}
POINT Draw::getPEnd() {
	return pEnd;
}