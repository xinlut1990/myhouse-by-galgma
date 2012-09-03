#pragma once
#include <iostream>
#include "constvalue.h"
#include "fieldblock.h"

//װ����
class Equipment
{
public:
	Equipment();
	~Equipment();
	
	//�������ڲ鿴
	//���ؿ����
	FieldBlock* getQuickFields();
	//��������װ����
	FieldBlock** getWholeFields();
	//�鿴ĳһ��
	FieldBlock getItem(int line, int row);
	
	//�������ڶ�װ��������
	//���ѡ����Ʒ
	FieldBlock getItemByLeftStick(int line, int row);
	//�һ�ѡ����Ʒ��ֻ���һ�һ��
	FieldBlock getItemByRightStick(int line, int row);
	//������Ʒ������ֵΪ��ʱ������Ʒ
	FieldBlock setItem(int line, int row, FieldBlock block);
	
	//��������������Ʒ����
	//ѡ��������Ʒ
	bool setHandingItem(int _line, int _row);
	//ʹ��������Ʒ
	FieldBlock useHandingItem();

	//��������ʰȡ��Ʒ
	//ʰȡ��Ʒ����װ��������λ�÷ŷ���true��ʧ�ܷ���false
	bool catchItems(FieldBlock block);
private:
	FieldBlock** materials;
	int current_row;
	int current_line;
};
