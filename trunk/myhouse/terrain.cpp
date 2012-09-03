#include "stdafx.h"
#include "terrain.h"
#include "d3dUtility.h"
//#include <vector>
using namespace std;
#define GET_TYPE(x) (x &  31)
#define GET_SHADOW(x) (x & 32)

terrain::terrain()
{
	//纹理坐标映射表
	terrainMappingU[STONE] = 1.0 / 16;
	terrainMappingV[STONE] = 0;
	terrainMappingU[GOLD] = 0;
	terrainMappingV[GOLD] = 2.0 / 16;
	terrainMappingU[IRON] = 1.0 / 16;
	terrainMappingV[IRON] = 2.0 / 16;
	terrainMappingU[COAL] = 2.0 / 16;
	terrainMappingV[COAL] = 2.0 / 16;
	terrainMappingU[SAND] = 2.0 / 16;
	terrainMappingV[SAND] = 1.0 / 16;
	terrainMappingU[LEAF] = 1.0 / 16;
	terrainMappingV[LEAF] = 9.0 / 16;
	terrainMappingU[BOTTOM] = 1.0 / 16;
	terrainMappingV[BOTTOM] = 1.0 / 16;
	terrainMappingU[SOIL] = 3.0 / 16;
	terrainMappingV[SOIL] = 0;
	terrainMappingU[SOIL + 1] = 2.0 / 16;
	terrainMappingV[SOIL + 1] = 0;
	terrainMappingU[SOIL + 2] = 2.0 / 16;
	terrainMappingV[SOIL + 2] = 9.0 / 16;
	terrainMappingU[WOOD] = 4.0 / 16;
	terrainMappingV[WOOD] = 1.0 / 16;
	terrainMappingU[WOOD + 1] = 5.0 / 16;
	terrainMappingV[WOOD + 1] = 1.0 / 16;
	terrainMappingU[TORCH] = 8.0 / 16;
	terrainMappingV[TORCH] = 1.0 / 16;
	terrainMappingU[PLANK] = 4.0 / 16;
	terrainMappingV[PLANK] = 0.0 / 16;
	terrainMappingU[SANDSTONE] = 0;
	terrainMappingV[SANDSTONE] = 11.0 / 16;
	terrainMappingU[BRICK] = 7.0 / 16;
	terrainMappingV[BRICK] = 0;
	terrainMappingU[IRONBLOCK] = 6.0 / 16;
	terrainMappingV[IRONBLOCK] = 1.0 / 16;
	terrainMappingU[GOLDBLOCK] = 7.0 / 16;
	terrainMappingV[GOLDBLOCK] = 1.0 / 16;

	//各种初始化
	g_pDS = 0;
	g_pDSBuffer = 0;
	texArray = 0;
	arrowTex = 0;
	blockBuffer = 0;
	blockInstance = 0;
	blockIndex = 0;
	instShader = 0;
	instConstTable = 0;
	vertexDeclHardware = 0;
	pfont = 0;

	viewMatrixHandle = 0;
	viewProjMatrixHandle = 0;
	ambientMtrlHandle = 0;
	diffuseMtrlHandle = 0;
	lightDirHandle = 0;
	tmUHandle = 0;
	tmVHandle = 0;
	playerPosHandle = 0;
	selPosHandle = 0;

	torchNum = 0;
	blockNum = 0;
	blocketNum = 2;
	selX = 0;
	selY = 0;
	selZ = 0;
	curX = 0;
	curY = 0;
	curZ = 0;
    remove = false;
	build = false;
	surfaceNum = 0;

	blockets[0].x = 0;
	blockets[0].y = -5;
	blockets[0].z = 0;
	blockets[0].type = EMPTY;
	blockets[1].x = 0;
	blockets[1].y = -5;
	blockets[1].z = 0;
	blockets[1].type = EMPTY;

	for (int i = 1; i<=MAX_X; i++)
	{
		xBound[i] = i * MAX_Z * 20;
	}

	cube.initialization();
}



void terrain::initialize(IDirect3DDevice9* device)			//渲染地图
{


	HRESULT hr = 0;

	if( device == 0 )
	{
		d3d::Release<IDirect3DTexture9*>(texArray);
		d3d::Release<IDirect3DTexture9*>(arrowTex);
		d3d::Release<IDirect3DVertexBuffer9*>(blockBuffer);
		d3d::Release<IDirect3DVertexBuffer9*>(blockInstance);
		d3d::Release<IDirect3DIndexBuffer9*>(blockIndex);
		d3d::Release<IDirect3DVertexBuffer9*>(blocketBuffer);
		d3d::Release<IDirect3DVertexBuffer9*>(blocketInstance);
		d3d::Release<IDirect3DIndexBuffer9*>(blocketIndex);
		d3d::Release<IDirect3DVertexShader9*>(instShader);
		d3d::Release<ID3DXConstantTable*>(instConstTable);
		d3d::Release<IDirect3DVertexDeclaration9*>(vertexDeclHardware);
		d3d::Release<ID3DXFont*>(pfont);
		d3d::Release<ID3DXFont*>(infoFont);
		d3d::Release<IDirectSoundBuffer8*>(g_pDSBuffer);

	}
	else
	{
		HRESULT hr = 0;
		ID3DXBuffer* shader = 0;
		ID3DXBuffer* errorBuffer = 0;
        //顶点着色器初始化
		hr = D3DXCompileShaderFromFileW(L"diffuse.txt",0,0,"Main","vs_3_0",D3DXSHADER_DEBUG,&shader,&errorBuffer,&instConstTable);
		
		if( errorBuffer )
		{
			::MessageBox(0, (LPCTSTR)errorBuffer->GetBufferPointer(), 0, 0);
			d3d::Release<ID3DXBuffer*>(errorBuffer);
		}

		if(FAILED(hr))
		{
			::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
			return ;
		}

        hr = device->CreateVertexShader((DWORD*)shader->GetBufferPointer(), &instShader);

		if(FAILED(hr))
		{
			::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
			return ;
		}

		d3d::Release<ID3DXBuffer*>(shader);

		device->CreateVertexDeclaration(g_VertexElemHardware,&vertexDeclHardware);

// 		BLOCK_VERTEX* hand = 0;
// 		device->CreateVertexBuffer(36 * sizeof(BLOCK_VERTEX),0,MYFVF,D3DPOOL_MANAGED,&handBuffer,0);
// 		handBuffer->Lock(0,36 * sizeof(BLOCK_VERTEX),(void**)&hand,0);
// 		hand[0].pos = D3DXVECTOR3(-1,-1,1);
// 		hand[1].pos = D3DXVECTOR3(-1,1,1);
// 		hand[2].pos = D3DXVECTOR3(1,1,1);
// 		hand[3].pos = D3DXVECTOR3(-1,-1,1);
// 		hand[4].pos = D3DXVECTOR3(1,1,1);
// 		hand[5].pos = D3DXVECTOR3(1,-1,1);
// 		handBuffer->Unlock();
		//创建几何数据
		//D3DXCreateBox(device, 10,10,10,&hand,0);
		BLOCK_VERTEX* v_cube = 0;
		WORD* indices = 0;
		float len = 1;
		float halflen = len / 2;

		device->CreateVertexBuffer(24 * sizeof(BLOCK_VERTEX),0,0,D3DPOOL_MANAGED,&blockBuffer,0);
		device->CreateIndexBuffer(36 * sizeof(WORD),0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&blockIndex,0);

		blockBuffer->Lock(0,24 * sizeof(BLOCK_VERTEX),(void**)&v_cube,0);
		blockIndex->Lock(0,36 * sizeof(WORD),(void**)&indices,0);

		//front
		createFace(v_cube,indices,0,D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,1,0),D3DXVECTOR3(0,0,-halflen));
		//back
		createFace(v_cube, indices,1,D3DXVECTOR3(0,0,1),D3DXVECTOR3(0,1,0),D3DXVECTOR3(0,0,halflen));
		//right
		createFace(v_cube, indices,2,D3DXVECTOR3(1,0,0),D3DXVECTOR3(0,1,0),D3DXVECTOR3(halflen,0,0));
		//left
		createFace(v_cube, indices,3,D3DXVECTOR3(-1,0,0),D3DXVECTOR3(0,1,0),D3DXVECTOR3(-halflen,0,0));
		//down
		createFace(v_cube, indices,4,D3DXVECTOR3(0,-1,0),D3DXVECTOR3(1,0,0),D3DXVECTOR3(0,-halflen,0));
		//top
		createFace(v_cube, indices,5,D3DXVECTOR3(0,1,0),D3DXVECTOR3(1,0,0),D3DXVECTOR3(0,halflen,0));

		blockIndex->Unlock();
		blockBuffer->Unlock();

		v_cube = 0;
		indices = 0;
		len = 0.2;
		halflen = len / 2;

		device->CreateVertexBuffer(24 * sizeof(BLOCK_VERTEX),0,0,D3DPOOL_MANAGED,&blocketBuffer,0);
		device->CreateIndexBuffer(36 * sizeof(WORD),0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&blocketIndex,0);

		blocketBuffer->Lock(0,24 * sizeof(BLOCK_VERTEX),(void**)&v_cube,0);
		blocketIndex->Lock(0,36 * sizeof(WORD),(void**)&indices,0);

		//front
		createFace(v_cube,indices,0,D3DXVECTOR3(0,0,-1),D3DXVECTOR3(0,len,0),D3DXVECTOR3(0,-0.2,-halflen));
		//back
		createFace(v_cube, indices,1,D3DXVECTOR3(0,0,1),D3DXVECTOR3(0,len,0),D3DXVECTOR3(0,-0.2,halflen));
		//right
		createFace(v_cube, indices,2,D3DXVECTOR3(1,0,0),D3DXVECTOR3(0,len,0),D3DXVECTOR3(halflen,-0.2,0));
		//left
		createFace(v_cube, indices,3,D3DXVECTOR3(-1,0,0),D3DXVECTOR3(0,len,0),D3DXVECTOR3(-halflen,-0.2,0));
		//down
		createFace(v_cube, indices,4,D3DXVECTOR3(0,-1,0),D3DXVECTOR3(len,0,0),D3DXVECTOR3(0,-halflen - 0.2,0));
		//top
		createFace(v_cube, indices,5,D3DXVECTOR3(0,1,0),D3DXVECTOR3(len,0,0),D3DXVECTOR3(0,halflen - 0.2,0));

		blocketIndex->Unlock();
		blocketBuffer->Unlock();

		//创建实例数据
		blockNum = 0;

		int c = 0;
		for (int i = 1;i<=MAX_X;i++)
		{
			for (int j = 1;j<=MAX_Y;j++)
			{
				for (int k = 1;k<=MAX_Z;k++)
				{
					if (shadowPicking(D3DXVECTOR3(i, j, k),cube.blocks))
					{
						cube.blocks[i][j][k] += SHADOWED;
						c++;
					}
					//判断块是否可见，如果可见，填入顶点缓冲
					addVisible(i, j, k);
				}
			}
		}

		fillInstData(device);

		getShaderConstants();
		//
		// Set shader constants:
		//

        //设定纹理坐标映射表
		instConstTable->SetFloatArray(device, tmUHandle, terrainMappingU, 20);
		instConstTable->SetFloatArray(device, tmVHandle, terrainMappingV, 20);

		// Light direction:
		D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);
		instConstTable->SetVector(device, lightDirHandle, &directionToLight);

		instConstTable->SetVectorArray(device, torchPosHandle, torchPos, torchNum);
        instConstTable->SetInt(device, torchNumHandle, torchNum);
		// 创建材质:
		D3DXVECTOR4 ambientMtrl(1.0f, 1.0f, 1.0f, 0.0f);
		D3DXVECTOR4 diffuseMtrl(1.0f, 1.0f, 1.0f, 0.0f);
		instConstTable->SetVector(device,ambientMtrlHandle,&ambientMtrl);
		instConstTable->SetVector(device,diffuseMtrlHandle,&diffuseMtrl);
		instConstTable->SetDefaults(device);

		//
		// Pre-Render Setup
		//
        
		//开启雾效
 		device->SetRenderState(D3DRS_FOGENABLE, TRUE);
 		device->SetRenderState(D3DRS_FOGCOLOR, 0xffffffff);

		//加载纹理
		D3DXCreateTextureFromFile(device,_T("terrain.png"),&texArray);
		D3DXCreateTextureFromFile(device,_T("arrow.png"),&arrowTex);
		device->SetTexture(0,texArray);
		//多级纹理过滤
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

        //背面裁剪加视锥裁剪
		device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		device->SetRenderState(D3DRS_CLIPPING,TRUE);

		//创建投影矩阵
		D3DXMatrixPerspectiveFovLH(
			&Proj,
			D3DX_PI / 3.0f, // 45 - degree
			(float)Width / (float)Height,
			0.1f,
			100.0f);

		//创建字体
		D3DXCreateFont(device, 40, 20, 2, 0, false, 
			DEFAULT_CHARSET, 
			OUT_TT_ONLY_PRECIS, 
			DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE,
			L"ariel",
			&pfont);

		D3DXCreateFont(device, 20, 10, 2, 0, false, 
			DEFAULT_CHARSET, 
			OUT_TT_ONLY_PRECIS, 
			DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE,
			L"Comic Sans MS",
			&infoFont);

		//创建声音接口,并设定协作级别
		if(FAILED(DirectSoundCreate8(NULL, &g_pDS, NULL))) {
			MessageBox(0,L"Unable to create DirectSound object",L"Warning!",MB_OK);
			return;
		}
		HWND hWnd = FindWindow(_T("Direct3D9App"), _T("Direct3D9App"));
		g_pDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
		//播放背景音乐
		PlayLoop("summer.wav",&g_pDS,g_pDSBuffer);
		PlayLoop("bird.wav",&g_pDS,g_pDSBuffer);

		itemBox.UIInitialize(device);
	}
}
		//
		// Render
		//
void terrain::render(IDirect3DDevice9 *device,Camera& camera, float scale)
{
	D3DXMATRIX V;
	camera.getViewMatrix(&V);
	device->SetTransform(D3DTS_VIEW, &V);
	instConstTable->SetMatrix(device, viewMatrixHandle, &V);

	D3DXMATRIX ViewProj = V * Proj;
	instConstTable->SetMatrix(device, viewProjMatrixHandle, &ViewProj);

	D3DXVECTOR3 playerPos3;
	camera.getPosition(&playerPos3);
	D3DXVECTOR4 playerPos;
	playerPos.x = playerPos3.x;
	playerPos.y = playerPos3.y;
	playerPos.z = playerPos3.z;
	playerPos.w = 1;
	instConstTable->SetVector(device, playerPosHandle, &playerPos);

	D3DXVECTOR4 selPos;
	selPos.x = selX;
	selPos.y = selY;
	selPos.z = selZ;
	selPos.w = 1;
	instConstTable->SetVector(device, selPosHandle, &selPos);

	blocketCollision(curX, curY, curZ, device);

	if (remove)
	{
        if (GET_TYPE(cube.blocks[selX][selY][selZ]) != BOTTOM)
        {
			PlaySND("dig.wav",&g_pDS);

			if (GET_TYPE(cube.blocks[selX][selY][selZ]) == TORCH)
			{
				for (int i = 0; i < torchNum; i++)
				{
					if (torchPos[i].x == selX + 0.5 && torchPos[i].y == selY + 0.5 && torchPos[i].z == selZ + 0.5)
					{
						torchNum--;
						torchPos[i] = torchPos[torchNum];
						instConstTable->SetVectorArray(device, torchPosHandle, torchPos, torchNum);
						instConstTable->SetInt(device, torchNumHandle, torchNum);
					}
				}
			}

			blockets[blocketNum].x = selX;
			int ground = selY;
			for (int i = selY - 1; i > 0; i --)
			{
				if(GET_TYPE(cube.blocks[selX][i][selZ]) != EMPTY)
				{
					ground = i + 1;
					break;
				}
			}
			blockets[blocketNum].y = ground;
			blockets[blocketNum].z = selZ;
			blockets[blocketNum].type = GET_TYPE(cube.blocks[selX][selY][selZ]);
			blocketNum ++;
			for (int i = 0; i < blocketNum;i ++)
		    {
				if(blockets[i].x == selX && blockets[i].z == selZ && blockets[i].y > ground)
					blockets[i].y = ground;
		    }
			fillBlocketInst(device);
			removeBlock();
			d3d::Release<IDirect3DVertexBuffer9*>(blockInstance);
			fillInstData(device);
        }

		remove = false;
	}

	if (build)
	{
		int type = itemBox.getItemInHand();
		if (selY<MAX_Y - 2 && type != EMPTY)
		{
			PlaySND("build.wav",&g_pDS);
			buildBlock(type,device);
			d3d::Release<IDirect3DVertexBuffer9*>(blockInstance);
			fillInstData(device);
		}

		build = false;
	}

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00ffffff, 1.0f, 0);
	device->BeginScene();

    drawblocks(device);

	D3DXMATRIX W;
	static float y = 0.0f;
	D3DXMatrixTranslation(&W,0,0.05 * sin(y),0);
	y += 0.1;
	camera.getViewMatrix(&V);
	//device->SetTransform(D3DTS_VIEW, &V);
	V = W * V;
	instConstTable->SetMatrix(device, viewMatrixHandle, &V);

	ViewProj = V * Proj;
	instConstTable->SetMatrix(device, viewProjMatrixHandle, &ViewProj);

	drawblockets(device);

// 	D3DXMATRIX w ;
// 	D3DXMATRIX v ;
// 	D3DXVECTOR3 pos;
// 	camera.getPosition(&pos);
// 	D3DXMatrixTranslation(&w,pos.x+5,pos.y,pos.z+5);
//     device->SetTransform(D3DTS_WORLD,&w);
//  	camera.getViewMatrix(&v);
//  	device->SetTransform(D3DTS_VIEW,&v);
// 	D3DXMATRIX p = Proj ;
// 	device->SetTransform(D3DTS_PROJECTION,&p);
// 	device->SetStreamSource(0, handBuffer, 0, sizeof(BLOCK_VERTEX));
// 	//device->SetVertexDeclaration(vertexDeclHardware);
// 	device->SetVertexShader(NULL);
// 	//device->SetIndices(blockIndex);
// 	device->SetFVF(MYFVF);
// 
// 	device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

	RECT rect;
	rect.bottom = Height;
	rect.top = 0;
	rect.left = 0;
	rect.right = Width;

	pfont->DrawTextW(NULL, L"+",1,&rect,DT_CENTER | DT_VCENTER,0xff000000);
	infoFont->DrawTextW(NULL, L"My House v1.0 By GALGMA",23,&rect, DT_TOP | DT_LEFT, 0xff000000);

	if (iPress)
	{
		itemBox.drawInventory(device);
	}

	itemBox.drawShortcutMenu(device);
// 	//if(selX != -1)
// 	{
// 		D3DXCreateSprite( device, &arrowSprite );
// 		D3DXVECTOR3 arrowPos;
// 		arrowPos.x = selX + 0.5;
// 		arrowPos.y = selY + 3;
// 		arrowPos.z = selZ + 0.5;
//  		arrowSprite->Begin( D3DXSPRITE_ALPHABLEND ) ;
//  		arrowSprite->Draw( arrowTex, &rect, NULL,NULL, 0xffffffff );
//  		arrowSprite->End();
// 	}

	device->EndScene();
	device->Present(0, 0, 0, 0);
	    
}

//创建一个面的顶点数据
void terrain::createFace(BLOCK_VERTEX* pVtx,WORD* indices, int faceIndex,D3DXVECTOR3 vNormal,D3DXVECTOR3 vUp, D3DXVECTOR3 vCenter)
{
	D3DXVECTOR3 vRight;
	D3DXVec3Cross( &vRight, &vNormal, &vUp );

	vRight *= 0.5;
	vUp *= 0.5;
	pVtx[faceIndex*4].pos = vCenter - vRight - vUp;
	pVtx[faceIndex*4+1].pos = vCenter - vRight + vUp;
	pVtx[faceIndex*4+2].pos = vCenter + vRight + vUp;
	pVtx[faceIndex*4+3].pos = vCenter + vRight - vUp;

    pVtx[faceIndex*4].norm = vNormal;
	pVtx[faceIndex*4+1].norm = vNormal;
	pVtx[faceIndex*4+2].norm = vNormal;
	pVtx[faceIndex*4+3].norm = vNormal;

	const float uvDist = 1.0 / 16;
	pVtx[faceIndex*4].u = 0;
	pVtx[faceIndex*4].v = uvDist - 0 ;
	pVtx[faceIndex*4+1].u = 0;
	pVtx[faceIndex*4+1].v = 0;
	pVtx[faceIndex*4+2].u = uvDist - 0;
	pVtx[faceIndex*4+2].v = 0;
	pVtx[faceIndex*4+3].u = uvDist - 0;
	pVtx[faceIndex*4+3].v = uvDist - 0;

	indices[faceIndex*6] = (WORD)(faceIndex * 4);
	indices[faceIndex*6+1] = (WORD)(faceIndex * 4 + 1);
	indices[faceIndex*6+2] = (WORD)(faceIndex * 4 + 2);
	indices[faceIndex*6+3] = (WORD)(faceIndex * 4);
	indices[faceIndex*6+4] = (WORD)(faceIndex * 4 + 2);
	indices[faceIndex*6+5] = (WORD)(faceIndex * 4 + 3);

}

//画方块
void terrain::drawblocks(IDirect3DDevice9* device)
{
	// Set up the geometry data stream
	device->SetStreamSourceFreq(0,(D3DSTREAMSOURCE_INDEXEDDATA | blockNum));
	device->SetStreamSource(0, blockBuffer, 0, sizeof(BLOCK_VERTEX));

	// Set up the instance data stream
	device->SetStreamSourceFreq(1,(D3DSTREAMSOURCE_INSTANCEDATA | 1));
	device->SetStreamSource(1, blockInstance, 0, sizeof(BLOCK_VERTEX_INSTANCE));

	device->SetVertexDeclaration(vertexDeclHardware);
	device->SetVertexShader(instShader);
	device->SetIndices(blockIndex);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,24,0,12);

	device->SetStreamSourceFreq(0,1);
	device->SetStreamSourceFreq(1,1);
}

void terrain::drawblockets(IDirect3DDevice9* device)
{
	// Set up the geometry data stream
	device->SetStreamSourceFreq(0,(D3DSTREAMSOURCE_INDEXEDDATA | blocketNum));
	device->SetStreamSource(0, blocketBuffer, 0, sizeof(BLOCK_VERTEX));

	// Set up the instance data stream
	device->SetStreamSourceFreq(1,(D3DSTREAMSOURCE_INSTANCEDATA | 1));
	device->SetStreamSource(1, blocketInstance, 0, sizeof(BLOCK_VERTEX_INSTANCE));

	device->SetVertexDeclaration(vertexDeclHardware);
	device->SetVertexShader(instShader);
	device->SetIndices(blocketIndex);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,24,0,12);

	device->SetStreamSourceFreq(0,1);
	device->SetStreamSourceFreq(1,1);
}

//设定选择的块坐标
void terrain::setSelectedBlock(int x, int y, int z)
{
	selX = x;
	selY = y;
	selZ = z;
}

void terrain::setCurrentPos(int x, int y, int z)
{
	curX = x;
	curY = y;
	curZ = z;
}

char terrain::getVisible(int x, int y, int z)
{
	if (x == 0 || y == 0 || z == 0 || x == MAX_X + 1 || y == MAX_Y + 1 || z == MAX_Z + 1)
	{
        return 1;
	}
	return !GET_TYPE(cube.blocks[x][y][z])+
		GET_TYPE(cube.blocks[x+1][y][z])*
		GET_TYPE(cube.blocks[x-1][y][z])*
		GET_TYPE(cube.blocks[x][y+1][z])*
		GET_TYPE(cube.blocks[x][y-1][z])*
		GET_TYPE(cube.blocks[x][y][z+1])*
		GET_TYPE(cube.blocks[x][y][z-1]);
}

void terrain::addVisible(int x, int y, int z)
{
	char visible = getVisible(x, y, z);

	if (visible == 0 && GET_TYPE(cube.blocks[x][y][z]) != BORDER)
	{
		visibleBlocks[xBound[x]].type = cube.blocks[x][y][z];
		visibleBlocks[xBound[x]].x = x;
		visibleBlocks[xBound[x]].y = y;
		visibleBlocks[xBound[x]].z = z;

		xBound[x]++;
		blockNum++;
	}
}

void terrain::setVisible(int x, int y, int z)
{
	visibleBlocks[xBound[x]].type = cube.blocks[x][y][z];
	visibleBlocks[xBound[x]].x = x;
	visibleBlocks[xBound[x]].y = y;
	visibleBlocks[xBound[x]].z = z;

	xBound[x]++;
	blockNum++;
}

void terrain::addBlock(int x, int y, int z, char type, IDirect3DDevice9* device)
{
	if (GET_TYPE(cube.blocks[x][y][z]) == EMPTY)
	{
		visibleBlocks[xBound[x]].x = x;
		visibleBlocks[xBound[x]].y = y;
		visibleBlocks[xBound[x]].z = z;
		cube.blocks[x][y][z] = type;
		if (shadowPicking(D3DXVECTOR3(x , y, z),cube.blocks))
		{
			cube.blocks[x][y][z] += SHADOWED;
		}
		visibleBlocks[xBound[x]].type = cube.blocks[x][y][z];
		xBound[x]++;
		blockNum++;
		if (GET_TYPE(cube.blocks[x][y][z]) == TORCH)
		{
			torchPos[torchNum] = D3DXVECTOR4(x + 0.5, y + 0.5, z + 0.5, 1);
			torchNum++;
			instConstTable->SetVectorArray(device, torchPosHandle, torchPos, torchNum);
			instConstTable->SetInt(device, torchNumHandle, torchNum);
		}
	}
	else
	{
		itemBox.inventory->catchItems(FieldBlock(type,1));
	}
}

void terrain::buildBlock(char type, IDirect3DDevice9* device)
{
	switch (surfaceNum)
	{
	case 1:
		addBlock(selX - 1, selY, selZ, type, device);
		updateSurroundVisible(selX - 1, selY, selZ);
		break;
	case 2:
		addBlock(selX, selY - 1, selZ, type, device);
		updateSurroundVisible(selX, selY - 1, selZ);
		break;
	case 3:
		addBlock(selX, selY, selZ - 1, type, device);
		updateSurroundVisible(selX, selY, selZ - 1);
		break;
	case 4:
		addBlock(selX + 1, selY, selZ, type, device);
		updateSurroundVisible(selX + 1, selY, selZ);
		break;
	case 5:
		addBlock(selX, selY + 1, selZ, type, device);
		updateSurroundVisible(selX, selY + 1, selZ);
		break;
	case 6:
		addBlock(selX, selY, selZ + 1, type, device);
		updateSurroundVisible(selX, selY, selZ + 1);
		break;
	default:
		break;
	}
}

void terrain::removeBlock()
{
	for (int i = 1; i <= MAX_X; i++)
	{
		for (int j = i * MAX_Z * 20; j < xBound[i]; j++)
		{
			if(visibleBlocks[j].x == selX && visibleBlocks[j].y == selY && visibleBlocks[j].z == selZ)
			{
				xBound[i]--;
				visibleBlocks[j] = visibleBlocks[xBound[i]];
				blockNum--;

				break;
			}
		}
	}
	char v1 = getVisible(selX + 1, selY, selZ);
	char v2 = getVisible(selX - 1, selY, selZ);
	char v3 = getVisible(selX, selY + 1, selZ);
	char v4 = getVisible(selX, selY - 1, selZ);
	char v5 = getVisible(selX, selY, selZ + 1);
	char v6 = getVisible(selX, selY, selZ - 1);
	updateShadow(selX + 1, selY, selZ, v1);
	updateShadow(selX - 1, selY, selZ, v2);
	updateShadow(selX, selY + 1, selZ, v3);
	updateShadow(selX, selY - 1, selZ, v4);
	updateShadow(selX, selY, selZ + 1, v5);
	updateShadow(selX, selY, selZ - 1, v6);
	cube.blocks[selX][selY][selZ] = EMPTY;
	v1 = !v1 * getVisible(selX + 1, selY, selZ) + v1 * !getVisible(selX + 1, selY, selZ);
	v2 = !v2 * getVisible(selX - 1, selY, selZ) + v2 * !getVisible(selX - 1, selY, selZ);
	v3 = !v3 * getVisible(selX, selY + 1, selZ) + v3 * !getVisible(selX, selY + 1, selZ);
	v4 = !v4 * getVisible(selX, selY - 1, selZ) + v4 * !getVisible(selX, selY - 1, selZ);
	v5 = !v5 * getVisible(selX, selY, selZ + 1) + v5 * !getVisible(selX, selY, selZ + 1);
	v6 = !v6 * getVisible(selX, selY, selZ - 1) + v6 * !getVisible(selX, selY, selZ - 1);
	if (v1)
	{
		setVisible(selX + 1, selY, selZ);
	}
	if (v2)
	{
		setVisible(selX - 1, selY, selZ);
	}
	if (v3)
	{
		setVisible(selX, selY + 1, selZ);
	}
	if (v4)
	{
		setVisible(selX, selY - 1, selZ);
	}
	if (v5)
	{
		setVisible(selX, selY, selZ + 1);
	}
	if (v6)
	{
		setVisible(selX, selY, selZ - 1);
	}


}
//获得shader的常数句柄
void terrain::getShaderConstants()
{
	//获得shader中的常数
	viewMatrixHandle    = instConstTable->GetConstantByName(0, "ViewMatrix");
	viewProjMatrixHandle= instConstTable->GetConstantByName(0, "ViewProjMatrix");
	ambientMtrlHandle   = instConstTable->GetConstantByName(0, "AmbientMtrl");
	diffuseMtrlHandle   = instConstTable->GetConstantByName(0, "DiffuseMtrl");
	lightDirHandle      = instConstTable->GetConstantByName(0, "LightDirection");
	playerPosHandle      = instConstTable->GetConstantByName(0, "PlayerPos");
	selPosHandle         = instConstTable->GetConstantByName(0,"SelPos");
	torchPosHandle = instConstTable->GetConstantByName(0, "TorchPos");
	torchNumHandle = instConstTable->GetConstantByName(0, "TorchNum");
	tmUHandle      = instConstTable->GetConstantByName(0, "TerrainMappingU");
	tmVHandle      = instConstTable->GetConstantByName(0, "TerrainMappingV");
}

void terrain::fillInstData(IDirect3DDevice9* device)
{
	BLOCK_VERTEX_INSTANCE* inst = 0;
	device->CreateVertexBuffer(blockNum * sizeof(BLOCK_VERTEX_INSTANCE),0,0,D3DPOOL_MANAGED,&blockInstance,0);
	blockInstance->Lock(0,blockNum * sizeof(BLOCK_VERTEX_INSTANCE),(void**)&inst,0);
	int curinst = 0;
	for (int i = 1; i <= MAX_X; i++)
	{
		for (int j = i * MAX_Z * 20; j < xBound[i]; j++)
		{
			inst[curinst].type = visibleBlocks[j].type;
			inst[curinst].x = visibleBlocks[j].x;
			inst[curinst].y = visibleBlocks[j].y;
			inst[curinst].z = visibleBlocks[j].z;
			curinst++;
		}
	}
	blockInstance->Unlock();
}

void terrain::fillBlocketInst(IDirect3DDevice9* device)
{
    if (blocketNum > 1)
    {
		BLOCK_VERTEX_INSTANCE* inst = 0;
		device->CreateVertexBuffer(blocketNum * sizeof(BLOCK_VERTEX_INSTANCE),0,0,D3DPOOL_MANAGED,&blocketInstance,0);
		blocketInstance->Lock(0,blocketNum * sizeof(BLOCK_VERTEX_INSTANCE),(void**)&inst,0);
		int curinst = 0;
		for (int i = 0; i < blocketNum; i++)
		{
			inst[curinst].type = blockets[i].type;
			inst[curinst].x = blockets[i].x;
			inst[curinst].y = blockets[i].y;
			inst[curinst].z = blockets[i].z;
			curinst++;
		}
		blocketInstance->Unlock();
    }
}

bool terrain::shadowPicking(D3DXVECTOR3 p, char*** maps)
{
	//方向 -0.57f, 0.57f, -0.57f
	int tempX = p.x;
	int tempY = p.y;
	int tempZ = p.z;
	int i;
	for (i = 1; i<=MAX_X; i++)
	{
		if (tempX < 1 || tempZ < 1 || tempY > MAX_Y )
		{
			return false;
		}
		tempX--;
		tempY++;
		tempZ--;
		if (GET_TYPE(maps[tempX][tempY][tempZ])!=EMPTY && GET_TYPE(maps[tempX][tempY][tempZ])!=BORDER)
		{
			return true;
		}
	}
	return false;
}
void terrain::updateSurroundVisible(int x, int y, int z)
{
	updateVisible(x + 1, y, z);
	updateVisible(x - 1, y, z);
	updateVisible(x, y + 1, z);
	updateVisible(x, y - 1, z);
	updateVisible(x, y, z + 1);
	updateVisible(x, y, z - 1);
}

void terrain::updateVisible(int x, int y, int z)
{
	if(getVisible(x, y, z))
	{
		for (int i = 1; i <= MAX_X; i++)
		{
			for (int j = i * MAX_Z * 20; j < xBound[i]; j++)
			{
				if(visibleBlocks[j].x == x && visibleBlocks[j].y == y && visibleBlocks[j].z == z)
				{
					xBound[i]--;
					visibleBlocks[j] = visibleBlocks[xBound[i]];
					blockNum--;
					break;
				}
			}
		}
	}
}

void terrain::updateShadow(int x, int y, int z,char visible)
{
	if (shadowPicking(D3DXVECTOR3(x , y, z),cube.blocks) && !GET_SHADOW(cube.blocks[x][y][z]) && !visible)
	{
		cube.blocks[x][y][z] += SHADOWED;
		for (int i = 1; i <= MAX_X; i++)
		{
			for (int j = i * MAX_Z * 20; j < xBound[i]; j++)
			{
				if(visibleBlocks[j].x == x && visibleBlocks[j].y == y && visibleBlocks[j].z == z)
				{
					xBound[i]--;
					visibleBlocks[j] = visibleBlocks[xBound[i]];
					blockNum--;
					break;
				}
			}
		}
		setVisible(x,y,z);
	}
}

void terrain::blocketCollision(int curX, int curY, int curZ, IDirect3DDevice9* device)
{
	for (int i = 0; i < blocketNum;i ++)
	{
		if(blockets[i].x == curX && blockets[i].z == curZ && blockets[i].y == curY)
		{
			//放入装备栏
            if(itemBox.inventory->catchItems(FieldBlock(blockets[i].type,1)))
			{
				blocketNum --;
				blockets[i] = blockets[blocketNum];
				PlaySND("pop.wav",&g_pDS);
				fillBlocketInst(device);
			}
		}
	}
}

bool terrain::getPress()
{
	return iPress;
}

void terrain::setPress(bool press)
{
	iPress = press;
}

void terrain::setShortcutFocus(int num)
{
	itemBox.setShortcutFocus(num);
}