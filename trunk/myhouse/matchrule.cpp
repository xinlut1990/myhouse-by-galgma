#include "stdafx.h"
#include "matchrule.h"

Rule::Rule(int item, int count)
{
	output.item = item;
	output.count = count;
}

Rule1M1::Rule1M1(int item, int outitem, int count):Rule(outitem, count), input(item){}

Rule2M2::Rule2M2(int item[4], int outitem, int count):Rule(outitem, count)
{
	for (int i = 0; i < 4; i++)
	{
		input[i] = item[i];
	}
}

FieldBlock Rule1M1::matchRule(int* mergefield)
{
	int blocks[9][1] = {{0},{1},{2},{3},{4},{5},{6},{7},{8}};
	for (int i = 0; i < 9; i++)
	{
		bool merged = true;
		for (int j = 0, k = 0; j < 9 && k < 1; j++)
		{
			if (j == blocks[i][k])
			{
				if (mergefield[j] == input)
				{
					k++;
				} 
				else
				{
					merged = false;
				}
			} 
			else
			{
				if (mergefield[j] != EMPTY)
				{
					merged = false;
				} 
			}
		}
		if (merged)
		{
			return output;
		}
		else if (!merged && i == 8)
		{
			FieldBlock temp;
			return temp;
		}
	}
	FieldBlock temp;
	return temp;
}

FieldBlock Rule2M2::matchRule(int* mergefield)
{
	int blocks[4][4] = {{0,1,3,4},{1,2,4,5},{3,4,6,7},{4,5,7,8}};
	for (int i = 0; i < 4; i++)
	{
		bool merged = true;
		for (int j = 0, k = 0; j < 9 && k < 4; j++)
		{
			if (j == blocks[i][k])
			{
				if (mergefield[j] == input[k])
				{
					k++;
				} 
				else
				{
					merged = false;
				}
			} 
			else
			{
				if (mergefield[j] != EMPTY)
				{
					merged = false;
				} 
			}
		}
		if (merged)
		{
			return output;
		}
		else if (!merged && i == 3)
		{
			FieldBlock temp;
			return temp;
		}
	}
	FieldBlock temp;
	return temp;
}

Rule3M3::Rule3M3(int item[9], int outitem, int count):Rule(outitem, count)
{
	for (int i = 0; i < 9; i++)
	{
		input[i] = item[i];
	}
}

FieldBlock Rule3M3::matchRule(int* mergefield)
{
	for (int i = 0; i < 9; i++)
	{
		if (input[i] != mergefield[i])
		{
			FieldBlock temp;
			return temp;
		}
	}
	return output;
}
