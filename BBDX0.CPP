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

const double timestep = 0.001;
double Time = 0.0;
double dt = timestep; //flexible parameter
double t_nextCol;
//double ghost_time;
const double xedge=81, yedge=81;
int nextevent=0, particletocollide=0;
int walltot=0, colltot=0;




CGuiObject* testParticle;
ofstream spreadsheet;
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
//void drawText();
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
void nextTime(particle&, particle);

double wallTime(particle, int&);
void wallCollide(particle&, int);
void initialiseCollisions();

double wallTimeP(particle, int&);
void wallCollideP(particle&, int);
void initialiseCollisionsP();
void initialiseCrits();


void motion(double);
//void motion_ghost(double, int);
double nextTimeToIterate( int& , int&);

void updateCollisions(int);
void updateCollisionsP(int);

void minusDT(double);

void updateStragglers(int);
void updateStragglersP(int);

void updateCrit(int, int);
void updateCritBoundary(int, int);

void correctGhostParticle(int, int);

const int numberofparticles = 8;
particle movingParticle [numberofparticles];
//particle ghostParticle [numberofparticles];
particle ghostParticle [9][numberofparticles];//changed because the method destructively overwrites the array so we need 1 array per particle.

//ISSUE: need to change method used for pbc's. do in terms of distance from wall (in terms of largest radius in system?)
//fixed the ghost particle iteration thing too, still didn't fix it.
particle ghostParticle1 [numberofparticles];
particle ghostParticle2 [numberofparticles];
particle ghostParticle3 [numberofparticles];
particle ghostParticle4 [numberofparticles];
double radius [numberofparticles];
double KEtot=0;
double Ptot=0;
const int wallcase=1;
int isitaghost [numberofparticles];
double crit_length;
double dummy;

/**********************************************************
 * Application entry point
 **********************************************************/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{

	double angle=(2*3.141/(1.0*numberofparticles) );
	for(int i=0;i<numberofparticles;i++)
	{
		
		movingParticle[i].setpos(xedge/2.0,yedge/2.0);
		movingParticle[i].setvel(1000+(100*i),i*angle,'t');
		movingParticle[i].setrad(13);
		movingParticle[i].setmass(1);
	}

	
	//corners problem
/*	
	movingParticle[1].setpos((xedge/2.0),yedge/2.0);
	movingParticle[1].setvel(500,500);
	movingParticle[1].setrad(20);
	movingParticle[1].setmass(1);
	
	
	movingParticle[0].setpos( (xedge/2.0), (yedge/2.0));
	movingParticle[0].setvel(-500,-500);
	movingParticle[0].setrad(20);
	movingParticle[0].setmass(1);
	*/

	//poking over problem
	/*
	movingParticle[1].setpos(xedge/2.0,0);
	movingParticle[1].setvel(0,1);
	movingParticle[1].setrad(20);
	movingParticle[1].setmass(1);
	
	
	movingParticle[0].setpos(20, yedge-10);
	movingParticle[0].setvel(500,0);
	movingParticle[0].setrad(20);
	movingParticle[0].setmass(1);

	*/
	
	for(int i=0;i<numberofparticles;i++)
	{
		radius[i]=movingParticle[i].getrad();
	}
	double max=0, max2=0;
	for(int i=0;i<numberofparticles;i++)
	{
		if(radius[i]>max)
		{
			max2=max;
			max=radius[i];
		}

	}
	crit_length=max;//MAYBE max+max2 ?
//	crit_length= 2*max;
//	initialiseCollisions();
	initialiseCrits();
	initialiseCollisionsP();


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
		t_nextCol = nextTimeToIterate(particletocollide, nextevent);//watch out! the args don't actually affect the result of the function, they are just being &'ed.

		if(t_nextCol<timestep)
		{
			dt=t_nextCol;
			motion(dt);
			minusDT(dt);
			if(nextevent<0)
			{
				if(nextevent<-8)//-8 is arbitrary - any number between -10 and -4 will do
				{
					updateCrit(particletocollide, nextevent);
					updateCollisionsP(particletocollide);
					updateStragglersP(particletocollide);
					
				}
				else
				{
					updateCritBoundary(particletocollide, nextevent);				
					wallCollideP(movingParticle[particletocollide], nextevent);
					updateCollisionsP(particletocollide);
					updateStragglersP(particletocollide);
				}

			}
			else if(nextevent>=0)
			{
				if(movingParticle[particletocollide].getnextevent0()==0)//if collision is normal (non ghosty)
				{
					collide(movingParticle[particletocollide],movingParticle[nextevent]);
					updateCollisionsP(particletocollide);
					updateCollisionsP(nextevent);
					updateStragglersP(particletocollide);
					updateStragglersP(nextevent);
				}
				else if(movingParticle[particletocollide].getnextevent0()>0 || movingParticle[nextevent].getnextevent0()>0)
				{
					int boundary=movingParticle[particletocollide].getnextevent0();
					dummy=boundary;
					correctGhostParticle(boundary, nextevent);

					collide(movingParticle[particletocollide],ghostParticle[boundary][nextevent]);
					movingParticle[nextevent].setvel(ghostParticle[boundary][nextevent].getvel());


					updateCollisionsP(particletocollide);
					updateCollisionsP(nextevent);
					updateStragglersP(particletocollide);
					updateStragglersP(nextevent);
				}
				/*if(movingParticle[nextevent].getnextevent0()==1)
				{
					int boundary=movingParticle[particletocollide].getnextevent1();
					correctGhostParticle(boundary, nextevent);

					collide(movingParticle[particletocollide],ghostParticle[boundary][nextevent]);
					movingParticle[nextevent].setvel(ghostParticle[boundary][nextevent].getvel());


					updateCollisionsP(particletocollide);
					updateCollisionsP(nextevent);
					updateStragglersP(particletocollide);
					updateStragglersP(nextevent);
				}*/
			}



			dt=timestep;//corrected 12/12/13
		}
		else
		{
			motion(dt);
			minusDT(dt);
		}
		Time += dt;



/*
		t_nextCol = nextTimeToIterate(particletocollide, nextevent);//watch out! the args don't actually affect the result of the function, they are just being &'ed.
		
		
		if(t_nextCol<timestep)
		{
			dt=t_nextCol;
			motion(dt);
			minusDT(dt);
			if(nextevent<0)
			{
				wallCollideP(movingParticle[particletocollide], nextevent);
				updateCollisionsP(particletocollide);
				updateStragglersP(particletocollide);
			}
			else if(nextevent>=0)
			{
				if(isitaghost[particletocollide]==1)
				{

					collide(movingParticle[particletocollide],ghostParticle[particletocollide][nextevent]);
					movingParticle[nextevent].setvel(ghostParticle[particletocollide][nextevent].getvel() );
					isitaghost[particletocollide] = 0;
				}
				else if(isitaghost[nextevent]==1)
				{

					collide(movingParticle[particletocollide],ghostParticle[particletocollide][nextevent]);
					movingParticle[nextevent].setvel(ghostParticle[particletocollide][nextevent].getvel() );
					isitaghost[particletocollide] = 0;
				}
				else
				{
					collide(movingParticle[particletocollide],movingParticle[nextevent]);
				}
				updateCollisionsP(particletocollide);
				updateCollisionsP(nextevent);
				updateStragglersP(particletocollide);
				updateStragglersP(nextevent);

			}


//			initialiseCollisions();
			dt=timestep;
		}
		else
		{
			motion(dt);
			
//			initialiseCollisions();
			minusDT(dt);

		}
		Time += dt;
	}

	*/
		}


/*		t_nextCol = nextTimeToIterate(particletocollide, nextevent);//watch out! the args don't actually affect the result of the function, they are just being &'ed.
		if(t_nextCol<timestep)
		{
			dt=t_nextCol;
			motion(dt);
			minusDT(dt);
			if(nextevent<0)
			{
				wallCollide(movingParticle[particletocollide], nextevent);
				updateCollisions(particletocollide);
				updateStragglers(particletocollide);
			}
			else if(nextevent>=0)
			{
				collide(movingParticle[particletocollide],movingParticle[nextevent]);
				updateCollisions(particletocollide);
				updateCollisions(nextevent);
				updateStragglers(particletocollide);
				updateStragglers(nextevent);

			}


//			initialiseCollisions();
			dt=timestep;
		}
		else
		{
			motion(dt);
//			initialiseCollisions();
			minusDT(dt);

		}
		Time += dt;
	}		*/
/*
	for (int exceltime; exceltime <20; exceltime++)
	{
	if(Time>exceltime)
	{
		spreadsheet[exceltime].open("boltz.xls");
		for(int j=0;j<numberofparticles;j++)
		{
			spreadsheet[exceltime]<<j << "\t" << movingParticle[j].getvel().mag()<<"\n";
		}
		spreadsheet[exceltime].close()

	}*/
}

/**********************************************************
 * Initialisation of System
 **********************************************************/
HRESULT initialiseSystem()
{
//	double radius = 10;
	double radius = movingParticle[0].getrad();
	std::ostringstream draws;
//	movingParticle[0].setrad(radius);
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

		CGuiObject* Ob [numberofparticles];
		for(int i=0;i<numberofparticles;i++)
		{

			Ob[i] = new CGuiObject(dxWindow.g_pD3DDevice, 
					(char*)(draws.str().c_str()),
					(int)movingParticle[i].getrad()*2, (int)movingParticle[i].getrad()*2,
					dxWindow.getScreenWidth(), dxWindow.getScreenHeight(),
					(int)movingParticle[i].getx(), ht - (int)movingParticle[i].gety());
//			if(FAILED(Ob[i]->setImage("angus.png", D3DCOLOR_XRGB(0,0,0))))
			if( FAILED(Ob[i]->setImage("particle.bmp", D3DCOLOR_XRGB(0,0,0))))
			{
					MessageBox(NULL, "setImage2", "initialiseSystem", NULL);
					return E_FAIL;
			}
			movingParticle[i].setGuiObject(Ob[i]);
		}

//		draws.str("");




	/*	movingParticle[0].setGuiObject(Ob);
		movingParticle[1].setGuiObject(Ob2);
		movingParticle[2].setGuiObject(Ob3);*/

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
					//movingParticle[0].setvely(100);
					/*
					dt = coltime(movingParticle[0], movingParticle[1]);
					movingParticle[0].setx(movingParticle[0].getx() + movingParticle[0].getvelx()*dt);
					movingParticle[0].sety(movingParticle[0].gety() + movingParticle[0].getvely()*dt );

					movingParticle[1].setx(movingParticle[1].getx() + movingParticle[1].getvelx()*dt );
					movingParticle[1].sety(movingParticle[1].gety() + movingParticle[1].getvely()*dt );
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
	drawLines(0,crit_length,xedge,crit_length);
	drawLines(0,yedge-crit_length,xedge,yedge-crit_length);
	drawLines(crit_length,0,crit_length,yedge);
	drawLines(xedge-crit_length,0,xedge-crit_length,yedge);
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
	KEtot=0;
	Ptot=0;
    for(int i=0;i<numberofparticles;i++)
	{
		KEtot=KEtot + movingParticle[i].getKE();
		Ptot=Ptot + movingParticle[i].getpx();
	}
	
	draws << "Frame Rate: " << frameRate << "\n";
	draws << "Total KE: " << KEtot<<"\n";
	draws << "pxtot : " << Ptot << "\n";
//	draws << "P1 x : " << movingParticle[0].getpx() << "\t P1 y: " << movingParticle[0].getpy() << "\n";
//	draws << "\nP2 x : " << movingParticle[1].getpx() << "\t P2 y: " <<movingParticle[1].getpy() << "\n";
	draws << "t_nextCol : " << t_nextCol << "\n";
	draws << "wall hits : " << walltot << "    colls : " << colltot << "\n";
//	draws << "ghost_time : " << ghost_time << "\n";
//	draws << "isitaghost : " << isitaghost[0] << "\t" << isitaghost[1] << "\t" << isitaghost[2] << "\n";

	draws << "crits0 : " << movingParticle[0].getCrit1() << "\t" << movingParticle[0].getCrit2() << "\t" << movingParticle[0].getCrit3() << "\t" << movingParticle[0].getCrit4() << "\n";
	draws << "crits1 : " << movingParticle[1].getCrit1() << "\t" << movingParticle[1].getCrit2() << "\t" << movingParticle[1].getCrit3() << "\t" << movingParticle[1].getCrit4() << "\n";
	draws << "particletocollide : " << particletocollide << "       nextevent : " << nextevent << "\n";
	draws << "dummy : " << dummy << "\n";
	
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
		if(movingParticle[0].getGuiObject()->mouseIsOver(mouseLoc.x, mouseLoc.y))
		{
			draws << "Position: " << movingParticle[0].getx() << ' ' 
				                  << movingParticle[0].gety();
			m_Font->DrawText( draws.str().c_str(), 200, 390, fontColor);
			draws.str("");
		}

}
/*
//duplicated below, this one has the incorrect y-axis
void drawParticles()
{
			movingParticle[0].getGuiObject()->render( movingParticle[0].getpos().x(), movingParticle[0].getpos().y());//note how the particle starts offset at y=50
			movingParticle[1].getGuiObject()->render( movingParticle[1].getpos().x(), movingParticle[1].getpos().y());//note how the particle starts offset at y=50


	//testParticle->render(tempXLoc, 20);
	tempXLoc = (tempXLoc + 5)%dxWindow.getScreenWidth();
}*/
void drawParticles()
{
	int ht = dxWindow.getScreenHeight();		//THIS IS WHERE I FIXED THE WEIRD GRAPHICS THING
	
	for(int i=0;i<numberofparticles;i++)
	{
		movingParticle[i].getGuiObject()->render( movingParticle[i].getpos().x()-radius[i], ht - movingParticle[i].getpos().y()-radius[i]);
	}


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


//Functions to actually change momenta
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
void wallCollideP(particle& p1, int nextwall)
{
	double x=p1.getx(), y=p1.gety();
	double radius=p1.getrad();
	switch (nextwall)
	{
	case -1:
		p1.sety(0);
		break;
	case -2:
		p1.setx(0);
		break;
	case -3:
		p1.sety(yedge);
		break;
	case -4:
		p1.setx(xedge);
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

//Obselete function
/*
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
*/


//Functions to see when next collision is happening by analysing physics
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
		t=10;
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
double wallTimeP(particle p1, int& nextwall)
{
	double tx=1000, ty=1000, t, R=p1.getrad();
	int nextx=0, nexty=0;
	double rx = p1.getx(), ry = p1.gety();
	double vx=p1.getvelx();
	double vy=p1.getvely();
	int is_critical [5];
	is_critical[1] = p1.getCrit1();
	is_critical[2] = p1.getCrit2();
	is_critical[3] = p1.getCrit3();
	is_critical[4] = p1.getCrit4();

	if( is_critical[1]==1)//if it's above the crit line along the top
	{
		if(vy>0)//only event to worry about is crossing the periodic boundary
		{
			ty= (yedge-ry)/vy;
			nexty=-1;
		}
		else if(vy<0) //only event to worry about is leaving crit zone from below
		{
			ty = (yedge-crit_length-R-ry)/vy;
			nexty=-10;
		}
	}

	if( is_critical[3]==1)
	{
		if(vy<0)
		{
			ty= -(ry)/vy;
			nexty=-3;
		}
		else if(vy>0)
		{
			ty = (crit_length+R-ry)/vy;
			nexty=-30;
		}
	}

	if( is_critical[1] == 0 && is_critical[3]==0 ) //find out when it will enter either top or bottom crit zone
	{
		if(vy>0)//top
		{
			ty = (yedge-crit_length-R-ry)/vy;
			nexty=-10;
		}
		else if(vy<0)//bottom
		{
			ty = -(ry-R-crit_length)/vy;//fixed minus sign 12/12/13
			nexty=-30;
		}
	}


	if( is_critical[2]==1)
	{
		if(vx>0)//only event to worry about is crossing the periodic boundary
		{
			tx= (xedge-rx)/vx;
			nextx=-2;
		}
		else if(vx<0)
		{
			tx = (xedge-crit_length-R-rx)/vx;
			nextx=-20;
		}
	}

	if( is_critical[4]==1)
	{
		if(vx<0)
		{
			tx= -(rx)/vx;
			nextx=-4;
		}
		else if(vx>0)
		{
			tx = (crit_length+R-rx)/vx;
			nextx=-40;
		}
	}

	if( is_critical[2] == 0 && is_critical[4]==0 ) //find out when it will enter either top or bottom crit zone
	{
		if(vx>0)
		{
			tx = (xedge-crit_length-R-rx)/vx;
			nextx=-20;
		}
		else if(vx<0)
		{
			tx = -(rx-R-crit_length)/vx;//fixed minus sign 12/12/13
			nextx=-40;
		}
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

//Looks at all the particles and finds the lowest time
/*
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
*/
/*
void nextTimeP(particle& p1, particle p2)
{
	int nextwall=0, nextparticle=1;

	double t_wall=wallTimeP(p1, nextwall);
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
*/
double dotprod(vector2d<double> v1, vector2d<double> v2)
{
	double v1x=v1.x(), v1y=v1.y(), v2x=v2.x(), v2y=v2.y();
	return ( (v1x*v2x)+(v1y*v2y) );
}
double separation(particle p1, particle p2)
{
	vector2d<double> r1 = p1.getpos();
	vector2d<double> r2 = p2.getpos();
	return (r2-r1).mag();
}

void initialiseCollisions()
{
	for(int i=0;i<numberofparticles;i++)
	{
		int nextwall;
		//when we want to randomise parameters, do that here possibly
		double wall_time = wallTime(movingParticle[i],nextwall);
		double col_time = wall_time;
		movingParticle[i].setnextevent(0,nextwall);
		movingParticle[i].sett_next(col_time);
		
		for(int j=0;j<numberofparticles;j++)
		{
			double colprov;
			if(j!=i)
			{
				colprov = coltime(movingParticle[i],movingParticle[j]);
				if (colprov < col_time)
				{
					col_time = colprov;
					movingParticle[i].sett_next(col_time);
					movingParticle[i].setnextevent(0,j);
				}
			}
			
		}
	}
}

void initialiseCollisionsP()//this is very probably broken if particles start over crit line!
{
	int nextwall;
	for(int k=0;k<numberofparticles;k++)
	{
		//when we want to randomise parameters, do that here possibly
		double wall_time = wallTimeP(movingParticle[k],nextwall);
		double col_time = wall_time;
		movingParticle[k].setnextevent(0,nextwall);
		movingParticle[k].sett_next(col_time);
		
		for(int j=0;j<numberofparticles;j++)
		{
			double colprov;
			if(j!=k)
			{
				colprov = coltime(movingParticle[k],movingParticle[j]);
				if (colprov < col_time)
				{
					col_time = colprov;
					movingParticle[k].sett_next(col_time);
					movingParticle[k].setnextevent(0,j);
				}
			}
			
		}
	

int nextevent2 [2];
	double col_time2=10000;
	double ghost_time=100000;

	//top 3 quadrants
	if(movingParticle[k].getCrit1()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[1][i]=movingParticle[i];
			ghostParticle[1][i].setpos(movingParticle[i].getx() , movingParticle[i].gety() + yedge );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[i],ghostParticle[1][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=1;
				nextevent2[1]=i;
			}
		}

		//now deal with corners. 
		if(movingParticle[k].getCrit2()==1) //if it's in the top right
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[5][i]=movingParticle[i];
				ghostParticle[5][i].setpos(movingParticle[i].getx() + xedge , movingParticle[i].gety() + yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[i],ghostParticle[5][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=5;
					nextevent2[1]=i;
				}
			}
		}

		if(movingParticle[k].getCrit4()==1) //if it's in the top left
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[8][i]=movingParticle[i];
				ghostParticle[8][i].setpos(movingParticle[i].getx() - xedge , movingParticle[i].gety() + yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[i],ghostParticle[8][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=8;
					nextevent2[1]=i;
				}
			}
		}
	}


	//now look at bottom 3 cases
	if(movingParticle[k].getCrit3()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[3][i]=movingParticle[i];
			ghostParticle[3][i].setpos(movingParticle[i].getx() , movingParticle[i].gety() - yedge );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[i],ghostParticle[3][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=3;
				nextevent2[1]=i;
			}
		}

		//now deal with corners. 
		if(movingParticle[k].getCrit2()==1) //if it's in the bottom right
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[6][i]=movingParticle[i];
				ghostParticle[6][i].setpos(movingParticle[i].getx() + xedge , movingParticle[i].gety() - yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[i],ghostParticle[6][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=6;
					nextevent2[1]=i;
				}
			}
		}

		if(movingParticle[k].getCrit4()==1) //if it's in the bottom left
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[7][i]=movingParticle[i];
				ghostParticle[7][i].setpos(movingParticle[i].getx() - xedge , movingParticle[i].gety() - yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[i],ghostParticle[7][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=7;
					nextevent2[1]=i;
				}
			}
		}
	}


	//now look at right hand side
	if(movingParticle[k].getCrit2()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[2][i]=movingParticle[i];
			ghostParticle[2][i].setpos(movingParticle[i].getx()+xedge , movingParticle[i].gety() );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[i],ghostParticle[2][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=2;
				nextevent2[1]=i;
			}
		}
				
	}

	if(movingParticle[k].getCrit4()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[4][i]=movingParticle[i];
			ghostParticle[4][i].setpos(movingParticle[i].getx()-xedge , movingParticle[i].gety() );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[i],ghostParticle[4][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=4;
				nextevent2[1]=i;
			}
		}
				
	}

	if(col_time2<movingParticle[k].gett_next() )
	{
		movingParticle[k].sett_next(col_time2);
		movingParticle[k].setnextevent(nextevent2);
		dummy=movingParticle[k].getnextevent0();
	}
	

	}
	//old periodic boundary conditions thing - broken
	/*
	for(int i=0;i<numberofparticles;i++)
	{
		nextwall2=movingParticle[i].getnextevent();
		if(nextwall2<0)
		{
			switch (nextwall2)
			{
			case -1:
				for(int j=0;j<numberofparticles;j++)
				{
					ghostParticle[j]=movingParticle[j];
					ghostParticle[j].sety(ghostParticle[j].gety()+yedge);
				}
				break;
			case -2:
				for(int j=0;j<numberofparticles;j++)
				{
					ghostParticle[j]=movingParticle[j];
					ghostParticle[j].setx(ghostParticle[j].getx()+xedge);
				}
				break;
			case -3:
				for(int j=0;j<numberofparticles;j++)
				{
					ghostParticle[j]=movingParticle[j];
					ghostParticle[j].sety(ghostParticle[j].gety()-yedge);
				}
				break;
			case -4:
				for(int j=0;j<numberofparticles;j++)
				{
					ghostParticle[j]=movingParticle[j];
					ghostParticle[j].setx(ghostParticle[j].getx()-xedge);
				}
				break;
			}

			for(int j=0;j<numberofparticles;j++)
			{
				col_time=coltime(movingParticle[i],ghostParticle[j]);
				ghost_time=col_time;
				if(col_time<movingParticle[i].gett_next() )
				{
					movingParticle[i].sett_next(col_time);
					movingParticle[i].setnextevent(j);
				}
			}
			col_time=1000;
		}
	}*/
}

void updateCollisionsP(int changed)
{
	int nextwall=0;
	
	double wall_time = wallTimeP(movingParticle[changed],nextwall);//find out time for next wall and which wall/crit it hits
	movingParticle[changed].sett_next(wall_time);
	movingParticle[changed].setnextevent(0,nextwall);
	double col_time =10000;

	for(int i=0;i<numberofparticles;i++)
	{
		if(i!=changed)
		{
			col_time = coltime(movingParticle[i], movingParticle[changed]);

			if(col_time<movingParticle[changed].gett_next() )
			{
				movingParticle[changed].setnextevent(0,i);
				movingParticle[changed].sett_next(col_time);
			}
		}
	}

	
	int nextevent2 [2];
	double col_time2=10000;
	double ghost_time=100000;

	//top 3 quadrants
	if(movingParticle[changed].getCrit1()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[1][i]=movingParticle[i];
			ghostParticle[1][i].setpos(movingParticle[i].getx() , movingParticle[i].gety() + yedge );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[changed],ghostParticle[1][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=1;
				nextevent2[1]=i;
			}
		}

		//now deal with corners. 
		if(movingParticle[changed].getCrit2()==1) //if it's in the top right
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[5][i]=movingParticle[i];
				ghostParticle[5][i].setpos(movingParticle[i].getx() + xedge , movingParticle[i].gety() + yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[changed],ghostParticle[5][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=5;
					nextevent2[1]=i;
				}
			}
		}

		if(movingParticle[changed].getCrit4()==1) //if it's in the top left
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[8][i]=movingParticle[i];
				ghostParticle[8][i].setpos(movingParticle[i].getx() - xedge , movingParticle[i].gety() + yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[changed],ghostParticle[8][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=8;
					nextevent2[1]=i;
				}
			}
		}
	}


	//now look at bottom 3 cases
	if(movingParticle[changed].getCrit3()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[3][i]=movingParticle[i];
			ghostParticle[3][i].setpos(movingParticle[i].getx() , movingParticle[i].gety() - yedge );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[changed],ghostParticle[3][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=3;
				nextevent2[1]=i;
			}
		}

		//now deal with corners. 
		if(movingParticle[changed].getCrit2()==1) //if it's in the bottom right
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[6][i]=movingParticle[i];
				ghostParticle[6][i].setpos(movingParticle[i].getx() + xedge , movingParticle[i].gety() - yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[changed],ghostParticle[6][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=6;
					nextevent2[1]=i;
				}
			}
		}

		if(movingParticle[changed].getCrit4()==1) //if it's in the bottom left
		{
			for(int i=0;i<numberofparticles;i++)
			{
				ghostParticle[7][i]=movingParticle[i];
				ghostParticle[7][i].setpos(movingParticle[i].getx() - xedge , movingParticle[i].gety() - yedge );
			}
			
			for(int i=0;i<numberofparticles;i++)
			{
				ghost_time=coltime(movingParticle[changed],ghostParticle[7][i]);
				if(ghost_time<col_time2)
				{
					col_time2=ghost_time;
					nextevent2[0]=7;
					nextevent2[1]=i;
				}
			}
		}
	}


	//now look at right hand side
	if(movingParticle[changed].getCrit2()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[2][i]=movingParticle[i];
			ghostParticle[2][i].setpos(movingParticle[i].getx()+xedge , movingParticle[i].gety() );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[changed],ghostParticle[2][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=2;
				nextevent2[1]=i;
			}
		}
				
	}

	if(movingParticle[changed].getCrit4()==1)
	{
		for(int i=0;i<numberofparticles;i++)
		{
			ghostParticle[4][i]=movingParticle[i];
			ghostParticle[4][i].setpos(movingParticle[i].getx()-xedge , movingParticle[i].gety() );
		}
		
		for(int i=0;i<numberofparticles;i++)
		{
			ghost_time=coltime(movingParticle[changed],ghostParticle[4][i]);
			if(ghost_time<col_time2)
			{
				col_time2=ghost_time;
				nextevent2[0]=4;
				nextevent2[1]=i;
			}
		}
				
	}

	if(col_time2<movingParticle[changed].gett_next() )
	{
		movingParticle[changed].sett_next(col_time2);
		movingParticle[changed].setnextevent(nextevent2);
		dummy=movingParticle[changed].getnextevent0();
	}
	

}
/*
void updateCollisions(int changed)
{
	int nextwall=0;
	double wall_time = wallTimeP(movingParticle[changed],nextwall);
	movingParticle[changed].sett_next(wall_time);
	movingParticle[changed].setnextevent(nextwall);
	double col_time =10000;
	for(int i=0;i<numberofparticles;i++)
	{
		if(i!=changed)
		{
			col_time = coltime(movingParticle[i], movingParticle[changed]);

			if(col_time<movingParticle[changed].gett_next() )
			{
				movingParticle[changed].setnextevent(i);
				movingParticle[changed].sett_next(col_time);
			}
		}
	}
}
*/
void motion(double deltat)
{
	for(int i=0;i<numberofparticles;i++)
	{

		movingParticle[i].setx(movingParticle[i].getx() + movingParticle[i].getvelx()*deltat);
		movingParticle[i].sety(movingParticle[i].gety() + movingParticle[i].getvely()*deltat );
		/*for(int j=0;j<numberofparticles;j++)
		{
			ghostParticle[i][j].setx(ghostParticle[i][j].getx() + ghostParticle[i][j].getvelx()*deltat);
			ghostParticle[i][j].sety(ghostParticle[i][j].gety() + ghostParticle[i][j].getvely()*deltat );
		}*/

	}
}
/*
void motion_ghost(double deltat, int i)
{
	for(int j=0;i<numberofparticles;i++)
	{

		ghostParticle[i][j].setx(ghostParticle[i][j].getx() + ghostParticle[i][j].getvelx()*deltat);
		ghostParticle[i][j].sety(ghostParticle[i][j].gety() + ghostParticle[i][j].getvely()*deltat );

	}
}
*/
double nextTimeToIterate( int& particleinquestion , int& collisioncase)
{
	double leastvalue=10000;
	for(int i=0;i<numberofparticles;i++)
	{
		if( movingParticle[i].gett_next()<leastvalue )
		{
			leastvalue=movingParticle[i].gett_next();
			collisioncase=movingParticle[i].getnextevent1();
			particleinquestion=i;
		}
	}
	return leastvalue;
}

void minusDT(double deltat)
{
	for(int i=0;i<numberofparticles;i++)
	{
		movingParticle[i].sett_next(movingParticle[i].gett_next()-deltat );
	}
//	ghost_time-=deltat;
}
/*
void updateStragglers(int changed)
{
	for(int i=0;i<numberofparticles;i++)
	{
		if(movingParticle[i].getnextevent1()==changed)
		{
			updateCollisions(i);
		}
	}
}
*/
//finds particles which had the changed particle as its next collision, and updates them.
void updateStragglersP(int changed)
{
	for(int i=0;i<numberofparticles;i++)
	{
		if(movingParticle[i].getnextevent1()==changed)
		{
			updateCollisionsP(i);
		}
	}
}

void updateCrit(int partic, int critinquestion)
{
	switch(critinquestion)
	{
		case -10:
			movingParticle[partic].changeCrit1();
		break;
		case -20:
			movingParticle[partic].changeCrit2();
		break;
		case -30:
			movingParticle[partic].changeCrit3();
		break;
		case -40:
			movingParticle[partic].changeCrit4();
		break;
	}		
}

void updateCritBoundary(int partic, int critinquestion)
{
	switch(critinquestion)
	{
		case -1:
			movingParticle[partic].changeCrit1();
			movingParticle[partic].changeCrit3();
		break;
		case -2:
			movingParticle[partic].changeCrit2();
			movingParticle[partic].changeCrit4();
		break;
		case -3:
			movingParticle[partic].changeCrit1();
			movingParticle[partic].changeCrit3();
		break;
		case -4:
			movingParticle[partic].changeCrit2();
			movingParticle[partic].changeCrit4();
		break;
	}

			
}

void correctGhostParticle(int boundary, int nextcollision)
{
	switch(boundary)
	{
		ghostParticle[boundary][nextcollision].setvel(movingParticle[nextcollision].getvel() );
		case 1:
			ghostParticle[boundary][nextcollision].setpos(movingParticle[nextcollision].getx(),(movingParticle[nextcollision].gety() )+yedge );
			break;
		case 2:
			ghostParticle[boundary][nextcollision].setpos( (movingParticle[nextcollision].getx())+xedge,movingParticle[nextcollision].gety() );
			break;
		case 3:
			ghostParticle[boundary][nextcollision].setpos(movingParticle[nextcollision].getx(), (movingParticle[nextcollision].gety() )-yedge );
			break;
		case 4:
			ghostParticle[boundary][nextcollision].setpos( (movingParticle[nextcollision].getx() )-xedge,movingParticle[nextcollision].gety() );
			break;
		case 5:
			ghostParticle[boundary][nextcollision].setpos((movingParticle[nextcollision].getx() )+xedge, (movingParticle[nextcollision].gety() )+yedge );
			break;
		case 6:
			ghostParticle[boundary][nextcollision].setpos( (movingParticle[nextcollision].getx()+xedge ), (movingParticle[nextcollision].gety()-yedge ) );
			break;
		case 7:
			ghostParticle[boundary][nextcollision].setpos( (movingParticle[nextcollision].getx())-xedge , (movingParticle[nextcollision].gety() )-yedge );
			break;
		case 8:
			ghostParticle[boundary][nextcollision].setpos( (movingParticle[nextcollision].getx() )-xedge,(movingParticle[nextcollision].gety() )+yedge );
			break;
	}
}

void initialiseCrits()
{
	double x, y, rad;
	for(int i=0;i<numberofparticles;i++)
	{
		x=movingParticle[i].getx(), y=movingParticle[i].gety(), rad=movingParticle[i].getrad();
		if(x>xedge-crit_length-rad)
		{
			movingParticle[i].changeCrit2();
		}
		if(x<crit_length+rad)
		{
			movingParticle[i].changeCrit4();
		}
		if(y>yedge-crit_length-rad)
		{
			movingParticle[i].changeCrit1();
		}
		if(y<crit_length+rad)
		{
			movingParticle[i].changeCrit3();
		}
	}
}