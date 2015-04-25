#include <windows.h>

//inclusion guards
#ifndef Mouse
#define Mouse

class CMouse
{
public:
	CMouse();
	POINT getMouseLocation(HWND windowHandle);
	void setLeftButtonDown();
	void setLeftButtonUp();
	bool isButtonDown();

private:
	bool leftButtonDown;

};

#endif