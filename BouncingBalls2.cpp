#include <iostream.h>
#include <fstream>
#include <cmath>
#include <vector>
#include "Particle Class.h"
//#include "Collision Time.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()


const int wallLength = 50;
const int particleNumber = 50;
particle movingParticles[particleNumber];
particle wallParticles[2*wallLength];
int height = 50;
double radius = 1; //this variables sets the radius of all wall particles

void initialiseSystem();
void initialiseWalls(int particlesPerWall);
void initialiseMovers(int numberOfParticles);
bool particleOverlaps(double x, double y, int index);
void collisiontime(int idnum);
double getCollisionTime(particle particle1, particle particle2);

int main(void)
{
	initialiseSystem();

	int nextCollisionTime = 0;

	
	return 0;
}

void collisiontime(int particleIndex) //calculates shortest time to next collision by calculating all times and only keeping the smallest
{
	double mintime = 0;
	int buddy = 0;

	for(int i=0;i<particleNumber;i++) //for(row=0;row<pvector.size() && != idnum;row++) something along these lines more efficient?
	{
		if(i != particleIndex)
		{
			double timetocoll = getCollisionTime(movingParticles[particleIndex], movingParticles[i]);

			if(i=0)
			{
				mintime = timetocoll;
				buddy = i;
			}
			else
			{		
				if(timetocoll != -1 && timetocoll<mintime)
				{
					mintime = timetocoll;
					buddy = i;
				}
			}
				
		}
	}

	for(i=0;i<2*wallLength;i++) //for(row=0;row<pvector.size() && != idnum;row++) something along these lines more efficient?
	{		
		double timetocoll = getCollisionTime(movingParticles[particleIndex], wallParticles[i]);

		if(i=0)
		{
			mintime = timetocoll;
			buddy = i;
		}
		else
		{		
			if(timetocoll != -1 && timetocoll<mintime)
			{
				mintime = timetocoll;
				buddy = i;
			}
		}		
	}
	
	movingParticles[particleIndex].settcoll(mintime);
	movingParticles[particleIndex].setbuddy(buddy);
}

double getCollisionTime(particle particle1, particle particle2)
{
	
	double a, b, c, t1, t2, timetocoll;

	a =		 particle1.getvelx()*particle1.getvelx() + particle1.getvely()*particle1.getvely() + particle1.getvelz()*particle1.getvelz()
		  +	 particle2.getvelx()*particle2.getvelx() + particle2.getvely()*particle2.getvely() + particle2.getvelz()*particle2.getvelz()
	   -2 * (particle2.getvelx()*particle1.getvelx() + particle2.getvely()*particle1.getvely() + particle2.getvelz()*particle1.getvelz() );
	
	b = 2 * (particle1.getx()*particle1.getvelx() + particle1.gety()*particle1.getvely() + particle1.getz()*particle1.getvelz()
		  +  particle2.getx()*particle2.getvelx() + particle2.gety()*particle2.getvely() + particle2.getz()*particle2.getvelz()
		  -  particle1.getx()*particle2.getvelx() - particle1.gety()*particle2.getvely() - particle1.getz()*particle2.getvelz()
		  -  particle2.getx()*particle1.getvelx() - particle2.gety()*particle1.getvely() - particle2.getz()*particle1.getvelz() );

	c =		 particle1.getx()*particle1.getx() + particle1.gety()*particle1.gety() + particle1.getz()*particle1.getz()
		  +	 particle2.getx()*particle2.getx() + particle2.gety()*particle2.gety() + particle2.getz()*particle2.getz()
	   -2 * (particle1.getx()*particle2.getx() + particle1.gety()*particle2.gety() + particle1.getz()*particle2.getz())
		  -((particle2.getrad() + particle1.getrad())*(particle2.getrad() + particle1.getrad()));
	

	if (b * b - 4 * a * c >=0)
	{
		t1 = (- b + sqrt(b * b - 4 * a * c)) / (2*a);
		t2 = (- b - sqrt(b * b - 4 * a * c)) / (2*a);
        
		if(t1 > 0 && t2 < 0)
		{
			timetocoll = t1;
		}
		else if(t2 > 0 && t1 < 0)
		{
			timetocoll = t2;
		}
		else if(t2 > 0 && t1 > 0)
		{
			if(t1<=t2)
			{
			timetocoll = t1;
			}
			else
			{
			timetocoll = t2;
			}
		}
		else //t1 < 0 && t2 < 0
		{
			timetocoll = -1;
		}
	}
	else
	{
		timetocoll = -1;
	}
	
	
	
	return timetocoll;
}







void initialiseSystem()
{
	initialiseWalls(wallLength);
	initialiseMovers(particleNumber);

	for(int i=0; i <particleNumber; i++)
	{
		cout << movingParticles[i].getx() << '\t' << movingParticles[i].gety() <<'\n';
	}
}

void initialiseMovers(int numberOfParticles)
{
	for(int i=0; i<numberOfParticles;i++)
	{
		double x;
		double y;

		do
		{
			x = wallLength*radius*((double)rand()/RAND_MAX);
			y = height*radius*((double)rand()/RAND_MAX);
		}while(particleOverlaps(x,y,i));

		particle newParticle(x, y, 0, 5.0, 5.0, 0, radius, 1);

		movingParticles[i] = newParticle;
	}
}

bool particleOverlaps(double x, double y, int index)
{
	while(index-1 >= 0)
	{
		particle currentParticle = movingParticles[index-1];
		double currX = currentParticle.getx();
		double currY = currentParticle.gety();

		if (sqrt((currX - x)*(currX - x) + (currY - y)*(currY - y))
			<= ( currentParticle.getrad() + radius ))
		{
			return true;
		}

		index--;
	}

	for(int i=0; i<2*wallLength; i++)
	{
		particle currentParticle = wallParticles[i];
		double currX = currentParticle.getx();
		double currY = currentParticle.gety();

		if (sqrt((currX - x)*(currX - x) + (currY - y)*(currY - y))
			<= ( currentParticle.getrad() + radius ))
		{
			return true;
		}
	}
	
	return false;
}

void initialiseWalls(int particlesPerWall)
{
	for(int i=0; i<particlesPerWall; i++)
	{
		particle newParticle1(2*i*radius, 0, 0, radius);
		particle newParticle2(2*i*radius, height*radius, 0, radius);

		wallParticles[i] = newParticle1;
		wallParticles[particlesPerWall+i] = newParticle2;
	}
}
