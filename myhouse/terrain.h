#pragma once
#include "sound1.h"
#include <d3dx9.h>
#include <vector>
#include "d3dUtility.h"
#include "block.h"
#include "camera.h"
#include <math.h>
#include "UI.h"
#include "equipment.h"

// const int X = 10;
// const int Y = 10;
// const int Z = 10;
const int Width  = 1024;
const int Height = 768;

#define MYFVF (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

//shader�ж����ʽ
const D3DVERTEXELEMENT9 g_VertexElemHardware[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	D3DDECL_END()
};

//���鶥�㼸������
struct BLOCK_VERTEX
{
	D3DXVECTOR3 pos;    // Position of the vertex
	D3DXVECTOR3 norm;   // Normal at this vertex
	float u;
	float v;
};

//����ʵ����Ϣ
struct BLOCK_VERTEX_INSTANCE
{
	float x;
	float y;
	float z;
	float type;  // Box instance index

};

struct visibleBlock
{
	int x;
	int y;
	int z;
	char type;  
};

class terrain
{
public:
	terrain();
	//int getBlock(int ***block);
	void render(IDirect3DDevice9* device,Camera& camera,float scale);
	void initialize(IDirect3DDevice9* device);
	void drawblocks(IDirect3DDevice9* device);
	void drawblockets(IDirect3DDevice9* device);
	void fillInstData(IDirect3DDevice9* device);
	void fillBlocketInst(IDirect3DDevice9* device);
	void getShaderConstants();
	char getVisible(int x, int y, int z);
	void addVisible(int x, int y, int z);
	void setVisible(int x, int y, int z);
	void addBlock(int x, int y, int z, char type, IDirect3DDevice9* device);
	void removeBlock();
	void buildBlock(char type, IDirect3DDevice9* device);
	void setSelectedBlock(int x, int y, int z);
	void setCurrentPos(int x, int y, int z);
	bool shadowPicking(D3DXVECTOR3 p, char*** maps);
	void updateShadow(int x, int y, int z, char visible);
	void updateSurroundVisible(int x, int y, int z);
	void updateVisible(int x, int y, int z);
	void blocketCollision(int curX, int curY, int curZ, IDirect3DDevice9* device);
	block cube;
	void createFace(BLOCK_VERTEX* pVtx,WORD* indices,int faceIndex,D3DXVECTOR3 vNormal,D3DXVECTOR3 vUp, D3DXVECTOR3 vCenter);
	bool getPress();
	void setPress(bool press);
	void setShortcutFocus(int num);
	bool remove;
	bool build;
	int surfaceNum;
	IDirectSound8* g_pDS;
	UI itemBox;

private:

	IDirectSoundBuffer8* g_pDSBuffer;
	IDirect3DTexture9* texArray;//���������
	IDirect3DTexture9* arrowTex;
	IDirect3DVertexBuffer9* handBuffer;
	IDirect3DVertexBuffer9* blockInstance;//ʵ���Ķ��㻺��
	IDirect3DVertexBuffer9* blockBuffer;//���㻺��
	IDirect3DIndexBuffer9* blockIndex;//��������
	IDirect3DVertexBuffer9* blocketInstance;//С����ʵ���Ķ��㻺��
	IDirect3DVertexBuffer9* blocketBuffer;//С���鶥�㻺��
	IDirect3DIndexBuffer9* blocketIndex;//С������������
	IDirect3DVertexShader9* instShader;//��ɫ��
	ID3DXConstantTable* instConstTable;//shader �ĳ�����
	IDirect3DVertexDeclaration9*    vertexDeclHardware;//shader �еĶ�������
	ID3DXFont* pfont;
	ID3DXFont* infoFont;
	ID3DXMesh* hand;

	D3DXHANDLE viewMatrixHandle;//��ͼ����
	D3DXHANDLE viewProjMatrixHandle;//��ͼͶӰ����
	D3DXHANDLE ambientMtrlHandle;//���������
	D3DXHANDLE diffuseMtrlHandle;//���������
	D3DXHANDLE lightDirHandle;//�ƹⷽ��
	D3DXHANDLE torchPosHandle;//���λ�ñ�
	D3DXHANDLE torchNumHandle;//������
	D3DXHANDLE playerPosHandle;//�����λ��
	D3DXHANDLE selPosHandle;//ѡ����λ��
	D3DXHANDLE tmUHandle;//����ӳ��Uƫ��
	D3DXHANDLE tmVHandle;//����ӳ��Vƫ��
	D3DXMATRIX Proj;//ͶӰ����

	D3DXVECTOR4 torchPos[20];
	int torchNum;

	float terrainMappingU[20];//����ӳ��Uƫ�ƣ��±�Ŀ����Ͷ�Ӧֵ��ƫ������
	float terrainMappingV[20];//����ӳ��Vƫ��

	visibleBlock visibleBlocks[(MAX_X+1)*MAX_Y*MAX_Z];
    visibleBlock blockets[100];
    int xBound[MAX_X+1];

	int blockNum;//�ɼ������
	int blocketNum;//С�������

	int selX;
	int selY;
	int selZ;

	int curX;
	int curY;
	int curZ;

	LPD3DXSPRITE arrowSprite;


	bool iPress;




};