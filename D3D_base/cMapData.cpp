#include "stdafx.h"
#include "cMapData.h"

#include "cHeightMap.h"

cMapData::cMapData()
{
}


cMapData::~cMapData()
{
	this->Delete();
}

void cMapData::Setup()
{
	cGameNode::Setup(true);


}
void cMapData::Update(float fDelta)
{
	cGameNode::Update(fDelta);

}
void cMapData::Render()
{
	cGameNode::Render();
}
void cMapData::Delete()
{
}

void cMapData::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}


bool cMapData::MakeBox(int x, int y, int z, EN_GRIDBOXKIND kind)
{
	assert((x >= 0 && x < MAPSIZE_X) && "x �ε����� ������ ���");
	assert((y >= 0 && y < MAPSIZE_Y) && "y �ε����� ������ ���");
	assert((z >= 0 && z < MAPSIZE_Z) && "z �ε����� ������ ���");

	m_arrGridBox[x][y][z].MakeBox(x, y, z, kind);
	return true;
	 
}