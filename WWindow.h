#pragma once
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
//#include <iostream.h>

#define SafeRelease(pObject) if(pObject != NULL) {pObject->Release(); pObject=NULL;}

//---------------------------------------------------------------------------
class WWindow
{
public:
	// We will use a default constructor to declare a window
	WWindow();
	// The Create() method wil be used to initialize a window
	HWND Create(HINSTANCE hinst,
				LPCTSTR clsname,
     		    LPCTSTR wndname,
	            HWND parent   = NULL,
	            DWORD dStyle  = WS_OVERLAPPEDWINDOW,
	            DWORD dXStyle = 0L,
				int x         = CW_USEDEFAULT,
                int y         = CW_USEDEFAULT,
				int width     = CW_USEDEFAULT,
                int height    = CW_USEDEFAULT);

	// This method will be used to display the window
	BOOL Show(int dCmdShow = SW_SHOWNORMAL);

	// Because each window is of type HWND, we will need a way
	// to recognize the window handle when used in our application
	operator HWND();

	/************************************************************
	*GLOBALS
	*************************************************************/
	LPDIRECT3D9	g_pD3D;	// Direct3D Pointer
	LPDIRECT3DDEVICE9 g_pD3DDevice;	// Device Pointer

	HWND getHandle();
	void setHandle(HWND newHandle);
	HRESULT InitialiseD3D(HWND hWnd);
	void CleanUp();
	int getScreenWidth();
	int getScreenHeight();
	void setScreenWidth(int newWidth);
	void setScreenHeight(int newHeight);

private:
	HWND handle;
	int screenWidth;
	int screenHeight;

};
//---------------------------------------------------------------------------