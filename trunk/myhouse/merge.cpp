#include "stdafx.h"
#include "merge.h"

Rule* Merge::rules[RULE_SIZE];

Merge::Merge()
{
	field = new MergeField();
}

void Merge::initializeRules()
{
	int wood = WOOD;
	int sand[4] = {SAND,SAND,SAND,SAND};
	int torch[9] = {PLANK,PLANK,PLANK,PLANK,COAL,PLANK,PLANK,PLANK,PLANK};
	int goldblock[4] = {GOLD,GOLD,GOLD,GOLD};
	int Ironblock[4] = {IRON,IRON,IRON,IRON};
	int brick[4] = {SOIL, SOIL, STONE, STONE};
	Merge::rules[0] = new Rule1M1(wood,PLANK,4);
	Merge::rules[1] = new Rule2M2(sand,SANDSTONE,1);
	Merge::rules[2] = new Rule3M3(torch,TORCH,1);
	Merge::rules[3] = new Rule2M2(goldblock,GOLDBLOCK,1);
	Merge::rules[4] = new Rule2M2(Ironblock,IRONBLOCK,1);
	Merge::rules[5] = new Rule2M2(brick,BRICK,1);
}

void Merge::destructRules()
{
	for (int i = 0; i < RULE_SIZE; i++)
	{
		delete Merge::rules[i];
	}
}

FieldBlock Merge::putItem(int n, FieldBlock block)
{
	if (block.item != EMPTY)
	{
		if (field->input[n] == EMPTY)
		{
			field->input[n] = block.item;
			block.count--;
			if (block.count == 0)
			{
				block.item = EMPTY;
			}
			return block;
		}
		else
		{
			if (field->input[n] != block.item && block.count == 1)
			{
				FieldBlock temp;
				temp.item = field->input[n];
				temp.count = 1;
				field->input[n] = block.item;
				return temp;
			}
			else
			{
				return block;
			}
		}
	}
	FieldBlock temp;
	return temp;
}

FieldBlock Merge::getInputItem(int n)
{
	FieldBlock temp;
	temp.item = field->input[n];
	temp.count = 1;
	field->input[n] = EMPTY;
	return temp;
}

FieldBlock Merge::getOutputItem()
{
	FieldBlock temp = field->output;
	field->output.item = EMPTY;
	field->output.count = 0;
	return temp;
}

MergeField* Merge::getWholeItems()
{
	return field;
}

FieldBlock Merge::startMerge()
{
	FieldBlock temp;
	for (int i = 0; i < RULE_SIZE && temp.item == EMPTY; i++)
	{
		temp = Merge::rules[i]->matchRule(field->input);
	}
	field->output = temp;
	return temp;
}

void Merge::resetMerge()
{
	for (int i = 0; i < 9; i++)
	{
		field->input[i] = EMPTY;
	}
}