#include "StdAfx.h"
#include "block.h"


block::block(void)
{
	//为存储块数据的三维数据分配空间
	blocks = new char**[MAX_X+2];

	    for(int i = 0; i <= MAX_X+1; i++)
		{
			blocks[i] = new char*[MAX_Y+2];
			for(int j = 0; j <= MAX_Y+1; j++)
			{
				blocks[i][j] = new char[MAX_Z+2];
				for(int k = 0; k <= MAX_Z+1; k++)
				{
  					if(i == 0 || j == 0 || k == 0||i == MAX_X+1 || j == MAX_Y+1 || k == MAX_Z+1)
  						blocks[i][j][k] = BORDER;
  					else 
						blocks[i][j][k] = EMPTY;
				}
			}
		}
	srand((unsigned)time(0)); 
}

block::~block(void)
{
	//释放空间
	for(int i = 0; i < MAX_X+1; i++)
	{
		for(int j = 0; j < MAX_Y+1; j++)
		{
			delete[] blocks[i][j];
		}
		delete[] blocks[i];
	}
	delete[] blocks;
}

void block::buildStones()
{
	//在最底层铺上底石（不能挖），在其次的N层铺上石头
	for(int i = 1;i <= MAX_X;i++)
	{
		for(int j = 1;j <= MAX_Z;j ++)
		{
			blocks[i][1][j] = BOTTOM;
			int probabOre;
			for(int k = 2;k <= 10*SCALE;k ++)
			{
				probabOre = RandomNum(20);
				if (probabOre == 0)
				{
					blocks[i][k][j] = GOLD;
				}
				else if (probabOre == 1)
				{
					blocks[i][k][j] = IRON;
				}
				else
				{
					blocks[i][k][j] = STONE;
				}
			}
		}
	}
}

void block::initialization()
{

	buildStones();
    //高度数组
	int height[MAX_X+1][MAX_Z+1];
	int Q11 = 0;
	int Q21 = 0;
	int Q12 = 0;
	int Q22 = 0;
	int R1 = 0;
	int R2 = 0;
	int x1 = 0;
	int x2 = 0;
	int z1 = 0;
	int z2 = 0;
	float temp1 = 0;
	float temp2 = 0;
	//每隔一定距离生成一个随机数
	for(int i = 0;i < MAX_X+1;i+=INTERVAL)
	{
		for(int j = 0;j < MAX_Z+1;j+=INTERVAL)
		{
			height[i][j] = RandomNum(20 * SCALE) + 10 * SCALE;
		}
	}
    //用二维线性插值的方法填满其余数据
	for(int i = 0;i < MAX_X;i++)
	{
		for(int j = 0;j < MAX_Z;j++)
		{
			x1 = i / INTERVAL * INTERVAL;
			z1 = j / INTERVAL * INTERVAL;
			x2 = x1 + INTERVAL;
			z2 = z1 + INTERVAL;
			Q11 = height[x1][z1];
			Q21 = height[x2][z1];
			Q12 = height[x1][z2];
			Q22 = height[x2][z2];
			temp1 = 1 / (float)(x2 - x1);
			temp2 = 1 / (float)(z2 - z1);
			R1 = (float)(x2 - i) * temp1 * Q11 + (float)(i - x1) * temp1 * Q21;
			R2 = (float)(x2 - i) * temp1 * Q12 + (float)(i - x1) * temp1 * Q22;
			height[i][j] = (float)(z2 - j) * temp2 * R1 + (float)(j - z1) * temp2 * R2;
		}
	}
	
	int top = 0;
	int soilDepth = 0;
	int sandDepth = 0;
	for(int i = 1;i <= MAX_X;i++)
	{
		for(int j = 1;j <= MAX_Z;j++)
		{
			top = height[i-1][j-1];
			soilDepth = RandomNum(SCALE)+SCALE;
			sandDepth = RandomNum(SCALE/2+1);
			int probabOre;
			for(int k = 10 * SCALE + 1;k <= top - soilDepth;k++)//石头
			{
				probabOre = RandomNum(20);
				if (probabOre == 0)
				{
					blocks[i][k][j] = COAL;
				}
				else if (probabOre == 1)
				{
					blocks[i][k][j] = IRON;
				}
				else
				{
					blocks[i][k][j] = STONE;
				}
			}
			for(int k = top - soilDepth + 1;k <= top;k++)//土
			{
				blocks[i][k][j] = SOIL;
			}
			for(int k = 10 * SCALE;k <= 10 * SCALE + 4*SCALE;k++)//沙子
			{
 				if(blocks[i][k][j] == EMPTY) 
 					blocks[i][k][j] = SAND;
			}
			if(RandomNum(DISTRBU_OF_TREE * SCALE) == 0 && top > 10 * SCALE)//树
			{
				for(int k = top + 1;k < top + 6;k++)
				{
					if (k > top + 3)
					{
						blocks[i+1][k][j] = LEAF;
						blocks[i-1][k][j] = LEAF;
						blocks[i+1][k][j+1] = LEAF;
						blocks[i-1][k][j+1] = LEAF;
						blocks[i+1][k][j-1] = LEAF;
						blocks[i-1][k][j-1] = LEAF;
						blocks[i][k][j+1] = LEAF;
						blocks[i][k][j-1] = LEAF;
					}
					blocks[i][k][j] = WOOD;
				}
			}
		}
	}

}

inline int RandomNum(int range)
{
	return (rand()%range);
}
