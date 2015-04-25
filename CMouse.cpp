#include "CMouse.h"

CMouse::CMouse()
{
	leftButtonDown = false;
}

POINT CMouse::getMouseLocation(HWND windowHandle)
{
	POINT mousePosition;
	POINT mouseLoc;

	if(SUCCEEDED(GetCursorPos(&mousePosition)))
	{
		int xCo = mousePosition.x;	
		int yCo = mousePosition.y;	
		
		//Title Bar Co-Ordinates
		TITLEBARINFO titleBarInfo;
		titleBarInfo.cbSize = sizeof(TITLEBARINFO);
		GetTitleBarInfo(windowHandle, &titleBarInfo);
		
		RECT titleBarRect = titleBarInfo.rcTitleBar;

		int titleLeft = titleBarRect.left;	
		int titleRight = titleBarRect.right;	
		int titleTop = titleBarRect.top;	
		int titleBottom = titleBarRect.bottom;	

		//get window location
		RECT windowLocation;
		int windowLeft;
		int windowTop;
		int windowRight;
		int windowBottom;

		if(SUCCEEDED(GetWindowRect(windowHandle, &windowLocation)))
		{	
			windowLeft = windowLocation.left;
			windowTop = windowLocation.top;
			windowRight = windowLocation.right;
			windowBottom = windowLocation.bottom;
		}

		//Calculate real mouse co-ordinates
		int realMouseX = xCo - windowLeft - (windowRight - titleRight);
		
		int realMouseY = yCo - titleBottom;

		mouseLoc.x = realMouseX;
		mouseLoc.y = realMouseY;
	}

	return mouseLoc;
}

void CMouse::setLeftButtonDown()
{
	leftButtonDown = true;
}

void CMouse::setLeftButtonUp()
{
	leftButtonDown = false;
}

bool CMouse::isButtonDown()
{
	return leftButtonDown;
}