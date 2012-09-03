#pragma once
#include <cstdlib>
#include <stdio.h> 　　
#include <time.h>
#include <iostream>
using namespace std;

/*#define SCALE 5*/
const int SCALE = 2;
const int MAX_X = (100*SCALE);
const int MAX_Y = (35*SCALE);
const int MAX_Z = (100*SCALE);
const int DISTRBU_OF_TREE = 20;//树的分布距离
const int INTERVAL = (5*SCALE);//插值采样点的距离
const char EMPTY = 0;
const char STONE = 1;
const char IRON = 2;
const char COAL = 3;
const char GOLD = 4;
const char SAND = 5;
const char WATER = 6;
const char LEAF = 7;
const char BOTTOM = 8;
const char SOIL = 9;
const char WOOD = 12;
const char TORCH = 14;
const char PLANK = 15;
const char SANDSTONE = 16;
const char BRICK = 17;
const char IRONBLOCK = 18;
const char GOLDBLOCK = 19;
const char BORDER = 20;
const char SHADOWED = 32;


class block
{
public:
	block(void);
	~block(void);
	void initialization();
	void output();
	char ***blocks;
private:
	//int ***blocks;
	void buildStones();
	
};

inline int RandomNum(int range);
