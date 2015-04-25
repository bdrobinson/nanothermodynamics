#include "CFont.h"

CFont::CFont()
{

}

CFont::CFont(LPDIRECT3DDEVICE9 pD3DDevice, LPCTSTR pFontFace, int nHeight, bool fBold, bool fItalic, bool fUnderlined)
{
    //HFONT hFont;

    m_pD3DDevice = pD3DDevice;
	fontSize = nHeight;
	bold = fBold;
	italisized = fItalic;
	fontType = pFontFace;
    
    int nWeight = FW_NORMAL;

    if(bold)
    {
        nWeight = FW_BOLD;
    }

    //hFont = CreateFont(nHeight, 0, 0, 0, nWeight, dwItalic, dwUnderlined, 0, ANSI_CHARSET, 0, 0, 0, 0, pFontFace);
    
    D3DXCreateFont( m_pD3DDevice, fontSize, 0, FW_BOLD, 0, italisized, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			fontType, &m_pFont );
	
	//D3DXCreateFont(m_pD3DDevice, hFont, &m_pFont);

}

CFont::~CFont()
{
	//release();
}

void CFont::setFontType(LPCTSTR newFontType)
{
	fontType = newFontType;

	//re-initialise the font object
	D3DXCreateFont( m_pD3DDevice, fontSize, 0, FW_BOLD, 0, italisized, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			fontType, &m_pFont );
}

void CFont::setFontSize(int newFontSize)
{
	fontSize = newFontSize;

	//re-initialise the font object
	D3DXCreateFont( m_pD3DDevice, fontSize, 0, FW_BOLD, 0, italisized, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			fontType, &m_pFont );
}

void CFont::setBold(bool newValue)
{
	bold = newValue;

	//re-initialise the font object
	D3DXCreateFont( m_pD3DDevice, fontSize, 0, FW_BOLD, 0, italisized, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			fontType, &m_pFont );
}

void CFont::setItalics(bool newValue)
{
	italisized = newValue;

	//re-initialise the font object
	D3DXCreateFont( m_pD3DDevice, fontSize, 0, FW_BOLD, 0, italisized, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			fontType, &m_pFont );
}

void CFont::DrawText(LPCTSTR pText, int x, int y, D3DCOLOR rgbFontColour)
{
    //re-initialise the font object
//	D3DXCreateFont( m_pD3DDevice, fontSize, 0, FW_BOLD, 0, italisized, DEFAULT_CHARSET, 
//			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
//			fontType, &m_pFont );

	RECT Rect = {x,y,0,0};

    m_pFont->DrawText(NULL, pText, -1, &Rect, DT_CALCRECT, 0);
	m_pFont->DrawText(NULL, pText, -1, &Rect, 0, rgbFontColour);

	//clear font object
//	release();
}

void CFont::release()
{
	m_pFont->OnLostDevice();
	m_pFont->Release();

	//SAFE_RELEASE(m_pFont);
}