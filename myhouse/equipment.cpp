#include "stdafx.h"
#include "equipment.h"

Equipment::Equipment():current_line(0), current_row(0)
{
	materials = new FieldBlock*[FIELD_LINES];
	for (int i = 0; i < FIELD_LINES; i++)
	{
		materials[i] = new FieldBlock[FIELD_ROWS];
	}
}

Equipment::~Equipment()
{
	for (int i = 0; i < FIELD_LINES; i++)
	{
		delete []materials[i];
	}
	delete []materials;
}

FieldBlock* Equipment::getQuickFields()
{
	return materials[0];
}

FieldBlock** Equipment::getWholeFields()
{
	return materials;
}

FieldBlock Equipment::getItem(int line, int row)
{
	if (line <= FIELD_LINES-1 && row <= FIELD_ROWS-1)
	{
		return materials[line][row];
	} 
	FieldBlock temp;
	return temp;
}

FieldBlock Equipment::getItemByLeftStick(int line, int row)
{
	if (line <= FIELD_LINES-1 && row <= FIELD_ROWS-1)
	{
		if (materials[line][row].item != EMPTY)
		{
			FieldBlock temp = materials[line][row];
			materials[line][row].count = 0;
			materials[line][row].item = EMPTY;
			return temp;
		}
	} 
	FieldBlock temp;
	return temp;
}

FieldBlock Equipment::getItemByRightStick(int line, int row)
{
	if (line <= FIELD_LINES-1 && row <= FIELD_ROWS-1)
	{
		if (materials[line][row].item != EMPTY)
		{
			FieldBlock temp = materials[line][row];
			temp.count = 1;
			materials[line][row].count--;
			if (materials[line][row].count == 0)
			{
				materials[line][row].item = EMPTY;
			}
			return temp;
		}
	} 
	FieldBlock temp;
	return temp;
}

FieldBlock Equipment::setItem(int line, int row, FieldBlock block)
{
	if (line <= FIELD_LINES-1 && row <= FIELD_ROWS-1 && block.count <= MAX_ITEMS && block.item != EMPTY)
	{
		//如果单元格为空
		if (materials[line][row].item == EMPTY)
		{
			materials[line][row] = block;
			FieldBlock temp;
			return temp;
		}
		//如果单元格物品与抓取物品不一致
		else if (materials[line][row].item != block.item)
		{
			FieldBlock temp = materials[line][row];
			materials[line][row] = block;
			return temp;
		}
		//如果一致
		else if (materials[line][row].item == block.item)
		{
			//如果单元格未满
			if (block.count + materials[line][row].count <= MAX_ITEMS)
			{
				materials[line][row].count += block.count;
				FieldBlock temp;
				return temp;
			}
			//如果单元格已满
			else
			{
				materials[line][row].count = MAX_ITEMS;
				block.count = materials[line][row].count + block.count -MAX_ITEMS;
				return block;
			}
		}
	} 
	FieldBlock temp;
	return temp;
}

bool Equipment::setHandingItem(int _line, int _row)
{
	if (_line < FIELD_LINES && _row < FIELD_ROWS)
	{
		current_line = _line;
		current_row = _row;
		return true;
	}
	else
	{
		return false;
	}
}

FieldBlock Equipment::useHandingItem()
{
	if (current_row >= 0 &&current_line >= 0)
	{
		if (materials[current_line][current_row].item != EMPTY && materials[current_line][current_row].count >= 1)
		{
			materials[current_line][current_row].count--;
			FieldBlock temp(materials[current_line][current_row].item,1);
			if (materials[current_line][current_row].count == 0)
			{
				materials[current_line][current_row].item = EMPTY;
			}
			return temp;
		}
	}
	FieldBlock temp;
	return temp;
}

bool Equipment::catchItems(FieldBlock block)
{
	int empty_row = -1, empty_line = -1;
	if (block.item != EMPTY && block.count > 0)
	{
		for (int i = 0; i < FIELD_LINES; i++)
		{
			for (int j = 0; j < FIELD_ROWS; j++)
			{
				if (materials[i][j].item == block.item && materials[i][j].count+block.count <= MAX_ITEMS)
				{
					materials[i][j].count += block.count;
					return true;
				} 
				else if (empty_line == -1 && empty_row == -1 && materials[i][j].item == EMPTY)
				{
					empty_line = i;
					empty_row = j;
				}
			}
		}
		if (empty_line != -1 && empty_row != -1)
		{
			materials[empty_line][empty_row].item = block.item;
			materials[empty_line][empty_row].count = block.count;
			return true;
		}
		return false;
	}
	return false;
}