#include "stdafx.h"
#include "cMapData.h"

#include "cHeightMap.h"
#include "cRay.h"

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

	for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int y = 0; y < MAPSIZE_Y; y++)
		{
			for (int z = 0; z < MAPSIZE_Z; z++)
			{
				m_arrGridBox[x][y][z].MakeBox(x, y, z, GRIDBOXKIND_NONE);
			}
		}
	}

}
void cMapData::Update(float fDelta)
{
	cGameNode::Update(fDelta);

}
void cMapData::Render()
{
	for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int y = 0; y < MAPSIZE_Y; y++)
		{
			for (int z = 0; z < MAPSIZE_Z; z++)
			{
				m_arrGridBox[x][y][z].RenderBox(nullptr, nullptr);
			}
		}
	}

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
	assert((x >= 0 && x < MAPSIZE_X) && "x ÀÎµ¦½º°¡ Á¦ÇÑÀ» ¹þ¾î³²");
	assert((y >= 0 && y < MAPSIZE_Y) && "y ÀÎµ¦½º°¡ Á¦ÇÑÀ» ¹þ¾î³²");
	assert((z >= 0 && z < MAPSIZE_Z) && "z ÀÎµ¦½º°¡ Á¦ÇÑÀ» ¹þ¾î³²");

	m_arrGridBox[x][y][z].MakeBox(x, y, z, kind);
	return true;
}

bool cMapData::GetPickingIndex(OUT int &nX, IN int nY, OUT int &nZ, cRay ray)
{
	if (nY < 0 || nY >= MAPSIZE_Y) return false;

	float u, v, dest;

	for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int z = 0; z < MAPSIZE_Z; z++)
		{
			//m_arrGridBox[x][y][nZ].stCube;
			if (D3DXIntersectTri(
				&m_arrGridBox[x][nY][z].v[30].p,
				&m_arrGridBox[x][nY][z].v[31].p,
				&m_arrGridBox[x][nY][z].v[32].p,
				&ray.GetPosition(), &ray.GetDirection(), &u, &v, &dest))
			{
				nX = x; nZ = z;
				return true;
			}

			else if (D3DXIntersectTri(
				&m_arrGridBox[x][nY][z].v[33].p,
				&m_arrGridBox[x][nY][z].v[34].p,
				&m_arrGridBox[x][nY][z].v[35].p,
				&ray.GetPosition(), &ray.GetDirection(), &u, &v, &dest))
			{
				nX = x; nZ = z;
				return true;
			}
		}
	}

	return false;
}

void cMapData::DrawVirtualBox(int x, int y, int z)
{
	m_arrGridBox[x][y][z].RenderVirtualBox();
}

void cMapData::SaveData(std::string sFileName)
{
	HANDLE file;
	DWORD write;
	file = CreateFile(sFileName.c_str(), GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, m_arrGridBox, sizeof(ST_GRIDBOX) * MAPSIZE_X * MAPSIZE_Y * MAPSIZE_Z, &write, NULL);
	CloseHandle(file);
}
void cMapData::LoadData(std::string sFileName)
{
	HANDLE file;
	DWORD read;
	file = CreateFile(sFileName.c_str(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, m_arrGridBox, sizeof(ST_GRIDBOX) * MAPSIZE_X * MAPSIZE_Y * MAPSIZE_Z, &read, NULL);
	CloseHandle(file);
}