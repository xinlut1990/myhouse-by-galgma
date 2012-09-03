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

//shader中顶点格式
const D3DVERTEXELEMENT9 g_VertexElemHardware[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	D3DDECL_END()
};

//方块顶点几何数据
struct BLOCK_VERTEX
{
	D3DXVECTOR3 pos;    // Position of the vertex
	D3DXVECTOR3 norm;   // Normal at this vertex
	float u;
	float v;
};

//方块实例信息
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
	IDirect3DTexture9* texArray;//方块的纹理
	IDirect3DTexture9* arrowTex;
	IDirect3DVertexBuffer9* handBuffer;
	IDirect3DVertexBuffer9* blockInstance;//实例的顶点缓冲
	IDirect3DVertexBuffer9* blockBuffer;//顶点缓冲
	IDirect3DIndexBuffer9* blockIndex;//索引缓冲
	IDirect3DVertexBuffer9* blocketInstance;//小方块实例的顶点缓冲
	IDirect3DVertexBuffer9* blocketBuffer;//小方块顶点缓冲
	IDirect3DIndexBuffer9* blocketIndex;//小方块索引缓冲
	IDirect3DVertexShader9* instShader;//着色器
	ID3DXConstantTable* instConstTable;//shader 的常数表
	IDirect3DVertexDeclaration9*    vertexDeclHardware;//shader 中的顶点声明
	ID3DXFont* pfont;
	ID3DXFont* infoFont;
	ID3DXMesh* hand;

	D3DXHANDLE viewMatrixHandle;//视图矩阵
	D3DXHANDLE viewProjMatrixHandle;//视图投影矩阵
	D3DXHANDLE ambientMtrlHandle;//环境光材质
	D3DXHANDLE diffuseMtrlHandle;//漫反射材质
	D3DXHANDLE lightDirHandle;//灯光方向
	D3DXHANDLE torchPosHandle;//火炬位置表
	D3DXHANDLE torchNumHandle;//火炬个数
	D3DXHANDLE playerPosHandle;//摄像机位置
	D3DXHANDLE selPosHandle;//选择块的位置
	D3DXHANDLE tmUHandle;//纹理映射U偏移
	D3DXHANDLE tmVHandle;//纹理映射V偏移
	D3DXMATRIX Proj;//投影矩阵

	D3DXVECTOR4 torchPos[20];
	int torchNum;

	float terrainMappingU[20];//纹理映射U偏移（下标的块类型对应值的偏移量）
	float terrainMappingV[20];//纹理映射V偏移

	visibleBlock visibleBlocks[(MAX_X+1)*MAX_Y*MAX_Z];
    visibleBlock blockets[100];
    int xBound[MAX_X+1];

	int blockNum;//可见块个数
	int blocketNum;//小方块个数

	int selX;
	int selY;
	int selZ;

	int curX;
	int curY;
	int curZ;

	LPD3DXSPRITE arrowSprite;


	bool iPress;




};