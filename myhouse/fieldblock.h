#pragma once

//��Ԫ��
struct FieldBlock
{
	int item;
	int count;
	FieldBlock():item(0), count(0){}
	FieldBlock(int _item, int _count):item(_item), count(_count){}
};