#include "stdafx.h"
#include "d3dUtility.h"
//#include "psystem.h"
#include "camera.h"
#include <cstdlib>
#include <ctime>
#include <atltypes.h>
#include <vector>
#include <set>
#include <iterator>
#include "terrain.h"
#include <math.h>


using namespace std;

#define GET_TYPE(x) (x & 31)
#define EDGE 1
#define LIMIT 3
//
// Globals
//


IDirect3DDevice9*     Device = 0; 
IDirectSoundBuffer8*  walkSound = 0;

//�������ʼ��
Camera TheCamera(Camera::LANDOBJECT,20,20*SCALE+2.5,20);

bool isJump = false;
terrain g_world;//��Ϸ��ͼ

static int cxClient,cyClient;

POINT cur_pos;
POINT last_pos;

//��ǰλ��
int cur_x = 0;
int cur_z = 0;
int cur_y = 0;

//ѡ��Ŀ�
float tempX = 0;
float tempY = 0;
float tempZ = 0;

float floor_y = 0;//���
float ceil_y = 0;//�컨��

float v = 0;//y�����˵��ٶȣ�����Ϊ��
float a = 15;//���ٶ�

float timer = 0;

bool iPress = false;	//�Ƿ���i��


struct Ray
{
	D3DXVECTOR3 vOrigin;
	D3DXVECTOR3 vDirection;
};

//ͨ���������Ļ�ϵ������������
Ray CalcPickingRay(IDirect3DDevice9 *device,int x,int y)
{
	float px=0.0f;
	float py=0.0f;
	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	D3DXMATRIX proj;
	device->GetTransform(D3DTS_PROJECTION,&proj);
	px=(((2.0f*x)/(float)vp.Width)-1.0f)/proj(0,0);
	py=(((-2.0f*y)/(float)vp.Height)+1.0f)/proj(1,1);

	Ray ray;
	ray.vOrigin = D3DXVECTOR3(0.0f,0.0f,0.0f);
	ray.vDirection = D3DXVECTOR3(px,py,1.0f);

	return ray;
}

//�任����
void TransformRay(Ray* ray,D3DXMATRIX* T)
{
	D3DXVec3TransformCoord(&ray->vOrigin,&ray->vOrigin,T);
	D3DXVec3TransformNormal(&ray->vDirection,&ray->vDirection,T);

	D3DXVec3Normalize(&ray->vDirection,&ray->vDirection);
}

float getAbs(float num)
{
	if (num<0)
	{
		num = -num;
	}
	return num;
}

//�ж�ʰȡ�Ŀ�����
bool Picking(D3DXVECTOR3* p, D3DXVECTOR3* d, char*** maps)
{
	tempX = (p->x);
	tempY = (p->y);
	tempZ = (p->z);

	int mX, mY, mZ;

	float XtempY = (d->x)*(p->y);
	float XtempZ = (d->x)*(p->z);
	float YtempX = (d->y)*(p->x);
	float YtempZ = (d->y)*(p->z);
	float ZtempX = (d->z)*(p->x);
	float ZtempY = (d->z)*(p->y);

	if (getAbs(d->y)<=getAbs(d->x))
	{
		for (int i=0; i<LIMIT; i++)
		{
			if ((d->x)>=0)
			{
				tempX++;
				XtempY += (d->y);
			}
			else
			{
				tempX--;
				XtempY -= (d->y);
			}
			if (getAbs(d->z)<=getAbs(d->x))
			{
				if ((d->x)>=0)
				{
					XtempZ += (d->z);
				}
				else
				{
					XtempZ -= (d->z);
				}	
			}
			else
			{
				for(int j=0; j<LIMIT; j++)
				{
					if ((d->z)>=0)
					{
						tempZ++;
						ZtempX += (d->x);
						ZtempY += (d->y);
					}
					else
					{
						tempZ--;
						ZtempX -= (d->x);
						ZtempY -= (d->y);
					}
					if ((d->z) != 0)
					{
						tempX = ZtempX/(d->z);
						tempY = ZtempY/(d->z);
					}
					if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
					{
						return false;
					}
					mX = (int)(tempX);
					mY = (int)(tempY);
					mZ = (int)(tempZ);

					if (GET_TYPE(maps[mX][mY][mZ])!=0)
					{
						return true;
					}
				}
				return false;
			}
			if ((d->x) != 0)
			{
				tempY = XtempY/(d->x);
				tempZ = XtempZ/(d->x);
			}
			if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
			{
				return false;
			}
			mX = (int)(tempX);
			mY = (int)(tempY);
			mZ = (int)(tempZ);

			if (GET_TYPE(maps[mX][mY][mZ])!=0)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for (int i=0; i<LIMIT; i++)
		{
			if ((d->y)>=0)
			{
				tempY++;
				YtempX += (d->x);
			}
			else
			{
				tempY--;
				YtempX -= (d->x);
			}
			if (getAbs(d->z)<=getAbs(d->y))
			{
				if ((d->y)>=0)
				{
					YtempZ += (d->z);
				}
				else
				{
					YtempZ -= (d->z);
				}
			}
			else
			{
				for(int j=0; j<LIMIT; j++)
				{
					if ((d->z)>=0)
					{
						tempZ++;
						ZtempX += (d->x);
						ZtempY += (d->y);
					}
					else
					{
						tempZ--;
						ZtempX -= (d->x);
						ZtempY -= (d->y);
					}
					if ((d->z) != 0)
					{
						tempX = ZtempX/(d->z);
						tempY = ZtempY/(d->z);
					}
					if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
					{
						return false;
					}
					mX = (int)(tempX);
					mY = (int)(tempY);
					mZ = (int)(tempZ);

					if (GET_TYPE(maps[mX][mY][mZ])!=0)
					{
						return true;
					}
				}
				return false;
			}
			if ((d->y) != 0)
			{
				tempX = YtempX/(d->y);
				tempZ = YtempZ/(d->y);
			}
			if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
			{
				return false;
			}
			mX = (int)(tempX);
			mY = (int)(tempY);
			mZ = (int)(tempZ);

			if (GET_TYPE(maps[mX][mY][mZ])!=0)
			{
				return true;
			}
		}
		return false;
	}
}
int getSurface(D3DXVECTOR3* p, D3DXVECTOR3* d, char* blockPos)
{
	float fx = (float)blockPos[0];
	float fy = (float)blockPos[1];
	float fz = (float)blockPos[2];
	float bx = fx+1;
	float by = fy+1;
	float bz = fz+1;
	float _sur[8] = {};
	float tempX = 0;
	float tempY = 0;
	float tempZ = 0;
	float k1 = 0;
	float k2 = 0;
	int i = 0;

	while (1)
	{
		if ((d->x)!=0)
		{
			k1 = (d->y)/(d->x);
			k2 = (d->z)/(d->x);
			tempY = k1*(fx-(p->x))+(p->y);
			tempZ = k2*(fx-(p->x))+(p->z);
			if (tempY<=by && tempY>=fy && tempZ<=bz && tempZ>=fz)
			{
				_sur[i++] = fx;
				_sur[i++] = tempY;
				_sur[i++] = tempZ;
				_sur[i++] = 1;
				if (i == 8)
				{
					break;
				}
			}
			tempY = k1*(bx-(p->x))+(p->y);
			tempZ = k2*(bx-(p->x))+(p->z);
			if (tempY<=by && tempY>=fy && tempZ<=bz && tempZ>=fz)
			{
				_sur[i++] = bx;
				_sur[i++] = tempY;
				_sur[i++] = tempZ;
				_sur[i++] = 4;
				if (i == 8)
				{
					break;
				}
			}
		}
		if ((d->y)!=0)
		{
			k1 = (d->x)/(d->y);
			k2 = (d->z)/(d->y);
			tempX = k1*(fy-(p->y))+(p->x);
			tempZ = k2*(fy-(p->y))+(p->z);
			if (tempX<=bx && tempX>=fx && tempZ<=bz && tempZ>=fz)
			{
				_sur[i++] = tempX;
				_sur[i++] = fy;
				_sur[i++] = tempZ;
				_sur[i++] = 2;
				if (i == 8)
				{
					break;
				}
			}
			tempX = k1*(by-(p->y))+(p->x);
			tempZ = k2*(by-(p->y))+(p->z);
			if (tempX<=bx && tempX>=fx && tempZ<=bz && tempZ>=fz)
			{
				_sur[i++] = tempX;
				_sur[i++] = by;
				_sur[i++] = tempZ;
				_sur[i++] = 5;
				if (i == 8)
				{
					break;
				}
			}
		}
		if ((d->z)!=0)
		{
			k1 = (d->x)/(d->z);
			k2 = (d->y)/(d->z);
			tempX = k1*(fz-(p->z))+(p->x);
			tempY = k2*(fz-(p->z))+(p->y);
			if (tempX<=bx && tempX>=fx && tempY<=by && tempY>=fy)
			{
				_sur[i++] = tempX;
				_sur[i++] = tempY;
				_sur[i++] = fz;
				_sur[i++] = 3;
				if (i == 8)
				{
					break;
				}
			}
			tempX = k1*(bz-(p->z))+(p->x);
			tempY = k2*(bz-(p->z))+(p->y);
			if (tempX<=bx && tempX>=fx && tempY<=by && tempY>=fy)
			{
				_sur[i++] = tempX;
				_sur[i++] = tempY;
				_sur[i++] = bz;
				_sur[i++] = 6;
				if (i == 8)
				{
					break;
				}
			}
		}
	}

	if ((d->x)!=0)
	{
		if ((d->x)>0 && _sur[0]<_sur[4] || (d->x)<0 && _sur[0]>_sur[4])
		{
			return _sur[3];
		}
		else
		{
			return _sur[7];
		}
	}
	else if ((d->y)!=0)
	{
		if ((d->y)>0 && _sur[1]<_sur[5] || (d->y)<0 && _sur[1]>_sur[5])
		{
			return _sur[3];
		}
		else
		{
			return _sur[7];
		}
	}
	else if ((d->z)!=0)
	{
		if ((d->z)>0 && _sur[2]<_sur[6] || (d->z)<0 && _sur[2]>_sur[6])
		{
			return _sur[3];
		}
		else
		{
			return _sur[7];
		}
	}
}

int surfacePicking(D3DXVECTOR3* p, D3DXVECTOR3* d, char***maps)
{
	//return 0		no picking
	//return 1		x-1 y z
	//return 2		x y-1 z	bottom surface, cant put on
	//return 3		x y z-1
	//return 4		x+1 y z
	//return 5		x y+1 z
	//return 6		x y z+1

	tempX = (p->x);
	float tempY = (p->y);
	float tempZ = (p->z);

	int mX, mY, mZ;

	float XtempY = (d->x)*(p->y);
	float XtempZ = (d->x)*(p->z);
	float YtempX = (d->y)*(p->x);
	float YtempZ = (d->y)*(p->z);
	float ZtempX = (d->z)*(p->x);
	float ZtempY = (d->z)*(p->y);

	if (getAbs(d->y)<=getAbs(d->x))
	{
		for (int i=0; i<LIMIT; i++)
		{
			if ((d->x)>=0)
			{
				tempX++;
				XtempY += (d->y);
			}
			else
			{
				tempX--;
				XtempY -= (d->y);
			}
			if (getAbs(d->z)<=getAbs(d->x))
			{
				if ((d->x)>=0)
				{
					XtempZ += (d->z);
				}
				else
				{
					XtempZ -= (d->z);
				}	
			}
			else
			{
				for(int j=0; j<LIMIT; j++)
				{
					if ((d->z)>=0)
					{
						tempZ++;
						ZtempX += (d->x);
						ZtempY += (d->y);
					}
					else
					{
						tempZ--;
						ZtempX -= (d->x);
						ZtempY -= (d->y);
					}
					if ((d->z) != 0)
					{
						tempX = ZtempX/(d->z);
						tempY = ZtempY/(d->z);
					}
					if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
					{
						return 0;
					}
					mX = (int)(tempX);
					mY = (int)(tempY);
					mZ = (int)(tempZ);

					if (GET_TYPE(maps[mX][mY][mZ])!=0)
					{
						char blockPos[3]={mX, mY, mZ};
						return getSurface(p, d, blockPos);
					}
				}
				return 0;
			}
			if ((d->x) != 0)
			{
				tempY = XtempY/(d->x);
				tempZ = XtempZ/(d->x);
			}
			if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
			{
				return 0;
			}
			mX = (int)(tempX);
			mY = (int)(tempY);
			mZ = (int)(tempZ);

			if (GET_TYPE(maps[mX][mY][mZ])!=0)
			{
				char blockPos[3]={mX, mY, mZ};
				return getSurface(p, d, blockPos);
			}
		}
		return false;
	}
	else
	{
		for (int i=0; i<LIMIT; i++)
		{
			if ((d->y)>=0)
			{
				tempY++;
				YtempX += (d->x);
			}
			else
			{
				tempY--;
				YtempX -= (d->x);
			}
			if (getAbs(d->z)<=getAbs(d->y))
			{
				if ((d->y)>=0)
				{
					YtempZ += (d->z);
				}
				else
				{
					YtempZ -= (d->z);
				}
			}
			else
			{
				for(int j=0; j<LIMIT; j++)
				{
					if ((d->z)>=0)
					{
						tempZ++;
						ZtempX += (d->x);
						ZtempY += (d->y);
					}
					else
					{
						tempZ--;
						ZtempX -= (d->x);
						ZtempY -= (d->y);
					}
					if ((d->z) != 0)
					{
						tempX = ZtempX/(d->z);
						tempY = ZtempY/(d->z);
					}
					if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
					{
						return 0;
					}
					mX = (int)(tempX);
					mY = (int)(tempY);
					mZ = (int)(tempZ);

					if (GET_TYPE(maps[mX][mY][mZ])!=0)
					{
						char blockPos[3]={mX, mY, mZ};
						return getSurface(p, d, blockPos);
					}
				}
				return false;
			}
			if ((d->y) != 0)
			{
				tempX = YtempX/(d->y);
				tempZ = YtempZ/(d->y);
			}
			if ((tempX<1 || tempX>MAX_X) || (tempY<1 || tempY>MAX_Y) || (tempZ<1 || tempZ>MAX_Z))
			{
				return 0;
			}
			mX = (int)(tempX);
			mY = (int)(tempY);
			mZ = (int)(tempZ);

			if (GET_TYPE(maps[mX][mY][mZ])!=0)
			{
			    char blockPos[3]={mX, mY, mZ};
				return getSurface(p, d, blockPos);
			}
		}
		return false;
	}
}
//
// Framework Functions
//
bool Setup()
{
	g_world.initialize(Device);
	return true;
}

void Cleanup()
{

	g_world.initialize(0);				//gai
	d3d::Release<IDirectSoundBuffer8*>(walkSound);
	d3d::Release<IDirectSound8*>(g_world.g_pDS);
}

bool Display(float timeDelta)
{

	if( Device )
	{
		//
		// Update the scene:
		//
		if (!iPress)
		{
			SetCursorPos(cxClient/2,cyClient/2);			//�������ڴ��ڿͻ����м�
		}

		D3DXVECTOR3 my_pos;
		TheCamera.getPosition(&my_pos);

        cur_x = (int)(my_pos.x);
        cur_z = (int)(my_pos.z);
		cur_y = (int)(my_pos.y);

		g_world.setCurrentPos(cur_x, cur_y - 1, cur_z);

		for(int j = cur_y > MAX_Y ? MAX_Y : cur_y - 0.5; j > 0; j--)//Ѱ�ҵ�ǰx��z�����µ���ߵ㣬��Ϊ��Ծ�����
		{
			if (GET_TYPE(g_world.cube.blocks[cur_x][j][cur_z]) != EMPTY)
			{
				floor_y = j;
				break;
			}
			
		}

		if (isJump)//���������Ծ���ж�ͷ�᲻����������
		{
			if (ceil_y != MAX_Y+3)
			{
				if (GET_TYPE(g_world.cube.blocks[cur_x][cur_y][cur_z]))
				{
					ceil_y = cur_y;
				}
				else if (GET_TYPE(g_world.cube.blocks[cur_x][cur_y + 1][cur_z]))
				{
					ceil_y = cur_y + 1;
				}
				else if (GET_TYPE(g_world.cube.blocks[cur_x][cur_y + 2][cur_z]))
				{
					ceil_y = cur_y + 2;
				}
				else
				{
					ceil_y = MAX_Y+3;
				}
			}
		}	

		if (isJump && my_pos.y > ceil_y)
		{
			v = -v;
		}

		if(my_pos.y > floor_y + 2.5 || v > 0)//������ڵ�ƽ���ϻ����ٶȴ����㣨����ʱ���ٶȵ���λ��û�䣩
		{
			v -= a * timeDelta;//�ٶȱ仯
			my_pos.y = my_pos.y + timeDelta*v;//λ�ñ仯
			TheCamera.setPosition(&my_pos);//���������λ��
		}
		else//�����أ�������Ծ���ٶ���Ϊ��
		{
			isJump = false;
			v = 0;
			my_pos.y = floor_y + 2.5;
			TheCamera.setPosition(&my_pos);//���������λ��
		}

		GetCursorPos(&last_pos);
		D3DXVECTOR3 newPos;
		int new_x;
		int new_y;
		int new_z;
		float step = 4.0f;//�ƶ�����
		float deviation = 0.3f;
		float playerHeight = 1.5f;
		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			TheCamera.pitch(1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			TheCamera.pitch(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			TheCamera.yaw(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			TheCamera.yaw(1.0f * timeDelta);

		if((::GetAsyncKeyState('A') & 0x8000f)||(::GetAsyncKeyState('W') & 0x8000f)||(::GetAsyncKeyState('D') & 0x8000f)||(::GetAsyncKeyState('S') & 0x8000f))
		{
			//�߶�ҡ��
			timer += timeDelta;
			TheCamera.yaw(0.005 * sin(timer * 10));
			TheCamera.roll(0.005 * sin(timer * 10));
			walkSound->Play(0,0,0);
		}
		if( (::GetAsyncKeyState('A') & 0x8000f))
		{
			//0.2��ƫ���������ڵ��������λ�õ����
			TheCamera.strafe(-step * timeDelta - deviation);//�ƶ�
			TheCamera.getPosition(&newPos);
			new_x = (int)(newPos.x);
			new_y = (int)(newPos.y - playerHeight);
			new_z = (int)(newPos.z);//�ƶ����λ��
			if(GET_TYPE(g_world.cube.blocks[new_x][new_y][new_z]) || GET_TYPE(g_world.cube.blocks[new_x][new_y+1][new_z]))//����ƶ�����ײ����һ��
				TheCamera.strafe(step * timeDelta + deviation);//�����ƶ�
			else
				TheCamera.strafe(deviation);
		}

		if( (::GetAsyncKeyState('D') & 0x8000f) )
		{
			TheCamera.strafe(step * timeDelta + deviation);
			TheCamera.getPosition(&newPos);
			new_x = (int)(newPos.x);
			new_y = (int)(newPos.y - playerHeight);
			new_z = (int)(newPos.z);
			if(GET_TYPE(g_world.cube.blocks[new_x][new_y][new_z]) || GET_TYPE(g_world.cube.blocks[new_x][new_y+1][new_z]))
				TheCamera.strafe(-step * timeDelta - deviation);
			else
				TheCamera.strafe(-deviation);
		}

		if( (::GetAsyncKeyState('W') & 0x8000f))
		{
			TheCamera.walk(step * timeDelta + 0.3);
			TheCamera.getPosition(&newPos);
			new_x = (int)(newPos.x);
			new_y = (int)(newPos.y - playerHeight);
			new_z = (int)(newPos.z);
			if(GET_TYPE(g_world.cube.blocks[new_x][new_y][new_z]) || GET_TYPE(g_world.cube.blocks[new_x][new_y+1][new_z]))
				TheCamera.walk(-step * timeDelta - deviation);
			else
				TheCamera.walk(-deviation);
		}

		if( (::GetAsyncKeyState('S') & 0x8000f) )
		{
			TheCamera.walk(-step * timeDelta - deviation);
			TheCamera.getPosition(&newPos);
			new_x = (int)(newPos.x);
			new_y = (int)(newPos.y - playerHeight);
			new_z = (int)(newPos.z);
			if(GET_TYPE(g_world.cube.blocks[new_x][new_y][new_z]) || GET_TYPE(g_world.cube.blocks[new_x][new_y+1][new_z]))
				TheCamera.walk(step * timeDelta + deviation);
			else
				TheCamera.walk(deviation);
		}

		g_world.render(Device,TheCamera,1.0f);

	}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Ray ray;
	D3DXMATRIX view;
	D3DXMATRIX viewInverse;
	int surfaceNum = 0;
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_CHAR:
		switch(wParam)
		{
		case VK_SPACE:
			if (!isJump)
			{
				v = 7;//7
				isJump = true;
			}
			break;
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);				//�õ����ڿͻ�����С
		cyClient = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		if (wParam == 0x49)						//�ж�i���Ƿ���
		{
			iPress = !iPress;
			g_world.setPress(iPress);
			if (iPress)
		 	{
				HCURSOR   hc;
				hc=LoadCursor(NULL,IDC_HAND);
				SetCursor(hc);
			 	ShowCursor(true);
			}
			 else
		    {
			     ShowCursor(false);
			}
		}
		if (wParam >= 0x31 && wParam <= 0x39)
		{
			int num = wParam - 0x31;
			g_world.setShortcutFocus(num);
		}
		break;
	case WM_KEYUP:
		
		if(walkSound != NULL)
		{
			walkSound->Stop();
		}
		break;
	case WM_LBUTTONDOWN:
		if (iPress)
		{
			GetCursorPos(&cur_pos);
			ScreenToClient(hwnd, &cur_pos);
			if(g_world.itemBox.selectItem(cur_pos.x, cur_pos.y, true))
				PlaySND("click.wav", &g_world.g_pDS);

		}
		else
		{
			if(tempX != -1)
				g_world.remove = true;
		}
		break;
	case WM_RBUTTONDOWN:
		if (iPress)
		{
			GetCursorPos(&cur_pos);
			ScreenToClient(hwnd, &cur_pos);
			if(g_world.itemBox.selectItem(cur_pos.x, cur_pos.y, false))
				PlaySND("click.wav", &g_world.g_pDS);
		}
		else
		{
			//ʰȡ����

			ray = CalcPickingRay(Device, Width / 2, Height / 2);
			Device->GetTransform(D3DTS_VIEW,&view);
			D3DXMatrixInverse(&viewInverse,0,&view);
			TransformRay(&ray,&viewInverse);
			surfaceNum = surfacePicking(&ray.vOrigin, &ray.vDirection, g_world.cube.blocks);
			if (!surfaceNum)
			{
				tempX = tempY = tempZ = -1;
			}
			g_world.setSelectedBlock(tempX,tempY,tempZ);
			g_world.surfaceNum = surfaceNum;
			if(tempX != -1)
				g_world.build = true;
		}
		break;
	case WM_MOUSEMOVE:
		//�������ת
		GetCursorPos(&cur_pos);

		if (iPress)
		{
			ScreenToClient(hwnd, &cur_pos);
			g_world.itemBox.updateMousePos(cur_pos.x, cur_pos.y);
		}
		else
		{
			D3DXVECTOR3* vUp=new D3DXVECTOR3();
			TheCamera.getUp(vUp);
			float deltaX = (float)(cur_pos.x - last_pos.x) / 100;
			float deltaY = (float)(cur_pos.y - last_pos.y) / 100;
			if (deltaX > 0)
			{
				TheCamera.yaw(deltaX);
			}
			else if (deltaX < 0)
			{
				TheCamera.yaw(deltaX);
			}
			if (deltaY < 0 )
			{
				TheCamera.pitch(deltaY);
			}
			else if (deltaY > 0 )
			{
				TheCamera.pitch(deltaY);
			}	
			last_pos = cur_pos;

			//ʰȡ����
			ray = CalcPickingRay(Device, Width / 2, Height / 2);
			Device->GetTransform(D3DTS_VIEW,&view);
			D3DXMatrixInverse(&viewInverse,0,&view);
			TransformRay(&ray,&viewInverse);
			if (!Picking(&ray.vOrigin, &ray.vDirection, g_world.cube.blocks))
			{
				tempX = tempY = tempZ = -1;
			}
			g_world.setSelectedBlock(tempX,tempY,tempZ);
			break;
		}
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{

	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, _T("InitD3D() - FAILED"), 0, 0);
		return 0;
	}

	if(!Setup())
	{
		::MessageBox(0, _T("Setup() - FAILED"), 0, 0);
		return 0;
	}
	GetCursorPos(&last_pos);
    ShowCursor(false);
    PlayLoop("walk.wav",&g_world.g_pDS,walkSound);
	walkSound->Stop();
	d3d::EnterMsgLoop( Display );
	Cleanup();

	Device->Release();

	return 0;
}

