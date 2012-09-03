#include "stdafx.h"
#include "UI.h"

UI::UI()
{
	itemAll = 0;
	unit = 57.6;
	vertex1 = D3DXVECTOR3(381,169,0);
	vertex2 = D3DXVECTOR3(680,230,0);
//	vertex3 = D3DXVECTOR3(410,321,0);
	vertex3 = D3DXVECTOR3(310,380.25,0);
	//vertex3 = D3DXVECTOR3(891.3,697.8,0);
	vertex4 = D3DXVECTOR3(310,567,0);
	vertex5 = D3DXVECTOR3(231,685,0);

	itemPosInTexture[STONE].x = 0;
	itemPosInTexture[STONE].y = 0;
	itemPosInTexture[COAL].x = 8;
	itemPosInTexture[COAL].y = 1;
	itemPosInTexture[WOOD].x = 0;
	itemPosInTexture[WOOD].y = 2;
	itemPosInTexture[GOLD].x = 6;
	itemPosInTexture[GOLD].y = 1;
	itemPosInTexture[IRON].x = 7;
	itemPosInTexture[IRON].y = 1;
	itemPosInTexture[SAND].x = 4;
	itemPosInTexture[SAND].y = 1;
	itemPosInTexture[LEAF].x = 5;
	itemPosInTexture[LEAF].y = 5;
	itemPosInTexture[TORCH].x = 4;
	itemPosInTexture[TORCH].y = 8;
	itemPosInTexture[PLANK].x = 4;
	itemPosInTexture[PLANK].y = 0;
	itemPosInTexture[SOIL].x = 2;
	itemPosInTexture[SOIL].y = 0;
	itemPosInTexture[BRICK].x = 1;
	itemPosInTexture[BRICK].y = 7;
	itemPosInTexture[SANDSTONE].x = 0;
	itemPosInTexture[SANDSTONE].y = 3;
	itemPosInTexture[IRONBLOCK].x = 4;
	itemPosInTexture[IRONBLOCK].y = 6;
	itemPosInTexture[GOLDBLOCK].x = 3;
	itemPosInTexture[GOLDBLOCK].y = 6;

	inventory = new Equipment();
	shortcutfocus = 0;
	mX = 0;
	mY = 0;
}

UI::~UI()
{
	Merge::destructRules();
	d3d::Release<IDirect3DTexture9*>(itemAll);
	d3d::Release<IDirect3DTexture9*>(mainMenu);
	d3d::Release<IDirect3DTexture9*>(shortcutMenu);
	d3d::Release<ID3DXSprite*>(pSprite);
	d3d::Release<ID3DXFont*>(numText);
}

void UI::UIInitialize(IDirect3DDevice9* device)
{
	//HRESULT hr = 0;
	Merge::initializeRules();
    selectedItem.item = EMPTY;
	selectedItem.count = 0;
	D3DXCreateFont(device, 16, 8, 4, 0, false, 
		DEFAULT_CHARSET, 
		OUT_TT_ONLY_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE,
		L"Comic Sans MS",
		&numText);

	D3DXCreateTextureFromFile(device,_T("crafting.png"),&mainMenu);
	D3DXCreateTextureFromFile(device,_T("gui.png"),&shortcutMenu);
	D3DXCreateTextureFromFile(device,_T("itemlist.png"),&itemAll);

	D3DXCreateSprite(device,&pSprite);


}

void UI::drawSelectedItem(IDirect3DDevice9* device)
{
	if (selectedItem.item == EMPTY)
	{
		return;
	}
	int x = itemPosInTexture[selectedItem.item].x;
	int y = itemPosInTexture[selectedItem.item].y;
	RECT pSrcRect = {x * iwidth,y * iheight,( x + 1 ) * iwidth,( y + 1 ) * iheight};
	D3DXVECTOR3 pos = D3DXVECTOR3(mX - 25, mY - 20, 0);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->Draw(itemAll,&pSrcRect,NULL,&pos,0xffffffff);
	pSprite->End();
	if (selectedItem.count == 1)
	{
		return;
	}
	else
	{
		wchar_t* num = new wchar_t[3];
		_itow_s(selectedItem.count,num,3,10);
		int offset = 35;
		RECT textRect = {pos.x + offset, pos.y + offset + 15, pos.x + offset + 30, pos.y + offset + 45};
		if (num[1]=='\0')
		{
			numText->DrawTextW(NULL, num, 1, &textRect, 0, 0xffffffff);
		}
		else if (num[2]=='\0')
		{
			numText->DrawTextW(NULL, num, 2, &textRect, 0, 0xffffffff);
		}
		else
		{
			numText->DrawTextW(NULL, num, 3, &textRect, 0, 0xffffffff);
		}
		
		delete[] num;
	}
}

void UI::drawAnItemS(IDirect3DDevice9 *device, int type, int col, int number)
{
	if (type == EMPTY)
	{
		return;
	}
	int x = itemPosInTexture[type].x;
	int y = itemPosInTexture[type].y;
	RECT pSrcRect = {x * iwidth,y * iheight,( x + 1 ) * iwidth,( y + 1 ) * iheight};
	D3DXVECTOR3 pos = vertex5;
	pos.x += col * 64;
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->Draw(itemAll,&pSrcRect,NULL,&pos,0xffffffff);
	pSprite->End();
	if (number == 1)
	{
		return;
	}
	else
	{
		wchar_t* num = new wchar_t[3];
		_itow_s(number,num,3,10);
		int offset = 35;
		RECT textRect = {pos.x + offset, pos.y + offset + 15, pos.x + offset + 30, pos.y + offset + 45};
		if (num[1]=='\0')
		{
			numText->DrawTextW(NULL, num, 1, &textRect, 0, 0xffffffff);
		}
		else if (num[2]=='\0')
		{
			numText->DrawTextW(NULL, num, 2, &textRect, 0, 0xffffffff);
		}
		else
		{
			numText->DrawTextW(NULL, num, 3, &textRect, 0, 0xffffffff);
		}
		delete[] num;
	}
}
void UI::drawAnItem(IDirect3DDevice9 *device, int type, int row, int col, D3DXVECTOR3* origin, int number)
{
	if (type == EMPTY)
	{
		return;
	}
	int x = itemPosInTexture[type].x;
	int y = itemPosInTexture[type].y;
	RECT pSrcRect = {x * iwidth,y * iheight,( x + 1 ) * iwidth,( y + 1 ) * iheight};
	D3DXVECTOR3 pos = *origin;
	pos.x += col * unit;
	pos.y += row * unit;
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->Draw(itemAll,&pSrcRect,NULL,&pos,0xffffffff);
	pSprite->End();
	if (number == 1)
	{
		return;
	}
	else
	{
		wchar_t* num = new wchar_t[3];
		_itow_s(number,num,3,10);
		int offset = 35;
		RECT textRect = {pos.x + offset, pos.y + offset + 15, pos.x + offset + 30, pos.y + offset + 45};
		if (num[1]=='\0')
		{
			numText->DrawTextW(NULL, num, 1, &textRect, 0, 0xffffffff);
		}
		else if (num[2]=='\0')
		{
			numText->DrawTextW(NULL, num, 2, &textRect, 0, 0xffffffff);
		}
		else
		{
			numText->DrawTextW(NULL, num, 3, &textRect, 0, 0xffffffff);
		}
		delete[] num;
	}
}

void UI::drawItems(IDirect3DDevice9 *device)
{


 	D3DXMATRIX identity;
 	D3DXMatrixIdentity( &identity );
 	pSprite->SetTransform(&identity);
    FieldBlock** invenItems = inventory->getWholeFields();
	//绘制装备栏
	for (int i = 1; i < FIELD_LINES; i++)
	{
		for (int j = 0; j < FIELD_ROWS; j++)
		{
			drawAnItem(device,invenItems[i][j].item, i - 1, j, &vertex3, invenItems[i][j].count);
		}
	}
	//绘制快捷栏
	for (int j = 0; j < FIELD_ROWS; j++)
	{
		drawAnItem(device,invenItems[0][j].item, 0, j, &vertex4, invenItems[0][j].count);
	}
	//绘制合成栏
    MergeField* mergeItems = merge.getWholeItems();
	int cnt = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			drawAnItem(device,mergeItems->input[cnt], i, j, &vertex1, 1);
			cnt ++;
		}
	}
	drawAnItem(device,mergeItems->output.item, 0, 0, &vertex2, mergeItems->output.count);

	drawSelectedItem(device);
}

void UI::drawInventory(IDirect3DDevice9* device)
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity( &identity );
	D3DXMatrixScaling(&identity,3.2f,3.2f,1.0f);
	pSprite->SetTransform(&identity);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->Draw(mainMenu,NULL,NULL,&D3DXVECTOR3(90.0f, 40.0f, 0.0f),0xffffffff);
	pSprite->End();
	drawItems(device);

}

void UI::drawShortcutMenu(IDirect3DDevice9* device)
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity( &identity );
	D3DXMatrixScaling(&identity,3.2f,3.2f,1.0f);
	pSprite->SetTransform(&identity);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	RECT menuRect = {0,0,182,22};
	pSprite->Draw(shortcutMenu, &menuRect,NULL,&D3DXVECTOR3(70.0f, 215.0f, 0.0f), 0xffffffff);
	RECT focusRect = {0,22,24,46};
	pSprite->Draw(shortcutMenu, &focusRect, NULL, &D3DXVECTOR3(69 + 20 * shortcutfocus, 214.0f, 0.0f), 0xffffffff);
	pSprite->End();

	D3DXMatrixIdentity( &identity );
	pSprite->SetTransform(&identity);
	FieldBlock** invenItems = inventory->getWholeFields();
	//绘制快捷栏
	for (int j = 0; j < FIELD_ROWS; j++)
	{
		drawAnItemS(device, invenItems[0][j].item, j, invenItems[0][j].count);
	}
	
}
void UI::setShortcutFocus(int num)
{
	shortcutfocus = num;
}

int UI::getItemInHand()
{
	if(inventory->setHandingItem(0,shortcutfocus))
	{
		FieldBlock fb = inventory->useHandingItem();
		return fb.item;
	}
	else
	{
		return EMPTY;
	}
}

bool UI::selectItem(int mX, int mY, bool takeAll)
{
	POINT point = {mX, mY};
	RECT rect1 = {vertex1.x, vertex1.y, vertex1.x + 3 * unit, vertex1.y + 3 * unit};
	RECT rect2 = {vertex2.x, vertex2.y, vertex2.x + unit, vertex2.y + unit};
	RECT rect3 = {vertex3.x, vertex3.y, vertex3.x + FIELD_ROWS * unit, vertex3.y + (FIELD_LINES - 1) * unit};
	RECT rect4 = {vertex4.x, vertex4.y, vertex4.x + FIELD_ROWS * unit, vertex4.y + unit};

	if (selectedItem.item == EMPTY)
	{
		if (pointInRect(rect1, point))
		{
			int col = (mX - vertex1.x) / unit;
			int row = (mY - vertex1.y) / unit;
			selectedItem = merge.getInputItem(col + 3 * row);
			merge.startMerge();
			return true;
		}
		else if (pointInRect(rect2, point))
		{
			selectedItem = merge.getOutputItem();
			if (selectedItem.item != EMPTY)
			{
				merge.resetMerge();
			}
			return true;
		}
		else if (pointInRect(rect3, point))
		{
			int col = (mX - vertex3.x) / unit;
			int row = (mY - vertex3.y) / unit + 1;
			if (takeAll)
			{
				selectedItem = inventory->getItemByLeftStick(row, col);
			}
			else
			{
				selectedItem = inventory->getItemByRightStick(row, col);
			}
			return true;
		}
		else if (pointInRect(rect4, point))
		{
			int col = (mX - vertex4.x) / unit;
			int row = 0;
			if (takeAll)
			{
				selectedItem = inventory->getItemByLeftStick(row, col);
			}
			else
			{
				selectedItem = inventory->getItemByRightStick(row, col);
			}
			return true;
		}
		return false;
	}
	else
	{
		RECT recti = {290, 130, 850, 650};
		if (pointInRect(rect1, point))
		{
			int col = (mX - vertex1.x) / unit;
			int row = (mY - vertex1.y) / unit;
			selectedItem = merge.putItem(row * 3 + col, selectedItem);
			merge.startMerge();
			return true;
		}
		else if (pointInRect(rect3, point))
		{
			int col = (mX - vertex3.x) / unit;
			int row = (mY - vertex3.y) / unit + 1;
			selectedItem = inventory->setItem(row, col, selectedItem);
			return true;
		}
		else if (pointInRect(rect4, point))
		{
			int col = (mX - vertex4.x) / unit;
			int row = 0;
			selectedItem = inventory->setItem(row, col, selectedItem);
			return true;
		}
		else if (!pointInRect(recti, point))
		{
			selectedItem.item = EMPTY;
			return false;
		}
		return false;
	}
	
}

void UI::updateMousePos(int mX, int mY)
{
	this->mX = mX;
	this->mY = mY;
}

bool UI::pointInRect(RECT rect, POINT point)
{
	if(point.x > rect.left && point.x < rect.right && point.y < rect.bottom && point.y > rect.top)
		return true;
	else 
		return false;
}