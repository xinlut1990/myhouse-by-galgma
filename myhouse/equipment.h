#pragma once
#include <iostream>
#include "constvalue.h"
#include "fieldblock.h"

//装备栏
class Equipment
{
public:
	Equipment();
	~Equipment();
	
	//以下用于查看
	//返回快捷栏
	FieldBlock* getQuickFields();
	//返回整个装备栏
	FieldBlock** getWholeFields();
	//查看某一格
	FieldBlock getItem(int line, int row);
	
	//以下用于对装备栏操作
	//左击选中物品
	FieldBlock getItemByLeftStick(int line, int row);
	//右击选中物品，只能右击一次
	FieldBlock getItemByRightStick(int line, int row);
	//放置物品，返回值为此时手中物品
	FieldBlock setItem(int line, int row, FieldBlock block);
	
	//以下用于手中物品操作
	//选择手中物品
	bool setHandingItem(int _line, int _row);
	//使用手中物品
	FieldBlock useHandingItem();

	//以下用于拾取物品
	//拾取物品放入装备栏，有位置放返回true，失败返回false
	bool catchItems(FieldBlock block);
private:
	FieldBlock** materials;
	int current_row;
	int current_line;
};
