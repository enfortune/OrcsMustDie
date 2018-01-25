#pragma once
#include "cGameNode.h"
#include "iGameMap.h"

#define MAPSIZE_X 30
#define MAPSIZE_Z 30
#define MAPSIZE_Y  8
#define MAPSIZE_Y_ZEROSTD 4
#define MAPSIZE_CUBE 2

class cTrap;
class cHeightMap;
class cRay;

typedef enum EN_GRIDBOXKIND
{
	GRIDBOXKIND_NONE,    // 박스가 없는 지점
	GRIDBOXKIND_DEFAULT, // 텍스쳐 씌우고, 오토 높이맵에도 어느정도 영향있을거임 (물일경우 체크를 안한다던지)
	GRIDBOXKIND_END
}GRIDBOXKIND;

struct ST_CUBE_XYZ
{
	/*ST_PTN_VERTEX _000;
	ST_PTN_VERTEX _100;
	ST_PTN_VERTEX _110;
	ST_PTN_VERTEX _010;
	ST_PTN_VERTEX _001;
	ST_PTN_VERTEX _101;
	ST_PTN_VERTEX _111;
	ST_PTN_VERTEX _011;*/
	/*ST_CUBE_XYZ()
	{
		_000.p = _100.p = _110.p = _010.p =
			_001 = _101 = _111 = _011 = D3DXVECTOR3(0.f, 0.f, 0.f);
	}*/
	/*ST_PTN_VERTEX& operator [] (int n) // 포문 돌리기 용
	{
		switch (n)
		{
			assert((n >= 0 && n < 8) && "큐브인덱스는 0~7까지만 접근가능");
		case 0:	return _000;  break;
		case 1:	return _100; break;
		case 2:	return _110; break;
		case 3:	return _010; break;
		case 4:	return _001; break;
		case 5:	return _101; break;
		case 6:	return _111; break;
		case 7:	return _011; break;

		default: return ST_PTN_VERTEX(); break; // 어짜피 assert 걸림
		}
	}*/

};

struct ST_GRIDBOX
{
	EN_GRIDBOXKIND enKind;
	//ST_CUBE_XYZ stCube;
	ST_PTN_VERTEX v[36]; // 그리는 버텍스 (추후 버텍스 버퍼로 바꿀 것)
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
		stCube.vNear_00 = _010;
		stCube.vNear_00 = _100;
		stCube.vNear_00 = _110;
		stCube.vFar_00 = _001;
		stCube.vFar_00 = _011;
		stCube.vFar_00 = _101;
		stCube.vFar_00 = _111;

		//뒤

		v[0].p = _000; v[0].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		v[1].p = _010; v[1].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		v[2].p = _110; v[2].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


		v[3].p = _000; v[3].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		v[4].p = _110; v[4].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		v[5].p = _100; v[5].n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


		//앞
		v[6].p = _001; v[3].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		v[7].p = _111; v[4].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		v[8].p = _011; v[5].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


		v[9].p = _001;	v[9].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		v[10].p = _101; v[10].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		v[11].p = _111;	v[11].n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		//왼쪽
		v[12].p = _001;	v[12].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		v[13].p = _011; v[13].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		v[14].p = _010; v[14].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

		v[15].p = _001; v[15].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		v[16].p = _010; v[16].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		v[17].p = _000; v[17].n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

		//오른쪽
		v[18].p = _100; v[18].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		v[19].p = _110; v[19].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		v[20].p = _111;  v[20].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

		v[21].p = _100; v[21].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		v[22].p = _111;  v[22].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		v[23].p = _101; v[23].n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

		//위
		v[24].p = _010; v[24].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[25].p = _011; v[25].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[26].p = _111;	v[26].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		v[27].p = _010; v[27].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[28].p = _111;  v[28].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[29].p = _110; v[29].n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//아래
		v[30].p = _001;	v[30].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		v[31].p = _000; v[31].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		v[32].p = _100; v[32].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		v[33].p = _001;  v[33].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		v[34].p = _100; v[34].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		v[35].p = _101; v[35].n = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
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
};


class cMapData : public cGameNode, public iGameMap
{
protected:
	ST_GRIDBOX m_arrGridBox[MAPSIZE_X][MAPSIZE_Y][MAPSIZE_Z];
	// cHeightMap m_hMap;
	
	//std::vector<> m_stVirtualBox;

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

	bool MakeBox(int x, int y, int z, EN_GRIDBOXKIND kind = GRIDBOXKIND_DEFAULT);

	bool GetPickingIndex(OUT int &nX, IN int nY, OUT int &nZ, cRay ray); // 맵 메이커에서 사용할 함수.

	void DrawVirtualBox(int x, int y, int z);


	void SaveData(std::string sFileName);
	void LoadData(std::string sFileName);

	

	/* for interface */
protected: 
	//bool OctaTreeCollisionCheck(cPhysicsBody* body, int nStartX, int nEndX, int nStartY, int nEndY, int nStartZ, int nEndZ);
public: 
	virtual bool MapCollisionCheck(cGameNode* pNode) override;
};

