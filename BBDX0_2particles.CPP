#include <d3dx9.h>
// Direct X creates max and min macros which interfere with standard C++ numeric_limits::max() etc.
#undef max
#undef min
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <ctime>
#include <vector>
#include "WWindow.h"
#include "WinApplication.h"
#include "CFont.h"
#include "CGuiObject.h"
#include "CMouse.h"
using namespace std;
//#include "../include/vectors.h"
#include "Particle Class.h"
#define BOUNDARIES 0
/************************************************************
*GLOBALS
*************************************************************/
WWindow dxWindow; //directX window including D3D device pointers
DWORD programStartTime;
CFont* m_Font;
CMouse mouse;
float AngleX = 0; //used to rotate the creature
float AngleY = 0; //used to rotate the creature
int frameCount = 0; 
int tempXLoc = 0;
int frameRate = 0;
bool paused = true;

const double timestep = 0.01;
double Time = 0.0;
double dt = timestep; //flexible parameter
double t_nextCol;
const double xedge=600, yedge=400;
int nextevent, nextparticle;
int walltot=0, colltot=0;




CGuiObject* testParticle;

double guiTime = 0;

/************************************************************
*DECLARATION OF ALL METHODS
************************************************************/
//The windows message handler
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT initialiseSystem();
HRESULT Render();
void setup3DCamera();
void setup2DCamera();
void drawGUI();
void drawText();
void setupPerspective();
void setupCamera();
void setupRotation();
void CleanUp();
void drawText();
void drawParticles();
void drawLines(double, double, double, double);
void SystemLoop();

void moveParticles();
void wallTest(particle&);
void collide(particle&, particle&);
double separation(particle, particle);
double dotprod(vector2d<double>, vector2d<double>);
double coltime(particle, particle);
double wallTime(particle, int&);
void nextTime(particle&, particle);
void wallCollide(particle&, int);

//particle movingParticle [2];

particle movingParticle(	60,   200,  600,  400  ,  20, 1);
particle movingParticle2(	119,  50,   -300, 100  ,   30, 4);

const double radius1=movingParticle.getrad(), radius2=movingParticle2.getrad();


/**********************************************************
 * Application entry point
 **********************************************************/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	/*
	movingParticle[0].setx(60);
	movingParticle[0].sety(200);
	movingParticle[0].setvelx(600);
	movingParticle[0].setvely(400);
	movingParticle[0].setrad(20);
	movingParticle[0].setmass(1);

	movingParticle[1].setx(119);
	movingParticle[1].sety(50);
	movingParticle[1].setvelx(-300);
	movingParticle[1].setvely(100);
	movingParticle[1].setrad(30);
	movingParticle[1].setmass(4);
	*/

	char *ClsNme = "DX Class";
	char *WndNme = "NanoThermodynamics";

	// Initialize the application class
	WApplication WinApp(hInst, ClsNme, WinProc);
	WinApp.Register();

	//Create the directX window
	dxWindow.Create(hInst, ClsNme, WndNme, NULL,  
				WS_OVERLAPPEDWINDOW, NULL, 
				50, 50, 800, 600);
	// Display the main winow
	dxWindow.Show();

	//if directX is successfully setup
	if(SUCCEEDED(dxWindow.InitialiseD3D(dxWindow.getHandle())))
	{
		if(SUCCEEDED(initialiseSystem())) //system is intialised if the window loads successfully
		{
			// Start running: Enter the loop
			SystemLoop();
		}
		else
		{
			MessageBox(NULL, "System Initialisation Failed", "LoadMesh()", NULL);
		}
		
	}
	else
	{
		MessageBox(NULL, "DirectX Initialisation Failed", "LoadMesh()", NULL);
	}
	
       
	dxWindow.CleanUp();

    UnregisterClass(ClsNme, hInst);
    
    return 0;
}

/**********************************************************
 * Main Loop
 **********************************************************/
void SystemLoop()
{   
	//Enter the loop
    MSG msg; 
    BOOL fMessage;



	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
    
    while(msg.message != WM_QUIT)
    {
        fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        if(fMessage)
        {
            //Process message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else //No message to process, so render the current scene
        {
            
			moveParticles(); //particle positions updated
			
			Render(); //render particles in window
			DWORD frameStartTime = GetTickCount();
			//if(Time > 7 && Time < 8){paused=true;}
			//increment frame counter
			frameCount++;

			//limit game fps to a max of 30
			while(GetTickCount() - frameStartTime < (DWORD)frameRate)
			{}
        }
    }
}

/**********************************************************
 * Basic Physics 
 **********************************************************/
void moveParticles()
{
	if ( !paused )
	{
		//FIRST run the prog and watch the confusing error. Then fix it.
		//NEXT THING TO DO is work out graphical glitch with diff sized particles.
		// and then generalise to multiple particles

//		t_nextCol = coltime(movingParticle, movingParticle2);
		nextTime(movingParticle, movingParticle2);
		nextTime(movingParticle2, movingParticle);
		if(movingParticle.gett_next()<=movingParticle2.gett_next() )
		{
			t_nextCol=movingParticle.gett_next();
			nextevent=movingParticle.getnextevent();
			nextparticle=1;
		}
		else
		{
			t_nextCol=movingParticle2.gett_next();
			nextevent=movingParticle2.getnextevent();
			nextparticle=2;
		}
				
		if(t_nextCol<timestep)
		{
			dt=t_nextCol;
		}
		
		else
		{
			dt=timestep;
		}
		Time += dt;
		movingParticle.setx(movingParticle.getx() + movingParticle.getvelx()*dt);
		movingParticle.sety(movingParticle.gety() + movingParticle.getvely()*dt );

		movingParticle2.setx(movingParticle2.getx() + movingParticle2.getvelx()*dt );
		movingParticle2.sety(movingParticle2.gety() + movingParticle2.getvely()*dt );

		if(t_nextCol<timestep)
		{
			if(nextevent<0)
			{
				if(nextparticle==1)
				{
					wallCollide(movingParticle, nextevent);
				}
				if(nextparticle==2)
				{
					wallCollide(movingParticle2,nextevent);
				}
			}
			else
			{
				collide(movingParticle, movingParticle2);
			}
		}

//		wallTest(movingParticle);
//		wallTest(movingParticle2);
		
		/*if(separation(movingParticle, movingParticle2)<(movingParticle.getrad()+movingParticle2.getrad() ))
		{
			collide(movingParticle, movingParticle2);
		}*/
		/*
		if(dt!=timestep)
		{
			collide(movingParticle, movingParticle2);
		}
		*/
		
		
	}			
}

/**********************************************************
 * Initialisation of System
 **********************************************************/
HRESULT initialiseSystem()
{
//	double radius = 10;
	double radius = movingParticle.getrad();
	std::ostringstream draws;
//	movingParticle.setrad(radius);
	m_Font = new CFont(dxWindow.g_pD3DDevice, "Verdana", 12, false, false, false);

    draws << "Particle test";
	testParticle = new CGuiObject(dxWindow.g_pD3DDevice, 
				(char*)(draws.str().c_str()),
				(int)radius*2, (int)radius*2,
				dxWindow.getScreenWidth(), dxWindow.getScreenHeight(),
				0, 0);
	draws.str("");
//	if(FAILED(testParticle->setImage("angus.png", D3DCOLOR_XRGB(0,0,0))))
	if(FAILED(testParticle->setImage("particle.bmp", D3DCOLOR_XRGB(0,0,0))))
	{
		MessageBox(NULL, "setImage1", "initialiseSystem", NULL);
		return E_FAIL;
	}

	//initialise gui particle objects

		int ht = dxWindow.getScreenHeight();
		draws << "Particle ";
		CGuiObject* Ob = new CGuiObject(dxWindow.g_pD3DDevice, 
				(char*)(draws.str().c_str()),
				(int)movingParticle.getrad()*2, (int)movingParticle.getrad()*2,
				dxWindow.getScreenWidth(), dxWindow.getScreenHeight(),
				(int)movingParticle.getx(), ht-(int)movingParticle.gety());
		draws.str("");
//		if(FAILED(Ob->setImage("angus.png", D3DCOLOR_XRGB(0,0,0))))
		if(FAILED(Ob->setImage("particle.bmp", D3DCOLOR_XRGB(0,0,0))))
		{
			MessageBox(NULL, "setImage2", "initialiseSystem", NULL);
			return E_FAIL;
		}
		movingParticle.setGuiObject(Ob);

		draws << "Particle2 ";
		CGuiObject* Ob2 = new CGuiObject(dxWindow.g_pD3DDevice, 
				(char*)(draws.str().c_str()),
				(int)movingParticle2.getrad()*2, (int)movingParticle2.getrad()*2,
				dxWindow.getScreenWidth(), dxWindow.getScreenHeight(),
				(int)movingParticle2.getx(), ht - (int)movingParticle2.gety());
		draws.str("");






//		if(FAILED(Ob2->setImage("angus.png", D3DCOLOR_XRGB(0,0,0))))
		if(FAILED(Ob2->setImage("particle.bmp", D3DCOLOR_XRGB(0,0,0))))
		{
			MessageBox(NULL, "setImage2", "initialiseSystem", NULL);
			return E_FAIL;
		}
		movingParticle.setGuiObject(Ob);
		movingParticle2.setGuiObject(Ob2);

	//program started, so record time
	programStartTime = timeGetTime();

	return S_OK;
}

/**********************************************************
 * The windows message handler
 **********************************************************/
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            CleanUp();
			PostQuitMessage(0);
            return 0;
        break;
		case WM_SIZING:
			//MessageBox(dxWindow.getHandle(), "size",0,0);
			RECT screenPos;
			GetWindowRect(dxWindow.getHandle(), &screenPos);
			dxWindow.setScreenWidth(screenPos.right - screenPos.left);
			dxWindow.setScreenHeight(screenPos.bottom - screenPos.top);
		break;
		case WM_LBUTTONDOWN:			
		break;
		case WM_LBUTTONUP:
		break;
		case WM_RBUTTONDOWN:
			MessageBox(dxWindow.getHandle(), "Right Mouse Button Clicked", "Mouse Event", 0);
		break;
        case WM_KEYUP: 
            switch (wParam)
            { 
                case VK_ESCAPE:
                    //User has pressed the escape key, so quit
                    DestroyWindow(hWnd);
                    return 0;
                break;
				case VK_UP:

					frameRate = frameRate + 50;
					//movingParticle.setvely(100);
					/*
					dt = coltime(movingParticle, movingParticle2);
					movingParticle.setx(movingParticle.getx() + movingParticle.getvelx()*dt);
					movingParticle.sety(movingParticle.gety() + movingParticle.getvely()*dt );

					movingParticle2.setx(movingParticle2.getx() + movingParticle2.getvelx()*dt );
					movingParticle2.sety(movingParticle2.gety() + movingParticle2.getvely()*dt );
					*/

					return 0;
				break;
				case VK_DOWN:
					if(frameRate!=0)
					{
						frameRate = frameRate - 50;
					}
					return 0;
				break;
				case VK_LEFT:
					paused = true;
					return 0;
				break;
				case VK_RIGHT:
					paused = false;
					return 0;
				break;
            } 
        break;

    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**********************************************************
 * GUI related stuff
 **********************************************************/
HRESULT Render()
{
	if(dxWindow.g_pD3DDevice == NULL)
    {
        return E_FAIL;	// Don't render if no device present to render
    }

    /* Clear the backbuffer to a blue color */
    dxWindow.g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0L);
    
    dxWindow.g_pD3DDevice->BeginScene(); // Initialise scene

	//DO ALL THE RENDERING...
	//setup3DCamera();	

	//DETECT COLLISION TIMES HERE

	setup2DCamera();
		
	drawText();
	drawLines(xedge,0,xedge,yedge);
	drawLines(0,yedge,xedge,yedge);
	drawParticles();
	/************** END RENDERING ***************/

	//Finish her off!
	dxWindow.g_pD3DDevice->EndScene();	// Finalise scene
    
    //Filp the back and front buffers so that whatever has been rendered on the back buffer
    //will now be visible on screen (front buffer).
    dxWindow.g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

void drawText()
{
	D3DCOLOR fontColor = D3DCOLOR_XRGB(0,0,0);  
	DWORD dwDuration = (timeGetTime() - programStartTime) / 1000;
	std::ostringstream draws;

	//put stats including FPS into buffer variable   
    if(dwDuration > 0)
    {
		draws << "Duration: " << dwDuration << " seconds. Frames: " << frameCount << ", FPS: " << (frameCount / dwDuration) << '\n'; 
    }
    else
    {
		draws << "Calculating...\n";
    }
    
	
	draws << "Frame Rate: " << frameRate << "\n";
	draws << "Total KE: " << movingParticle.getKE() + movingParticle2.getKE();
	draws << "Total Px : " << movingParticle.getpx() + movingParticle2.getpx();
	draws << "Total Py : " << movingParticle.getpy() + movingParticle2.getpy() << "\n";
	draws << "t_nextCol : " << t_nextCol << "\n";
	draws << "wall hits : " << walltot << "    colls : " << colltot << "\n";
	draws << "px1 : " << movingParticle.getpx() << "   py1 : " << movingParticle.getpy() << "\n";
	
	m_Font->DrawText( draws.str().c_str(), 0, 0, fontColor);
	draws.str("");
	{
		
		m_Font->DrawText( draws.str().c_str(), 300, 10, fontColor);
		draws.str("");
		draws << "Time:  " << Time;
		m_Font->DrawText( draws.str().c_str(), 500, 10, fontColor);
		draws.str("");
	}
			
	//draw info on particles

		POINT mouseLoc = mouse.getMouseLocation(dxWindow.getHandle());
		if(movingParticle.getGuiObject()->mouseIsOver(mouseLoc.x, mouseLoc.y))
		{
			draws << "Position: " << movingParticle.getx() << ' ' 
				                  << movingParticle.gety();
			m_Font->DrawText( draws.str().c_str(), 200, 390, fontColor);
			draws.str("");
		}

}
/*
//duplicated below, this one has the incorrect y-axis
void drawParticles()
{
			movingParticle.getGuiObject()->render( movingParticle.getpos().x(), movingParticle.getpos().y());//note how the particle starts offset at y=50
			movingParticle2.getGuiObject()->render( movingParticle2.getpos().x(), movingParticle2.getpos().y());//note how the particle starts offset at y=50


	//testParticle->render(tempXLoc, 20);
	tempXLoc = (tempXLoc + 5)%dxWindow.getScreenWidth();
}*/
void drawParticles()
{
	int ht = dxWindow.getScreenHeight();		//THIS IS WHERE I FIXED THE WEIRD GRAPHICS THING
	movingParticle.getGuiObject()->render( movingParticle.getpos().x()-radius1, ht - movingParticle.getpos().y()-radius1);
	movingParticle2.getGuiObject()->render( movingParticle2.getpos().x()-radius2, ht - movingParticle2.getpos().y()-radius2);


	//testParticle->render(tempXLoc, 20);
	//tempXLoc = (tempXLoc + 5)%dxWindow.getScreenWidth();
}
void drawLines(double x0, double y0, double x1, double y1)
{
	struct point_vertex{
		float x, y, z, rhw;
		DWORD colour;
	};
	point_vertex lineends[2];
	lineends[0].x = (float)x0-(float)(dxWindow.getScreenWidth()/2.0);
	lineends[0].y = (float)y0-(float)(dxWindow.getScreenHeight()/2.0);
	lineends[0].z = 1.0f;
	lineends[0].rhw = 1.0f;
	lineends[0].colour = D3DCOLOR_XRGB(1,1,1);
    lineends[1].x = (float)x1-(float)(dxWindow.getScreenWidth()/2.0);
	lineends[1].y = (float)y1-(float)(dxWindow.getScreenHeight()/2.0);
	lineends[1].z = 1.0f;
	lineends[1].rhw = 1.0f;
	lineends[1].colour = D3DCOLOR_XRGB(1,1,1);
    dxWindow.g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, lineends, sizeof(point_vertex) );
}

void setup3DCamera()
{
	/************** Camera/Rotation/Perspective **************/
    setupRotation();
    setupCamera();
    setupPerspective();
}

void setup2DCamera()
{
	/*
	To setup the camera for 2D objects we need to change the projection matrix from a perspective one 
	used in 3D to an orthogonal projection.
	We also set the world and view matrices to an identity matrix and disable the z-buffer and lighting.
	We disable the z-buffer so that everything that is rendered from then on will appear on top of 
	objects that have already been rendered (3D objects).
	*/

	D3DXMATRIX matOrtho;
    D3DXMATRIX matIdentity;
    
    //Setup the orthogonal projection matrix and the default world/view matrix
    D3DXMatrixOrthoLH(&matOrtho, (float)dxWindow.getScreenWidth(), (float)dxWindow.getScreenHeight(), 
					0.0f, 1.0f);
    D3DXMatrixIdentity(&matIdentity);

    dxWindow.g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
    dxWindow.g_pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
    dxWindow.g_pD3DDevice->SetTransform(D3DTS_VIEW, &matIdentity);

    //Make sure that the z-buffer and lighting are disabled
    dxWindow.g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    dxWindow.g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void setupRotation()
{
	float RadiansX;
	float RadiansY;
	
	AngleX = AngleX + 5;
	
	AngleX = (AngleX >= 360 ? AngleX - 360 : AngleX);
	AngleY = (AngleY >= 360 ? AngleY - 360 : AngleY);
	
	RadiansX = AngleX * (D3DX_PI / 180);
    RadiansY = AngleY * (D3DX_PI / 180);

	//Here we will rotate our world around the x, y and z axis.
    D3DXMATRIX matWorld, matWorldX, matWorldY, matWorldZ;
    
    //Create the transformation matrices
    D3DXMatrixRotationX(&matWorldX, RadiansY);
    D3DXMatrixRotationY(&matWorldY, RadiansX); 
    D3DXMatrixRotationZ(&matWorldZ, 0.0f); 

    //Combine the transformations by multiplying them together
    D3DXMatrixMultiply(&matWorld, &matWorldX, &matWorldY);
    D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldZ);

    //Apply the tansformation
    dxWindow.g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
}


void setupCamera()
{
    //Here we will setup the camera.
    //The camera has three settings: "Camera Position", "Look at Position" and "Up Direction"
    //We have set the following:
    //Camera Position: (0, 0, -30)
    //Look at Position: (0, 0, 0)
    //Up direction: Y-Axis.
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(-20.0f, -0.0f,-0.0f), //Camera Position
                                 &D3DXVECTOR3(10.0f, 0.0f, -5.0f), //Look At Position
                                 &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); //Up Direction
    dxWindow.g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void setupPerspective()
{
    //Here we specify the field of view, aspect ration and near and far clipping planes.
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 500.0f);
    dxWindow.g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CleanUp()
{
	dxWindow.CleanUp();
	
	m_Font->release();
	delete m_Font;

}
void wallTest(particle& p1)
{
	double x=p1.getx(), y=p1.gety();
	double radius=p1.getrad();
	if(x>(xedge-radius) || x<(20+radius))
	{
		p1.setvelx(-p1.getvelx() );
	}
	if(y>(yedge-radius) || y<(20+radius))
	{
		p1.setvely(-p1.getvely() );
	}
}
void wallCollide(particle& p1, int nextwall)
{
	double x=p1.getx(), y=p1.gety();
	double radius=p1.getrad();
	switch (nextwall)
	{
	case -1:
		p1.setvely(-p1.getvely() );
		break;
	case -2:
		p1.setvelx(-p1.getvelx() );
		break;
	case -3:
		p1.setvely(-p1.getvely() );
		break;
	case -4:
		p1.setvelx(-p1.getvelx() );
		break;
	}
	walltot++;
}

void collide(particle& p1, particle& p2)
{
	double m1=p1.getmass(), m2=p2.getmass();
	vector2d<double> P1( p1.getpx(),p1.getpy() );
	vector2d<double> P2( p2.getpx(),p2.getpy() );

	vector2d<double> newP1=P1, newP2;
	
	vector2d<double> r21_hat = p2.getpos()-p1.getpos();
	r21_hat = r21_hat / r21_hat.mag();
	vector2d<double> n21_hat(-r21_hat.y() , r21_hat.x() );

	//twovector p1vec( p1.getPx() , p1.getPy() ), p2vec(p2.getPx(), p2.getPy() ), newp1, newp2;
	//particle newp1=p1, newp2=p2;

//	newP1 = (r21_hat * dotprod( ( ( P2 * 2.0 *  m2 ) + P1*( m2 - m1 ) ), r21_hat)/(m1 + m2 ) ) + (n21_hat * dotprod(P1, n21_hat));
//	newP2 = (r21_hat * dotprod(( ( P1 * 2.0 *  m1 ) + P2*( m1 - m2 ) )/(m1 + m2 ),r21_hat)) + (n21_hat * dotprod(P2, n21_hat));

	newP1 = (r21_hat * dotprod( ( ( P2 * 2.0 *  m1 ) + P1*( m1 - m2 ) ), r21_hat)/(m1 + m2 ) ) + (n21_hat * dotprod(P1, n21_hat));
	newP2 = (r21_hat * dotprod(( ( P1 * 2.0 *  m2 ) + P2*( m2 - m1 ) )/(m1 + m2 ),r21_hat)) + (n21_hat * dotprod(P2, n21_hat));



	p1.setp(newP1);
	p2.setp(newP2);

	colltot++;
}
double separation(particle p1, particle p2)
{
	vector2d<double> r1 = p1.getpos();
	vector2d<double> r2 = p2.getpos();
	return (r2-r1).mag();
}
double coltime(particle p1, particle p2)
{
	vector2d<double> deltavel, deltapos;
	double R1=p1.getrad(), R2=p2.getrad();
	deltavel=p1.getvel()-p2.getvel();
	deltapos=p1.getpos()-p2.getpos();
	double a=dotprod(deltavel,deltavel);
	double b=2*dotprod(deltavel,deltapos);
	double c=( dotprod(deltapos,deltapos) ) - ((R1+R2)*(R1+R2));

//	double t_p=( (-b)+(sqrt( (b*b)-(4*a*c) )) )/(2*a);
	double discriminant= (b*b)-(4*a*c);
	double t;
	if(discriminant>0)
	{
		t=( (-b)-(sqrt( discriminant )) )/(2*a);
	}
/*	else if(discriminant==0)//commented because without friction, they won't interact in the brushing case anyway
	{ // and if you uncomment this, it thinks they collide if your velocity is a multiple of 12.5 (and one brushes the other)
		t= (-b)/(2*a); //so watch out for that if you want to introduce friction.
	}*/
	else
	{
		t=100000;
	}

	if(t<0)
	{
		t=100000;
	}


	return t;
}
double wallTime(particle p1, int& nextwall)
{
	double tx=1000, ty=1000, t, R=p1.getrad();
	int nextx=0, nexty=0;
	double rx = p1.getx(), ry = p1.gety();
	double vx=p1.getvelx();
	double vy=p1.getvely();
	if(vx>0)
	{
		tx=(xedge-R-rx)/vx;
		nextx=-2;
	}
	else if(vx<0)
	{
		tx=(rx-R)/(-vx);
		nextx=-4;
	}


	if(vy>0)
	{
		ty=(yedge-R-ry)/vy;
		nexty=-1;
	}
	else if(vy<0)
	{
		ty=(ry-R)/(-vy);
		nexty=-3;
	}


	if(tx<=ty)
	{
		t=tx;
		nextwall=nextx;
	}
	else if(ty<tx)
	{
		t=ty;
		nextwall=nexty;
	}

	return t;
}
void nextTime(particle& p1, particle p2)
{
	int nextwall=0, nextparticle=1;
	double t_wall=wallTime(p1, nextwall);
	double t_coll=coltime(p1,p2);
	if(t_wall<=t_coll)//this might break if they are simultaneous?
	{
		p1.sett_next(t_wall);
		p1.setnextevent(nextwall);
	}
	else
	{
		p1.sett_next(t_coll);
		p1.setnextevent(nextparticle);
	}
		
}
double dotprod(vector2d<double> v1, vector2d<double> v2)
{
	double v1x=v1.x(), v1y=v1.y(), v2x=v2.x(), v2y=v2.y();
	return ( (v1x*v2x)+(v1y*v2y) );
}