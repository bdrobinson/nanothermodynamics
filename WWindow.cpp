#include "WWindow.h"

//---------------------------------------------------------------------------
WWindow::WWindow()
{
	// If we declare a window class with a default constructor,
	// we need to reset the window to a nothing
	handle = NULL;
}
//---------------------------------------------------------------------------
HWND WWindow::Create(HINSTANCE hinst,
    				 LPCTSTR clsname,
	    			 LPCTSTR wndname,
	  	    		 HWND parent,
			    	 DWORD dStyle,
					 DWORD dXStyle,
				     int x,
     				 int y,
	     			 int width,
		    		 int height)
{
	// When call the Create() method, we can use it to create a new window
	
	handle = CreateWindowEx(dXStyle, clsname, wndname, dStyle, x, y, width,
		                 height, parent, NULL, hinst, NULL);

	// We hope everything went alright and the window was created
	if( handle != NULL )
	{
		setHandle(handle);
		return handle;
	}
	// If something went wrong, for example if the window could not
	// be created, return a "nothing" window
	return NULL;
}
//---------------------------------------------------------------------------
BOOL WWindow::Show(int dCmdShow)
{
	// We will display the main window as a regular object and update it
	if(	ShowWindow(handle, dCmdShow) && UpdateWindow(handle) )
		return TRUE;
	return FALSE;
}

//---------------------------------------------------------------------------
/********************************************************************************
 * Initialise Direct3D
 ********************************************************************************/

HRESULT WWindow::InitialiseD3D(HWND hWnd)
{
	/* Create main Direct3D object */
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(g_pD3D == NULL)
    {
        return E_FAIL;
    }
	
	/*** Set Window handle ***/
	handle = hWnd;

    /* Get the current display mode */
    D3DDISPLAYMODE d3ddm;
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }


    //Create a structure to hold the settings for our device
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    /* Fill the structure. 
	 * We want our program to be windowed, and set the back buffer to a format
	 * that matches our current display mode
	 */
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_COPY; //_VSYNC;
    d3dpp.BackBufferFormat = d3ddm.Format;

	//set global variables about size of the window
	

    // Create a Direct3D device.
    if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, handle, 
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
    {
        return E_FAIL;
    }
	
	screenWidth = d3dpp.BackBufferWidth;
	screenHeight = d3dpp.BackBufferHeight;
	
	//Enable alpha blending so we can use transparent textures
	if(FAILED(g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE)))
	{
		return E_FAIL;
	}
	else
	{
		//Set how the texture should be blended (use alpha)
		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		
	}
    
    // Turn on back face culling. This is becuase we want to hide the back of our polygons
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); //D3DCULL_NONE

    // Turn off lighting becuase we are specifying that our vertices have colour
    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}

/*******************************************************
 * Clean up after exit
 *******************************************************/

void WWindow::CleanUp()
{
 	//Clean up D3D objects
	if(g_pD3DDevice != NULL)
    {
        g_pD3DDevice->Release();
        g_pD3DDevice = NULL;
    }

    if(g_pD3D != NULL)
    {
        g_pD3D->Release();
        g_pD3D = NULL;
    }

	SafeRelease(g_pD3DDevice);
    SafeRelease(g_pD3D);
}

HWND WWindow::getHandle()
{
	return handle;
}

void WWindow::setHandle(HWND newHandle)
{
	handle = newHandle;
}

int WWindow::getScreenWidth()
{
	return screenWidth;
}

int WWindow::getScreenHeight()
{
	return screenHeight;
}

void WWindow::setScreenWidth(int newWidth)
{
	screenWidth = newWidth;
}

void WWindow::setScreenHeight(int newHeight)
{
	screenHeight = newHeight;
}