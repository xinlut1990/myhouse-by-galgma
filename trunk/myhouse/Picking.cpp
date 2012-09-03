// Picking.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

#define EDGE 3
#define LIMIT 3

struct vec 
{
	int x;
	int y;
	int z;
};

int Picking(vec* p, vec* d, int*** maps)
{
	int tempX = p->x;
	int tempY = 0;
	int tempZ = 0;
	int i;
	for (i = 0; i<=LIMIT; tempX++,i++)
	{
		tempY = (tempX - p->x)*(d->y)/(d->x)+(p->y);
		tempY = tempY/EDGE;

		tempZ = (tempX - p->x)*(d->z)/(d->x)+(p->z);
		tempZ = tempZ/EDGE;

		if (maps[tempX][tempY][tempZ]!=0)
		{
			cout << tempX << ' ' << tempY << ' ' << tempZ << endl;
			return maps[tempX][tempY][tempZ];
		}
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	vec* p = new vec;
	vec* d = new vec;

	int z1[3] = {0,1,2};
	int z2[3] = {0,1,0};
	int z3[3] = {0,1,1};
	int z4[3] = {1,1,0};
	int z5[3] = {3,1,0};
	int z6[3] = {5,1,3};
	int z7[3] = {0,1,5};
	int z8[3] = {4,1,1};
	int z9[3] = {3,1,2};
	int z10[3] = {1,1,3};
	int z11[3] = {1,1,1};
	int z12[3] = {3,1,1};
	int z13[3] = {0,1,0};
	int z14[3] = {1,1,0};
	int z15[3] = {0,0,0};
	int z16[3] = {4,1,0};
	int z17[3] = {0,1,2};
	int z18[3] = {0,1,0};
	int z19[3] = {0,1,4};
	int z20[3] = {0,1,0};
	int z21[3] = {0,1,0};
	int z22[3] = {5,1,1};
	int z23[3] = {1,1,0};
	int z24[3] = {0,1,0};
	int z25[3] = {0,1,0};

	int* y1[5] = {z1, z2, z3, z4,z5};
	int* y2[5] = {z6, z7, z8, z9,z10};
	int* y3[5] = {z11, z12, z13, z14,z15};
	int* y4[5] = {z16, z17, z18, z19,z20};
	int* y5[5] = {z21, z22, z23, z24,z25};

	int** maps[5] = {y1, y2, y3 ,y4, y5};

	cin>> p->x >> p->y >> p->z;
	cin>> d->x >> d->y >> d->z;

	int results;
	results = Picking(p, d, maps);

	cout << results << endl;

	return 0;
}

