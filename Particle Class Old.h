
#ifndef part
#define part

class particle 
{
private: 
	double xcoord, ycoord, zcoord, xvel, yvel, zvel, radius, mass, tcoll;
	int buddy;
	char buddyType; //m = moving, w = wall, b = boundary.
	bool justCollided;

public: 
	static double xTotalLength, yTotalHeight;

particle()
{
	xcoord = 1.0; 
	ycoord = 1.0; 
	zcoord = 0.0;
	xvel = -1.0;
	yvel = -1.0;
	zvel = 0.0;
	radius = 1.0;
	mass = 1.0;
	tcoll = -1;
	buddy = -50;
	justCollided = false;
	//std::cout << xcoord << std::endl;
}

particle(double x, double y, double z, double rad) 
{
	xcoord = x;
	ycoord = y;
	zcoord = z;
	xvel = 0;
	yvel = 0;
	zvel = 0;
	radius = rad;
	mass = 0;
	justCollided = false;
 
}

particle(double x, double y, double z, double vx, double vy, double vz, double r, double m) 
{
	xcoord = x;
	ycoord = y;
	zcoord = z;
	xvel = vx;
	yvel = vy;
	zvel = vz;
	radius = r;
	mass = m;
	tcoll = -1;	
	justCollided = false;

}


void print() 
{
	/*std::cout << xcoord << '\t'
	<< ycoord << '\t'
	<< zcoord << std::endl
	<< xvel << '\t'
	<< yvel << '\t'
	<< zvel << std::endl;*/
}

bool getJustCollided()
{
	return justCollided;
}

void setJustCollided(bool value)
{
	justCollided = value;
}

double getx()
{	
	return xcoord;
}
double gety()
{	
	return ycoord;
}
double getz()
{	
	return zcoord;
}
double getvelx()
{	
	return xvel;
}
double getvely()
{	
	return yvel;
}
double getvelz()
{	
	return zvel;
}
double getrad()
{	
	return radius;
}
double getmass()
{	
	return mass;
}
double gettcoll()
{	
	return tcoll;
}
int getbuddy()
{	
	return buddy;
}
char getbuddyType()
{	
	return buddyType;
}
//===========================set values of copies of particle==============================

void setx(double value)
{
	xcoord = value;
	if ( value < 0.0 )
		xcoord = value + xTotalLength;
	else if ( value > xTotalLength )
		xcoord = value - xTotalLength;
}
void sety(double value)
{
	ycoord = value;
	if ( value < 0.0 )
		ycoord = value + yTotalHeight;
	else if ( value > yTotalHeight )
		ycoord = value - yTotalHeight;
}
void setz(double value)
{
	zcoord = value;
}
void setvelx(double value)
{
	xvel = value;
}
void setvely(double value)
{
	yvel = value;
}
void setvelz(double value)
{
	zvel = value;
}
void setrad(double value)
{
	radius = value;
}
void setmass(double value)
{
	mass = value;
}
void settcoll(double value)
{
	tcoll = value;
}
void setbuddy(int value)
{
	buddy = value;
}

void setbuddyType(char value)
{
	buddyType = value;
}
//=============================other stuff===============================

};

#endif
 

 


