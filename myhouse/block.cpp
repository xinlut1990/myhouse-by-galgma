#include "StdAfx.h"
#include "block.h"


block::block(void)
{
	//Ϊ�洢�����ݵ���ά���ݷ���ռ�
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
	//�ͷſռ�
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
	//����ײ����ϵ�ʯ�������ڣ�������ε�N������ʯͷ
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
    //�߶�����
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
	//ÿ��һ����������һ�������
	for(int i = 0;i < MAX_X+1;i+=INTERVAL)
	{
		for(int j = 0;j < MAX_Z+1;j+=INTERVAL)
		{
			height[i][j] = RandomNum(20 * SCALE) + 10 * SCALE;
		}
	}
    //�ö�ά���Բ�ֵ�ķ���������������
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
			for(int k = 10 * SCALE + 1;k <= top - soilDepth;k++)//ʯͷ
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
			for(int k = top - soilDepth + 1;k <= top;k++)//��
			{
				blocks[i][k][j] = SOIL;
			}
			for(int k = 10 * SCALE;k <= 10 * SCALE + 4*SCALE;k++)//ɳ��
			{
 				if(blocks[i][k][j] == EMPTY) 
 					blocks[i][k][j] = SAND;
			}
			if(RandomNum(DISTRBU_OF_TREE * SCALE) == 0 && top > 10 * SCALE)//��
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
