#pragma once
#include <d3dx9.h>
#include "d3dUtility.h"
#include "block.h"
#include "equipment.h"
#include "merge.h"
#include "fieldblock.h"
#define iwidth 57
#define iheight 78


class UI
{
public:
	UI();
	~UI();
	void UIInitialize(IDirect3DDevice9* device);
	void drawItems(IDirect3DDevice9* device);
	void drawAnItem(IDirect3DDevice9* device, int type, int row, int col, D3DXVECTOR3* origin, int number);
	void drawAnItemS(IDirect3DDevice9* device, int type, int col, int number);
	void drawSelectedItem(IDirect3DDevice9* device);
	void drawInventory(IDirect3DDevice9* device);
	void drawShortcutMenu(IDirect3DDevice9* device);
	bool selectItem(int mX, int mY, bool takeAll);
	void updateMousePos(int mX, int mY);
	void setShortcutFocus(int num);
	int getItemInHand();
	bool pointInRect(RECT rect, POINT point);
	Equipment* inventory;
private:
	int mX;
	int mY;
	Merge merge;
	int shortcutfocus;
	FieldBlock selectedItem;
	POINT itemPosInTexture[20];
	IDirect3DTexture9* itemAll;
	IDirect3DTexture9* mainMenu;
	IDirect3DTexture9* shortcutMenu;
	ID3DXSprite* pSprite;
	ID3DXFont* numText;
	D3DXVECTOR3 vertex1;	//3*3方格组左上角坐标
	D3DXVECTOR3 vertex2;	//合成方格左上角坐标
	D3DXVECTOR3 vertex3;	
	D3DXVECTOR3 vertex4;
	D3DXVECTOR3 vertex5;
	float unit;
};