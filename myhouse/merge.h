#pragma once
#include <iostream>
#include "constvalue.h"
#include "mergerule.h"
#include "fieldblock.h"
#include "matchrule.h"

struct MergeField
{
	//输入栏，9格
	int* input;
	//输出栏，1格
	FieldBlock output;
	MergeField():input(new int[MERGE_FIELDS])	
	{
		for (int i = 0; i < MERGE_FIELDS; i++)
		{
			input[i] = EMPTY;
		}
	}
	~MergeField()
	{
		if (input != NULL)
		{
			delete []input;
		}
	}
};

class Merge
{
public:
	Merge();
	static Rule* rules[RULE_SIZE];
	//初始化合成规则
	static void initializeRules();
	static void destructRules();
	//放置物品，返回值为此时手中应有物品
	FieldBlock putItem(int n, FieldBlock block);
	//点击选中输入格
	FieldBlock getInputItem(int n);
	//点击选中输出格
	FieldBlock getOutputItem();
	//返回合成栏
	MergeField* getWholeItems();
	//合成函数
	FieldBlock startMerge();
	//重置合成栏
	void resetMerge();
private:
	MergeField* field;
};