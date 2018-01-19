#pragma once
#include "cGameNode.h"

#define MAPSIZE_X 100
#define MAPSIZE_Y 100
#define MAPSIZE_Z  10
#define MAPSIZE_Z_ZEROSTD 4
#define MAPSIZE_CUBE 1

class cTrap;
class cHeightMap;

typedef enum EN_GRIDBOXKIND
{
	GRIDBOXKIND_NONE,    // �ڽ��� ���� ����
	GRIDBOXKIND_DEFAULT, // �ؽ��� �����, ���� ���̸ʿ��� ������� ������������ (���ϰ�� üũ�� ���Ѵٴ���)
	GRIDBOXKIND_END
}GRIDBOXKIND;

struct ST_CUBE_XYZ
{
	D3DXVECTOR3 _000;
	D3DXVECTOR3 _100;
	D3DXVECTOR3 _110;
	D3DXVECTOR3 _010;
	D3DXVECTOR3	_001;
	D3DXVECTOR3	_101;
	D3DXVECTOR3	_111;
	D3DXVECTOR3 _011;

	ST_CUBE_XYZ()
	{
		_000 = _100 = _110 = _010 =
			_001 = _101 = _111 = _011 = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	D3DXVECTOR3& operator [] (int n) // ���� ������ ��
	{
		switch (n)
		{
			assert((n >= 0 && n < 8) && "ť���ε����� 0~7������ ���ٰ���");

			case 0:	return _000;  break;
			case 1:	return _100; break;
			case 2:	return _110; break;
			case 3:	return _010; break;
			case 4:	return _001; break;
			case 5:	return _101; break;
			case 6:	return _111; break;
			case 7:	return _011; break;
				
			default: return D3DXVECTOR3(); break; // ��¥�� assert �ɸ�
		}
	}
};

struct ST_GRIDBOX
{
	EN_GRIDBOXKIND enKind;
	ST_CUBE_XYZ stCube;

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
		assert((x >= 0 && x < MAPSIZE_X) && "x �ε����� ������ ���");
		assert((y >= 0 && y < MAPSIZE_Y) && "y �ε����� ������ ���");
		assert((z >= 0 && z < MAPSIZE_Z) && "z �ε����� ������ ���");


		stCube._000 = D3DXVECTOR3(x * MAPSIZE_CUBE, y * MAPSIZE_CUBE, (z - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._001 = D3DXVECTOR3(x * MAPSIZE_CUBE, y * MAPSIZE_CUBE, (z + 1 - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._010 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y + 1) * MAPSIZE_CUBE, (z - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._011 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y + 1) * MAPSIZE_CUBE, (z + 1 - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._100 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, y * MAPSIZE_CUBE, (z - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._101 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, y * MAPSIZE_CUBE, (z + 1 - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._110 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y + 1) * MAPSIZE_CUBE, (z - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
		stCube._110 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y + 1) * MAPSIZE_CUBE, (z + 1 - MAPSIZE_Z_ZEROSTD) * MAPSIZE_CUBE);
	}
};


class cMapData : public cGameNode
{
protected:
	ST_GRIDBOX m_arrGridBox[MAPSIZE_X][MAPSIZE_Y][MAPSIZE_Z];
	// cHeightMap m_hMap;

public:
	cMapData();
	~cMapData();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	bool MakeBox(int x, int y, int z, EN_GRIDBOXKIND kind = GRIDBOXKIND_DEFAULT);
};

