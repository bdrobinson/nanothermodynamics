#pragma once
#include <d3dx9.h>
//#include <iostream.h>

//inclusion guards
#ifndef Font
#define Font

class CFont 
{
public:
	CFont();
	CFont(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pFontFace, int nHeight, bool fBold, bool fItalic, bool fUnderlined);
	void DrawText(LPCTSTR pText, int x, int y, D3DCOLOR rgbFontColour);
	void setFontType(LPCTSTR newFontType);
	void setFontSize(int newFontSize);
	void setBold(bool newValue);
	void setItalics(bool newValue);
	void release();

	virtual ~CFont();
private:
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	ID3DXFont* m_pFont;
	int fontSize;
	bool bold;
	bool italisized; //in italics!!!
	LPCTSTR fontType;
};

#endif