#pragma once
#include <iostream>
#include "constvalue.h"
#include "mergerule.h"
#include "fieldblock.h"
#include "matchrule.h"

struct MergeField
{
	//��������9��
	int* input;
	//�������1��
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
	//��ʼ���ϳɹ���
	static void initializeRules();
	static void destructRules();
	//������Ʒ������ֵΪ��ʱ����Ӧ����Ʒ
	FieldBlock putItem(int n, FieldBlock block);
	//���ѡ�������
	FieldBlock getInputItem(int n);
	//���ѡ�������
	FieldBlock getOutputItem();
	//���غϳ���
	MergeField* getWholeItems();
	//�ϳɺ���
	FieldBlock startMerge();
	//���úϳ���
	void resetMerge();
private:
	MergeField* field;
};