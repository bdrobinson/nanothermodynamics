
#ifndef part
#define part

//#include "../include/vectors.h"
#include "vectors.h"

class particle 
{
private: 
	vector2d<double> pos, vel;
	double radius, mass, t_next;
	int nextevent [2]; //ghostness indicated by index from 1 to 8 (0 if not ghost collision, see desktop background for further details), collisions labelled 1 to N, walls labelled -1,-2,-3,-4 (clockwise starting from top)
	CGuiObject* myGuiObject;
	int is_critical [5]; //shows if particle is over critical edge
public:
particle()
{
/*	double snan = numeric_limits<double>::signaling_NaN();
	pos = vector2d<double>(snan, snan); 
	pos = vector2d<double>(snan, snan); 
	radius = snan;
	mass = snan;
	t_next=1;
*/
	pos = vector2d<double>(1, 1);
	vel = vector2d<double>(1, 8.3);
	radius = 1;
	mass = 1;
	t_next=1234;
}

particle(double x, double y, double z, double rad) 
{
	pos = vector2d<double>(x, y);
	radius = rad;
	mass = 0;
	t_next=1;
}

particle(double x, double y, double rad) 
{
	pos = vector2d<double>(x, y);
	radius = rad;
	mass = 0;
	t_next=1;
}
particle( const vector2d<double>& r, const vector2d<double>& v, double rad, double m) 
{
	pos = r;
	vel = v;
	radius = rad;
	mass = m;
	t_next=1;
}
particle(double x, double y, double z, double vx, double vy, double vz, double r, double m) 
{
	pos = vector2d<double>(x, y);
	vel = vector2d<double>(vx, vy);
	radius = r;
	mass = m;
	t_next=1;
}

particle(double x, double y, double vx, double vy, double r, double m) 
{
	pos = vector2d<double>(x, y);
	vel = vector2d<double>(vx, vy);
	radius = r;
	mass = m;
	t_next=1;
}

vector2d<double> getpos()
{
	return pos;
}
double getx()
{	
	return pos.x();
}
double gety()
{	
	return pos.y();
}
double getz()
{	
	return 0.0;
}
vector2d<double> getvel()
{
	return vel;
}
double getvelx()
{	
	return vel.x();
}
double getvely()
{	
	return vel.y();
}
double getvelz()
{	
	return 0.0;
}
double getrad()
{	
	return radius;
}
double getmass()
{	
	return mass;
}
double gett_next()
{
	return t_next;
}
int getnextevent0()
{
	return nextevent[0];
}
int getnextevent1()
{
	return nextevent[1];
}
vector2d<double> getp()
{
	return (vel*mass);
}
double getpx()
{
	return (mass*vel.x());
}
double getpy()
{
	return (mass*vel.y());
}
double getKE()
{
	return (0.5 * mass * ( vel.mag() )*( vel.mag() ) );
}

int getCrit1()
{
	return is_critical[1];
}

int getCrit2()
{
	return is_critical[2];
}

int getCrit3()
{
	return is_critical[3];
}

int getCrit4()
{
	return is_critical[4];
}

//===========================set values of copies of particle==============================

void setx(double value)
{
	double xcoord = value;
	pos.setx(xcoord);
}
void sety(double value)
{
	double ycoord = value;
    pos.sety(ycoord);
}
void setz(double value)
{
//	pos.setz(value);
}
void setpos(const vector2d<double>& r)
{
	double xcoord = r.x(), ycoord = r.y(); 
	pos = vector2d<double>(xcoord, ycoord);
}
void setpos(double x, double y)
{
	pos = vector2d<double>(x, y);
}
void setvelx(double value)
{
	vel.setx(value);
}
void setvely(double value)
{
	vel.sety(value);
}
void setvelz(double value)
{
//	vel.setz(value);
}
void setvel(const vector2d<double>& v)
{
	vel = v;
}
void setvel(double vx, double vy)
{
	vel = vector2d<double>(vx, vy);
}
void setvel(double r, double theta, char type)
{
	vel = vector2d<double>(r*cos(theta), r*sin(theta) );
}
void setrad(double value)
{
	radius = value;
}
void setmass(double value)
{
	mass = value;
}
void setp(vector2d<double> momentum)
{
	setvel(momentum/mass);
}
void sett_next(double t)
{
	t_next=t;
}
void setnextevent(int nextcol [2])
{
	nextevent[0]=nextcol[0];
	nextevent[1]=nextcol[1];
}
void setnextevent(int el0, int el1)
{
	nextevent[0]=el0;
	nextevent[1]=el1;
}
CGuiObject* getGuiObject()
{
	return myGuiObject;
}
void setGuiObject(CGuiObject* Ob)
{
	myGuiObject = Ob;
}

void changeCrit1()
{
	if(is_critical[1]==0)
	{
		is_critical[1]=1;
	}
	else if(is_critical[1]==1)
	{
		is_critical[1]=0;
	}
}

void changeCrit2()
{
	if(is_critical[2]==0)
	{
		is_critical[2]=1;
	}
	else if(is_critical[2]==1)
	{
		is_critical[2]=0;
	}
}

void changeCrit3()
{
	if(is_critical[3]==0)
	{
		is_critical[3]=1;
	}
	else if(is_critical[3]==1)
	{
		is_critical[3]=0;
	}
}

void changeCrit4()
{
	if(is_critical[4]==0)
	{
		is_critical[4]=1;
	}
	else if(is_critical[4]==1)
	{
		is_critical[4]=0;
	}
}

//=============================other stuff===============================

};


#endif