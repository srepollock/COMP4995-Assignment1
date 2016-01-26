#pragma once
#ifndef _DRAW
#define _DRAW
class Draw {
private:
	POINT pStart = { 0, 0 };
	POINT pEnd = { 0, 0 };
public:
	Draw();
	~Draw();
	void draw(int, DWORD*);
	void setPStart(POINT);
	void setPEnd(POINT);
	POINT getPStart();
	POINT getPEnd();
};
#endif