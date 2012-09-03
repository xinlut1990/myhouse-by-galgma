#pragma once
#include "constvalue.h"
#include "fieldblock.h"

class Rule
{
public:
	Rule(int item, int count);
	virtual FieldBlock matchRule(int* mergefield) = 0;
protected:
	FieldBlock output;
};

class Rule3M3: public Rule
{
public:
	Rule3M3(int item[9], int outitem, int count);
	FieldBlock matchRule(int* mergefield);
private:
	int input[9];
};

class Rule1M1: public Rule
{
public:
	Rule1M1(int item, int outitem, int count);
	FieldBlock matchRule(int* mergefield);
private:
	int input;
};

class Rule2M2: public Rule
{
public:
	Rule2M2(int item[4], int outitem, int count);
	FieldBlock matchRule(int* mergefield);
private:
	int input[4];
};