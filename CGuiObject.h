#include <windows.h>
#include <d3dx9.h>
#include "resource.h"

//inclusion guards
#ifndef GuiObject
#define GuiObject

class CGuiObject
{
public:
	CGuiObject();
	CGuiObject(LPDIRECT3DDEVICE9 l_directXDevice, char* id, int l_width, int l_height, 
					int l_screenWidth, int l_screenHeight, int xLoc, int yLoc);
	
	HRESULT setImage(const char *filePath, DWORD keyColour);
	
	char* getID();
	int getObjectWidth();
	int getObjectHeight();
	int getScreenWidth();
	int getScreenHeight();
	POINT getLocation();
	void setLocation(int x, int y);
	bool mouseIsOver(int mouseX, int mouseY);

	void render(double xLoc, double yLoc);

protected:
	char* ID;
	int objectWidth;
	int objectHeight;
	int screenWidth;
	int screenHeight;
	POINT location;
	
	LPDIRECT3DDEVICE9 directXDevice;	// Device Pointer
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 texture;

};

#endif