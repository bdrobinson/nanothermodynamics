
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Particle Class.h" 
//#include <ctime>    // For time()
//#include <cstdlib> 

void collisiontime(int idnum) //calculates shortest time to next collision by calculating all times and only keeping the smallest
{
	double a, b, c, t1, t2, timetocoll, mintime;

	for(int row=0;row<pvector.size();row++) //for(row=0;row<pvector.size() && != idnum;row++) something along these lines more efficient?
	{
		if(row != idnum)
		{
			
			a =		 pvector[idnum].getvelx()*pvector[idnum].getvelx() + pvector[idnum].getvely()*pvector[idnum].getvely() + pvector[idnum].getvelz()*pvector[idnum].getvelz()
				  +	 pvector[row].getvelx()*pvector[row].getvelx() + pvector[row].getvely()*pvector[row].getvely() + pvector[row].getvelz()*pvector[row].getvelz()
			   -2 * (pvector[row].getvelx()*pvector[idnum].getvelx() + pvector[row].getvely()*pvector[idnum].getvely() + pvector[row].getvelz()*pvector[idnum].getvelz() );
			
			b = 2 * (pvector[idnum].getx()*pvector[idnum].getvelx() + pvector[idnum].gety()*pvector[idnum].getvely() + pvector[idnum].getz()*pvector[idnum].getvelz()
				  +  pvector[row].getx()*pvector[row].getvelx() + pvector[row].gety()*pvector[row].getvely() + pvector[row].getz()*pvector[row].getvelz()
				  -  pvector[idnum].getx()*pvector[row].getvelx() - pvector[idnum].gety()*pvector[row].getvely() - pvector[idnum].getz()*pvector[row].getvelz()
				  -  pvector[row].getx()*pvector[idnum].getvelx() - pvector[row].gety()*pvector[idnum].getvely() - pvector[row].getz()*pvector[idnum].getvelz() );
		
			c =		 pvector[idnum].getx()*pvector[idnum].getx() + pvector[idnum].gety()*pvector[idnum].gety() + pvector[idnum].getz()*pvector[idnum].getz()
				  +	 pvector[row].getx()*pvector[row].getx() + pvector[row].gety()*pvector[row].gety() + pvector[row].getz()*pvector[row].getz()
			   -2 * (pvector[idnum].getx()*pvector[row].getx() + pvector[idnum].gety()*pvector[row].gety() + pvector[idnum].getz()*pvector[row].getz())
			      -((pvector[row].getrad() + pvector[idnum].getrad())*(pvector[row].getrad() + pvector[idnum].getrad());
			

			if (b * b - 4 * a * c >=0)
			{
				t1 = (- b + sqrt(b * b - 4 * a * c)) / (2*a);
				t2 = (- b - sqrt(b * b - 4 * a * c)) / (2*a);
                
				if(t1 > 0 && t2 < 0)
					{
						timetocoll = t1;
					}
				if(t2 > 0 && t1 < 0)
					{
						timetocoll = t2;
					}
				if(t2 > 0 && t1 > 0)
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
				if(t1 < 0 && t2 < 0)
					{
						timetocoll = -1;
					}
			}
			else
			{
				timetocoll = -1;
			}
		if(row=0)
		{
			mintime = timetocoll;
			buddy = row;
		}
		else
		{		
			if(timetocoll != -1 && timetocoll<mintime)
			{
				mintime = timetocoll;
				buddy = row;
			}
		}
				
		}
	}
	
	

	pvector[idnum].settcoll()= mintime;
	pvector[idnum].setbuddy() = buddy;
}

int nexttocollide(VECTOR input) //call this function when checking to see next pair to collide
{
	int partrow;
	double minimum;

	minimum = input[0].gettcoll();
	partrow = 0;
	for(row=1;row<input.size();row++) 
	{
		if(input[row].gettcoll()<=minimum && input[row].gettcoll()!= -1)
		{
			minimum = input[row].gettcoll();
			partrow = row;
		}
	}
	return partrow;
}

void scattering (int i)//contains the physics of two colliding particles (includiong walls)
{
	int j;
	double rijx, rijy, rijz, unit_rijx, unit_rijy, unit_rijz, mag_rij,
		   momix, momiy, momiz, momjx, momjy, momjz,
		   momix2, momiy2, momiz2, momjx2, momjy2, momjz2, factor;
		
	for(row=0;row<pvector.size();row++) //updates the position of all the particles
	{
		pvector[row].setx(pvector[row].getx() + pvector[row].getvelx()*pvector[partrow].gettcoll());
		pvector[row].sety(pvector[row].gety() + pvector[row].getvely()*pvector[partrow].gettcoll());
		pvector[row].setz(pvector[row].getz() + pvector[row].getvelz()*pvector[partrow].gettcoll());
		pvector[row].settcoll(0);
	}
	
	j = pvector[i].getbuddy();
	if (pvector[i].gettype()=1 && pvector[j].gettype=1)
	{
	rijx = (pvector[i].getx() - pvector[j].getx());
	rijy = (pvector[i].gety() - pvector[j].gety());
	rijz = (pvector[i].getz() - pvector[j].getz());
	mag_rij = sqrt((rijx)*(rijx) + (rijy)*(rijy) + (rijz)*(rijz));
	unit_rijx = rijx/mag_rij;
	unit_rijy = rijy/mag_rij;
	unit_rijz = rijz/mag_rij;

	momix = pvector[i].getmass() * pvector[i].getvelx();
	momiy = pvector[i].getmass() * pvector[i].getvely();
	momiz = pvector[i].getmass() * pvector[i].getvelz();
	momjx = pvector[j].getmass() * pvector[j].getvelx(); 
	momjy = pvector[j].getmass() * pvector[j].getvely();
	momjz = pvector[j].getmass() * pvector[j].getvelz();

	factor = ((momjx - momix)*unit_rijx + 
			  (momjy - momiy)*unit_rijy +
			  (momjz - momiz)*unit_rijz);

	momix2 = factor*unit_rijx + momix;
	momiy2 = factor*unit_rijy + momiy;
	momiz2 = factor*unit_rijz + momiz;
	momjx2 = (momix + momjx) - momix2;
	momjy2 = (momiy + momjy) - momiy2;
	momjz2 = (momiz + momjz) - momiz2;

	vel_ix2 = momix2/(pvector[i].getmass());
	vel_iy2 = momiy2/(pvector[i].getmass());
	vel_iz2 = momiz2/(pvector[i].getmass());
	vel_jx2 = momjx2/(pvector[j].getmass());
	vel_jy2 = momjy2/(pvector[j].getmass());
	vel_jz2 = momjz2/(pvector[j].getmass());

	pvector[i].setvelx(vel_ix2);
	pvector[i].setvely(vel_iy2);
	pvector[i].setvelz(vel_iz2);
	pvector[j].setvelx(vel_jx2);
	pvector[j].setvely(vel_jy2);
	pvector[j].setvelz(vel_jz2);

	if (pvector[i].gettype() = 2) 
	{
	pvector[i].setvelx(0.0);
	pvector[i].setvely(0.0);
	pvector[i].setvelz(0.0);//not necessary but better safe than sorry
	
	pvector[j].setvely() = (-1)*pvector[j].setvely();
	}

	if (pvector[j].gettype() = 2) 
	{
	pvector[j].setvelx(0.0);
	pvector[j].setvely(0.0);
	pvector[j].setvelz(0.0); //not necessary but better safe than sorry
	
	pvector[i].setvely() = (-1)*pvector[i].setvely();	
	}
	
	}
}
