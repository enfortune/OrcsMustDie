#pragma once
#include "cGameNode.h"
#include "iGameMap.h"
#include "MapDataEnumList.h"

#define MAPSIZE_X 30
#define MAPSIZE_Z 30
#define MAPSIZE_Y  8
#define MAPSIZE_Y_ZEROSTD 4
#define MAPSIZE_CUBE 2

class cTrap;
class cHeightMap;
class cRay;

struct ST_TRAPBUILDDATA
{
	DIRECTION_6 enDir;
	std::vector<INDEX_XYZ> vecXYZ;
};

struct ST_GRIDBOX
{
	EN_GRIDBOXKIND enKind;
	ST_PTN_VERTEX v[36]; // 그리는 버텍스 (추후 버텍스 버퍼로 바꿀 것)
	LPDIRECT3DVERTEXBUFFER9 pVB;
	ST_FRUSTUM stCube;

	cTrap* pRight;  //  1  0  0
	cTrap* pLeft;	// -1  0  0
	cTrap* pTop;	//  0  1  0
	cTrap* pBottom; //  0 -1  0
	cTrap* pFront;  //  0  0  1
	cTrap* pRear;   //  0  0 -1

	ST_GRIDBOX()
	{
		enKind = GRIDBOXKIND_NONE;
		pRight = pLeft = pTop = pBottom = pFront = pRear = nullptr;
	}

	void MakeBox(int x, int y, int z, EN_GRIDBOXKIND kind = GRIDBOXKIND_DEFAULT)
	{
		assert((x >= 0 && x < MAPSIZE_X) && "x 인덱스가 제한을 벗어남");
		assert((y >= 0 && y < MAPSIZE_Y) && "y 인덱스가 제한을 벗어남");
		assert((z >= 0 && z < MAPSIZE_Z) && "z 인덱스가 제한을 벗어남");

		enKind = kind;

		D3DXVECTOR3 _000;
		D3DXVECTOR3 _100;
		D3DXVECTOR3 _110;
		D3DXVECTOR3 _010;
		D3DXVECTOR3 _001;
		D3DXVECTOR3 _101;
		D3DXVECTOR3 _111;
		D3DXVECTOR3 _011;

		_000 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, z* MAPSIZE_CUBE);
		_001 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
		_010 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z)* MAPSIZE_CUBE);
		_011 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
		_100 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD)* MAPSIZE_CUBE, (z)* MAPSIZE_CUBE);
		_101 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD)* MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
		_110 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z)* MAPSIZE_CUBE);
		_111 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);

		stCube.vNear_00 = _000;
		stCube.vNear_01 = _010;
		stCube.vNear_10 = _100;
		stCube.vNear_11 = _110;
		stCube.vFar_00 = _001;
		stCube.vFar_01 = _011;
		stCube.vFar_10 = _101;
		stCube.vFar_11 = _111;


		//뒤
		v[0].p = _000; v[0].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f); v[0].t = D3DXVECTOR2(0.f, 1.f);
		v[1].p = _010; v[1].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f); v[1].t = D3DXVECTOR2(0.f, 0.f);
		v[2].p = _110; v[2].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f); v[2].t = D3DXVECTOR2(1.f, 0.f);

		v[3].p = _000; v[3].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f); v[3].t = D3DXVECTOR2(0.f, 1.f);
		v[4].p = _110; v[4].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f); v[4].t = D3DXVECTOR2(1.f, 0.f);
		v[5].p = _100; v[5].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f); v[5].t = D3DXVECTOR2(1.f, 1.f);

		//앞
		v[6].p = _001; v[3].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f); v[6].t = D3DXVECTOR2(1.f, 1.f);
		v[7].p = _111; v[4].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f); v[7].t = D3DXVECTOR2(0.f, 0.f);
		v[8].p = _011; v[5].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f); v[8].t = D3DXVECTOR2(1.f, 0.f);

		v[9].p = _001;	v[9].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);  v[9].t = D3DXVECTOR2(1.f, 1.f);
		v[10].p = _101; v[10].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f); v[10].t = D3DXVECTOR2(0.f, 0.f);
		v[11].p = _111;	v[11].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f); v[11].t = D3DXVECTOR2(0.f, 1.f);

		//왼쪽
		v[12].p = _001;	v[12].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); v[12].t = D3DXVECTOR2(0.f, 1.f);
		v[13].p = _011; v[13].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); v[13].t = D3DXVECTOR2(0.f, 0.f);
		v[14].p = _010; v[14].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); v[14].t = D3DXVECTOR2(1.f, 1.f);

		v[15].p = _001; v[15].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); v[15].t = D3DXVECTOR2(0.f, 1.f);
		v[16].p = _010; v[16].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); v[16].t = D3DXVECTOR2(1.f, 1.f);
		v[17].p = _000; v[17].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); v[17].t = D3DXVECTOR2(1.f, 0.f);

		//오른쪽
		v[18].p = _100; v[18].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f); v[18].t = D3DXVECTOR2(0.f, 1.f);
		v[19].p = _110; v[19].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f); v[19].t = D3DXVECTOR2(0.f, 0.f);
		v[20].p = _111;  v[20].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f); v[20].t = D3DXVECTOR2(1.f, 0.f);

		v[21].p = _100; v[21].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  v[21].t = D3DXVECTOR2(0.f, 1.f);
		v[22].p = _111;  v[22].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f); v[22].t = D3DXVECTOR2(1.f, 0.f);
		v[23].p = _101; v[23].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  v[23].t = D3DXVECTOR2(1.f, 1.f);

		//위
		v[24].p = _010; v[24].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f); v[24].t = D3DXVECTOR2(0.f, 1.f);
		v[25].p = _011; v[25].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f); v[25].t = D3DXVECTOR2(0.f, 0.f);
		v[26].p = _111;	v[26].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f); v[26].t = D3DXVECTOR2(1.f, 0.f);

		v[27].p = _010; v[27].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  v[27].t = D3DXVECTOR2(0.f, 1.f);
		v[28].p = _111;  v[28].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f); v[28].t = D3DXVECTOR2(1.f, 0.f);
		v[29].p = _110; v[29].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  v[29].t = D3DXVECTOR2(1.f, 1.f);

		//아래
		v[30].p = _001;	v[30].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f); v[30].t = D3DXVECTOR2(1.f, 1.f);
		v[31].p = _000; v[31].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f); v[31].t = D3DXVECTOR2(1.f, 0.f);
		v[32].p = _100; v[32].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f); v[32].t = D3DXVECTOR2(0.f, 0.f);

		v[33].p = _001;  v[33].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f); v[33].t = D3DXVECTOR2(1.f, 1.f);
		v[34].p = _100; v[34].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);  v[34].t = D3DXVECTOR2(0.f, 0.f);
		v[35].p = _101; v[35].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);  v[35].t = D3DXVECTOR2(0.f, 1.f);
	}

	void RenderBox(LPDIRECT3DTEXTURE9 pTex, D3DMATERIAL9* pMtrl)
	{
		if (enKind == GRIDBOXKIND_NONE) return;

		if (pTex) g_pD3DDevice->SetTexture(0, pTex);
		if (pMtrl)g_pD3DDevice->SetMaterial(pMtrl);
		//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetFVF(ST_PTN_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			12,
			&v[0],
			sizeof(ST_PTN_VERTEX)
			);
		//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	void RenderVirtualBox()
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetFVF(ST_PTN_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			12,
			&v[0],
			sizeof(ST_PTN_VERTEX)
			);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	void CopyToVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pVB)
	{
		if (pVB)
		{
			ST_PTN_VERTEX* pV;
			pVB->Lock(0, sizeof(ST_PTN_VERTEX) * 36, (void**)&pV, 0);
			memcpy_s(pV, sizeof(ST_PTN_VERTEX) * 36, &v[0], sizeof(ST_PTN_VERTEX) * 36);
			pVB->Unlock();
		}
	}
};


class cMapData : public cGameNode, public iGameMap
{
protected:
	ST_GRIDBOX m_arrGridBox[MAPSIZE_X][MAPSIZE_Y][MAPSIZE_Z];

	bool m_bisVBExist;
	LPDIRECT3DVERTEXBUFFER9 m_arrVB[MAPSIZE_X][MAPSIZE_Y][MAPSIZE_Z];
	LPDIRECT3DTEXTURE9 m_arrTex[GRIDBOXKIND_END];
	D3DMATERIAL9 m_arrMtrl[GRIDBOXKIND_END];

	/* for Trap */
	std::map<cTrap*, ST_TRAPBUILDDATA> m_mapTrapBuildData;

public:
	cMapData();
	~cMapData();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	int GetSizeX()		{ return MAPSIZE_X; }
	int GetSizeY()		{ return MAPSIZE_Y; }
	int GetSizeZ()		{ return MAPSIZE_Z; }
	int GetZeroStdY()	{ return MAPSIZE_Y_ZEROSTD; }
	int GetCubeSize()	{ return MAPSIZE_CUBE; }

	bool MakeBox(int x, int y, int z, EN_GRIDBOXKIND kind = GRIDBOXKIND_DEFAULT);

	bool GetPickingIndex(OUT int &nX, IN int nY, OUT int &nZ, cRay ray); // 맵 메이커에서 사용할 함수.

	void DrawVirtualBox(int x, int y, int z);


	void SaveData(std::string sFileName);
	void LoadData(std::string sFileName);

	bool GetPickingBox(OUT int& nX, OUT int& nY, OUT int& nZ, OUT DIRECTION_6& enPickingDir, IN cRay ray, IN float fDistMax);
	bool IsEnableToBuild(int nX, int nY, int nZ, DIRECTION_6 enDir, int nWidth, int nHeight);
	bool IsEnableToBuild(cRay ray, float fDistMax, int nWidth, int nHeight);

	bool BuildTrap(cTrap* pTrap, cRay ray, float fDistMax, int nWidth, int nHeight);
	bool ClearTrap(cTrap* pTrap);

	/* for interface */
protected: 
	//bool OctaTreeCollisionCheck(cPhysicsBody* body, int nStartX, int nEndX, int nStartY, int nEndY, int nStartZ, int nEndZ);
public: 
	virtual bool MapCollisionCheck(cGameNode* pNode, float fDelta) override;
};

