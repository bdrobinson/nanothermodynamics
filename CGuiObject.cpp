#include "CGuiObject.h"

CGuiObject::CGuiObject()
{

}

CGuiObject::CGuiObject(LPDIRECT3DDEVICE9 l_directXDevice, char* id, int l_width, int l_height, 
					int l_screenWidth, int l_screenHeight, int xLoc, int yLoc)
{
	directXDevice = l_directXDevice;

	texture = NULL;
	sprite=NULL;

	ID = id;
	objectWidth = l_width;
	objectHeight = l_height;
	screenWidth = l_screenWidth;
	screenHeight = l_screenHeight;
	location.x = xLoc;
	location.y = yLoc;

	D3DXCreateSprite(directXDevice,&sprite);

}

bool CGuiObject::mouseIsOver(int mouseX, int mouseY)
{
	if( 
		(mouseX >= location.x) && (mouseX <= location.x + objectWidth)
		&&
		(mouseY >= location.y) && (mouseY <= location.y + objectHeight)
	  )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void CGuiObject::render(double xLoc, double yLoc)
{
	D3DXVECTOR3 pos;

	pos.x=(float)xLoc;
	pos.y=(float)yLoc;
	pos.z=0.0f;

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	// Texture being used is 512 by 128:
	D3DXVECTOR2 spriteCentre=D3DXVECTOR2((float)xLoc/1.0f,(float)yLoc/1.0f);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling(objectWidth/128.0f,objectHeight/128.0f);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,&spriteCentre,0.0,&scaling,0,0,0);
	// Tell the sprite about the matrix
	sprite->SetTransform(&mat);
	
	D3DCOLOR spriteColour = D3DCOLOR_XRGB(255,255,255); 

	sprite->Draw(texture,0,NULL,&pos,spriteColour);
	sprite->End();
}

HRESULT CGuiObject::setImage(const char *filePath, DWORD keyColour)
{	
	if(FAILED(D3DXCreateTextureFromFileEx(directXDevice, filePath, 0, 0, 0, 0,
										  D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
										  D3DX_DEFAULT, keyColour, NULL, NULL, &texture)))
	//if(FAILED(D3DXCreateTextureFromResource(directXDevice, NULL, IDB_BITMAP1, &texture )))
	{
		return E_FAIL;
	}	

	return S_OK;
}

char* CGuiObject::getID()
{
	return ID;
}

POINT CGuiObject::getLocation()
{
	return location;
}

int CGuiObject::getObjectHeight()
{
	return objectHeight;
}

int CGuiObject::getObjectWidth()
{
	return objectWidth;
}

int CGuiObject::getScreenHeight()
{
	return screenHeight;
}

int CGuiObject::getScreenWidth()
{
	return screenWidth;
}

void CGuiObject::setLocation(int x, int y)
{
	location.x = x;
	location.y = y;
}