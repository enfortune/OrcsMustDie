#include "stdafx.h"
#include "cMapData.h"

#include "cHeightMap.h"
#include "cRay.h"

#include "cPhysicsBody.h"

#define MAPCOLLISION_Y_EPSILON 2.0f

cMapData::cMapData()
	: m_bisVBExist(false)
{
	/*for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int y = 0; y < MAPSIZE_Y; y++)
		{
			for (int z = 0; z < MAPSIZE_Z; z++)
			{
				m_arrVB[x][y][z] = nullptr;
			}
		}
	}*/

	ZeroMemory(&m_arrVB[0][0][0], sizeof(LPDIRECT3DVERTEXBUFFER9) * MAPSIZE_X * MAPSIZE_Y * MAPSIZE_Z);
	ZeroMemory(&m_arrTex[0], sizeof(LPDIRECT3DTEXTURE9) * static_cast<int>(GRIDBOXKIND_END));
	ZeroMemory(&m_arrMtrl[0], sizeof(D3DMATERIAL9) *  static_cast<int>(GRIDBOXKIND_END));

	m_mapTrapBuildData.clear();
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
				g_pD3DDevice->CreateVertexBuffer(sizeof(ST_PTN_VERTEX) * 36, D3DUSAGE_WRITEONLY, ST_PTN_VERTEX::FVF, D3DPOOL_MANAGED, &m_arrVB[x][y][z], NULL);
				m_arrGridBox[x][y][z].CopyToVertexBuffer(m_arrVB[x][y][z]);
			}
		}
	}

	for (int i = 0; i < static_cast<int>(GRIDBOXKIND_END); i++)
	{
		switch (static_cast<EN_GRIDBOXKIND>(i))
		{
			case GRIDBOXKIND_NONE:
				m_arrTex[i] = nullptr;
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 0.f;
			break;
			case GRIDBOXKIND_DEFAULT:
				m_arrTex[i] = nullptr;
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 0.f;
			break;
			case GRIDBOXKINE_BLOCK1:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_block1.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 2.f;
			break;
			case GRIDBOXKINE_BLOCK2:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_block2.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 0.f;
			break;
			case GRIDBOXKINE_GRASS:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_grass.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 3.f;
			break;
			case GRIDBOXKINE_MARBLE:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_marble.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 5.f;
			break;
			case GRIDBOXKINE_ROCK:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_rock.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 0.f;
			break;
			case GRIDBOXKINE_SOIL:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_soil.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 0.f;
			break;
			case GRIDBOXKINE_WOODEN:
				m_arrTex[i] = g_pTextureManager->GetTexture("Resource/Image/MapTool/gridbox_wooden.jpg");
				m_arrMtrl[i].Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
				m_arrMtrl[i].Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 0.1f);
				m_arrMtrl[i].Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 0.05f);
				m_arrMtrl[i].Power = 0.f;
			break;
		}

		if (m_arrTex[i] != nullptr) m_arrTex[i]->AddRef();
	}
}
void cMapData::Update(float fDelta)
{
	cGameNode::Update(fDelta);

}
void cMapData::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());
	for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int y = 0; y < MAPSIZE_Y; y++)
		{
			for (int z = 0; z < MAPSIZE_Z; z++)
			{
				//m_arrGridBox[x][y][z].RenderBox(nullptr, nullptr);
				if (m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE) continue;
				g_pD3DDevice->SetTexture(0, m_arrTex[m_arrGridBox[x][y][z].enKind]);
				g_pD3DDevice->SetMaterial(&m_arrMtrl[m_arrGridBox[x][y][z].enKind]);
				g_pD3DDevice->SetFVF(ST_PTN_VERTEX::FVF);
				g_pD3DDevice->SetStreamSource(0, m_arrVB[x][y][z], 0, sizeof(ST_PTN_VERTEX));
				
				ST_PTN_VERTEX* pV;
				m_arrVB[x][y][z]->Lock(0, sizeof(ST_PTN_VERTEX) * 36, (void**)&pV, 0);
				g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);
				m_arrVB[x][y][z]->Unlock();
			}
		}
	}

	cGameNode::Render();
}
void cMapData::Delete()
{
	for (int i = 0; i < static_cast<int>(GRIDBOXKIND_END); i++)
	{
		SAFE_RELEASE(m_arrTex[i]);
	}
	for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int y = 0; y < MAPSIZE_Y; y++)
		{
			for (int z = 0; z < MAPSIZE_Z; z++)
			{
				SAFE_RELEASE(m_arrVB[x][y][z]);
			}
		}
	}

	m_mapTrapBuildData.clear();
}

void cMapData::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}

bool cMapData::MakeBox(int x, int y, int z, EN_GRIDBOXKIND kind)
{
	assert((x >= 0 && x < MAPSIZE_X) && "x 인덱스가 제한을 벗어남");
	assert((y >= 0 && y < MAPSIZE_Y) && "y 인덱스가 제한을 벗어남");
	assert((z >= 0 && z < MAPSIZE_Z) && "z 인덱스가 제한을 벗어남");

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
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());
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

bool cMapData::GetPickingBox(OUT int& nX, OUT int& nY, OUT int& nZ, OUT DIRECTION_6& enPickingDir, IN cRay ray, IN float fDistMax)
{
	bool bRet = false;
	float u, v, dist;
	float fMinDist = -1.f;
	DIRECTION_6 enRetDir = DIRECTION_6::END;
	int nRetX, nRetY, nRetZ;
	for (int x = 0; x < MAPSIZE_X; x++)
	{
		for (int y = 0; y < MAPSIZE_Y; y++)
		{
			for (int z = 0; z < MAPSIZE_Z; z++)
			{
				if ((float)MAPSIZE_CUBE * sqrtf(3) / 4.f + fDistMax < D3DXVec3Length(&(ray.GetPosition() - m_arrGridBox[x][y][z].stCube.GetCenterVec3(DIRECTION_6::END))))
					continue;

				if (m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
					m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END) continue;

				// 1. rear
				if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_00,
					&m_arrGridBox[x][y][z].stCube.vNear_01,
					&m_arrGridBox[x][y][z].stCube.vNear_10,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist) )
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::REAR;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				else if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_01,
					&m_arrGridBox[x][y][z].stCube.vNear_11,
					&m_arrGridBox[x][y][z].stCube.vNear_10,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::REAR;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}

				// 2. front
				if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vFar_00,
					&m_arrGridBox[x][y][z].stCube.vFar_10,
					&m_arrGridBox[x][y][z].stCube.vFar_01,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::FRONT;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				else if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vFar_10,
					&m_arrGridBox[x][y][z].stCube.vFar_01,
					&m_arrGridBox[x][y][z].stCube.vFar_11,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::FRONT;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}

				// 3. left
				if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_00,
					&m_arrGridBox[x][y][z].stCube.vFar_00,
					&m_arrGridBox[x][y][z].stCube.vNear_01,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::LEFT;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				else if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_01,
					&m_arrGridBox[x][y][z].stCube.vFar_00,
					&m_arrGridBox[x][y][z].stCube.vFar_01,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::LEFT;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}

				// 4. right
				if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_10,
					&m_arrGridBox[x][y][z].stCube.vNear_11,
					&m_arrGridBox[x][y][z].stCube.vFar_10,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::RIGHT;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				else if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_11,
					&m_arrGridBox[x][y][z].stCube.vFar_11,
					&m_arrGridBox[x][y][z].stCube.vFar_10,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::RIGHT;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				// 5. top
				if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_01,
					&m_arrGridBox[x][y][z].stCube.vFar_01,
					&m_arrGridBox[x][y][z].stCube.vNear_11,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::TOP;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				else if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vFar_01,
					&m_arrGridBox[x][y][z].stCube.vFar_11,
					&m_arrGridBox[x][y][z].stCube.vNear_11,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::TOP;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}

				// 6. bottom
				if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_10,
					&m_arrGridBox[x][y][z].stCube.vFar_00,
					&m_arrGridBox[x][y][z].stCube.vNear_00,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::BOTTOM;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
				else if (D3DXIntersectTri(
					&m_arrGridBox[x][y][z].stCube.vNear_10,
					&m_arrGridBox[x][y][z].stCube.vFar_10,
					&m_arrGridBox[x][y][z].stCube.vFar_00,
					&ray.GetPosition(), &ray.GetDirection(),
					&u, &v, &dist))
				{
					if (fMinDist < -0.001f || dist < fMinDist)
					{
						fMinDist = dist;
						enRetDir = DIRECTION_6::BOTTOM;
						nRetX = x; nRetY = y; nRetZ = z;
						bRet = true;
					}
				}
			}
		}
	}

	if (bRet)
	{
		nX = nRetX; nY = nRetY; nZ = nRetZ;
		enPickingDir = enRetDir;
	}

	return bRet;
}
bool cMapData::IsEnableToBuild(int nX, int nY, int nZ, DIRECTION_6 enDir, int nWidth, int nHeight)
{
	if (enDir == DIRECTION_6::END || nWidth == 0 || nHeight == 0) return false;

	int nMinX, nMinY, nMinZ;
	int nMaxX, nMaxY, nMaxZ;

	switch (enDir)
	{
		case D3DX_UTIL::DIRECTION_6::FRONT:
			nMinX = nX - nWidth / 2;
			nMaxX = nX + (nWidth - 1) / 2;
			nMinY = nY - nHeight / 2;
			nMaxY = nY + (nHeight - 1) / 2;
			nMinZ = nMaxZ = nZ;
		break;
		case D3DX_UTIL::DIRECTION_6::REAR:
			nMinX = nX - (nWidth - 1) / 2;
			nMaxX = nX + nWidth / 2;
			nMinY = nY - (nHeight - 1) / 2;
			nMaxY = nY + nHeight / 2;
			nMinZ = nMaxZ = nZ;
		break;
		case D3DX_UTIL::DIRECTION_6::LEFT:
			nMinX = nMaxX = nX;
			nMinY = nY - nHeight / 2;
			nMaxY = nY + (nHeight - 1) / 2;
			nMinZ = nZ - nWidth / 2;
			nMaxZ = nZ + (nWidth - 1) / 2;
		break;
		case D3DX_UTIL::DIRECTION_6::RIGHT:
			nMinX = nMaxX = nX;
			nMinY = nY - (nHeight - 1) / 2;
			nMaxY = nY + nHeight / 2;
			nMinZ = nZ - (nWidth - 1) / 2;
			nMaxZ = nZ + nWidth / 2;
		break;
		case D3DX_UTIL::DIRECTION_6::TOP:
			nMinX = nX - (nWidth - 1) / 2;
			nMaxX = nX + nWidth / 2;
			nMinY = nMaxY = nY;
			nMinZ = nZ - (nHeight - 1) / 2;
			nMaxZ = nZ + nHeight / 2;
		break;
		case D3DX_UTIL::DIRECTION_6::BOTTOM:
			nMinX = nX - nWidth / 2;
			nMaxX = nX + (nWidth - 1) / 2;
			nMinY = nMaxY = nY;
			nMinZ = nZ - nHeight / 2;
			nMaxZ = nZ + (nHeight - 1) / 2;
		break;
	}

	if (nMinX < 0) nMinX = 0; if (nMaxX > MAPSIZE_X - 1) nMaxX = MAPSIZE_X - 1;
	if (nMinY < 0) nMinY = 0; if (nMaxY > MAPSIZE_Y - 1) nMaxY = MAPSIZE_Y - 1;
	if (nMinZ < 0) nMinZ = 0; if (nMaxZ > MAPSIZE_Z - 1) nMaxZ = MAPSIZE_Z - 1;

	for (int x = nMinX; x <= nMaxX; x++)
	{
		for (int y = nMinY; y <= nMaxY; y++)
		{
			for (int z = nMinZ; z <= nMaxZ; z++)
			{
				switch (enDir)
				{
					case D3DX_UTIL::DIRECTION_6::FRONT:
						if (m_arrGridBox[x][y][z].pFront != nullptr ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END)
							return false;
					break;
					case D3DX_UTIL::DIRECTION_6::REAR:
						if (m_arrGridBox[x][y][z].pRear != nullptr ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END)
							return false;
					break;
					case D3DX_UTIL::DIRECTION_6::LEFT:
						if (m_arrGridBox[x][y][z].pLeft != nullptr ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END)
							return false;
					break;
					case D3DX_UTIL::DIRECTION_6::RIGHT:
						if (m_arrGridBox[x][y][z].pRight != nullptr ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END)
							return false;
					break;
					case D3DX_UTIL::DIRECTION_6::TOP:
						if (m_arrGridBox[x][y][z].pTop != nullptr ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END)
							return false;
					break;
					case D3DX_UTIL::DIRECTION_6::BOTTOM:
						if (m_arrGridBox[x][y][z].pBottom != nullptr ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE ||
							m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_END)
							return false;
					break;
				}
			}
		}
	}

	return true;
}

bool cMapData::GetBuildPostion(OUT D3DXVECTOR3& vCenter, OUT DIRECTION_6& enPickingDir, cRay ray, float fDistMax, int nWidth, int nHeight)
{
	int nX, nY, nZ;
	DIRECTION_6 enDir;

	if (!this->GetPickingBox(nX, nY, nZ, enDir, ray, fDistMax))
		return false;

	//IsEnableToBuild(nX, nY, nZ, enDir, nWidth, nHeight);
	//vCenter = m_arrGridBox[nX][nY][nZ].stCube.GetCenterVec3(enDir);

	int nMinX, nMinY, nMinZ;
	int nMaxX, nMaxY, nMaxZ;
	D3DXVECTOR3 vTemp(0.f, 0.f, 0.f);

	switch (enDir)
	{
	case D3DX_UTIL::DIRECTION_6::FRONT:
		nMinX = nX - nWidth / 2;
		nMaxX = nX + (nWidth - 1) / 2;
		nMinY = nY - nHeight / 2;
		nMaxY = nY + (nHeight - 1) / 2;
		nMinZ = nMaxZ = nZ;
		break;
	case D3DX_UTIL::DIRECTION_6::REAR:
		nMinX = nX - (nWidth - 1) / 2;
		nMaxX = nX + nWidth / 2;
		nMinY = nY - (nHeight - 1) / 2;
		nMaxY = nY + nHeight / 2;
		nMinZ = nMaxZ = nZ;
		break;
	case D3DX_UTIL::DIRECTION_6::LEFT:
		nMinX = nMaxX = nX;
		nMinY = nY - nHeight / 2;
		nMaxY = nY + (nHeight - 1) / 2;
		nMinZ = nZ - nWidth / 2;
		nMaxZ = nZ + (nWidth - 1) / 2;
		break;
	case D3DX_UTIL::DIRECTION_6::RIGHT:
		nMinX = nMaxX = nX;
		nMinY = nY - (nHeight - 1) / 2;
		nMaxY = nY + nHeight / 2;
		nMinZ = nZ - (nWidth - 1) / 2;
		nMaxZ = nZ + nWidth / 2;
		break;
	case D3DX_UTIL::DIRECTION_6::TOP:
		nMinX = nX - (nWidth - 1) / 2;
		nMaxX = nX + nWidth / 2;
		nMinY = nMaxY = nY;
		nMinZ = nZ - (nHeight - 1) / 2;
		nMaxZ = nZ + nHeight / 2;
		break;
	case D3DX_UTIL::DIRECTION_6::BOTTOM:
		nMinX = nX - nWidth / 2;
		nMaxX = nX + (nWidth - 1) / 2;
		nMinY = nMaxY = nY;
		nMinZ = nZ - nHeight / 2;
		nMaxZ = nZ + (nHeight - 1) / 2;
		break;
	}

	for (int x = nMinX; x <= nMaxX; x++)
	{
		for (int y = nMinY; y <= nMaxY; y++)
		{
			for (int z = nMinZ; z <= nMaxZ; z++)
			{
				vTemp += m_arrGridBox[x][y][z].stCube.GetCenterVec3(enDir);
			}
		}
	}

	vTemp /= nWidth * nHeight;
	vCenter = vTemp;
	enPickingDir = enDir;

	return true;
}


bool cMapData::IsEnableToBuild(cRay ray, float fDistMax, int nWidth, int nHeight)
{
	int nX, nY, nZ;
	DIRECTION_6 enDir;

	if (!this->GetPickingBox(nX, nY, nZ, enDir, ray, fDistMax))
		return false;

	return this->IsEnableToBuild(nX, nY, nZ, enDir, nWidth, nHeight);
}

bool cMapData::BuildTrap(Trap* pTrap, cRay ray, float fDistMax, int nWidth, int nHeight)
{
	int nX, nY, nZ;
	DIRECTION_6 enDir;

	if (!this->GetPickingBox(nX, nY, nZ, enDir, ray, fDistMax))
		return false;

	if (!this->IsEnableToBuild(nX, nY, nZ, enDir, nWidth, nHeight))
		return false;

	int nMinX, nMinY, nMinZ;
	int nMaxX, nMaxY, nMaxZ;

	switch (enDir)
	{
	case D3DX_UTIL::DIRECTION_6::FRONT:
		nMinX = nX - nWidth / 2;
		nMaxX = nX + (nWidth - 1) / 2;
		nMinY = nY - nHeight / 2;
		nMaxY = nY + (nHeight - 1) / 2;
		nMinZ = nMaxZ = nZ;
		break;
	case D3DX_UTIL::DIRECTION_6::REAR:
		nMinX = nX - (nWidth - 1) / 2;
		nMaxX = nX + nWidth / 2;
		nMinY = nY - (nHeight - 1) / 2;
		nMaxY = nY + nHeight / 2;
		nMinZ = nMaxZ = nZ;
		break;
	case D3DX_UTIL::DIRECTION_6::LEFT:
		nMinX = nMaxX = nX;
		nMinY = nY - nHeight / 2;
		nMaxY = nY + (nHeight - 1) / 2;
		nMinZ = nZ - nWidth / 2;
		nMaxZ = nZ + (nWidth - 1) / 2;
		break;
	case D3DX_UTIL::DIRECTION_6::RIGHT:
		nMinX = nMaxX = nX;
		nMinY = nY - (nHeight - 1) / 2;
		nMaxY = nY + nHeight / 2;
		nMinZ = nZ - (nWidth - 1) / 2;
		nMaxZ = nZ + nWidth / 2;
		break;
	case D3DX_UTIL::DIRECTION_6::TOP:
		nMinX = nX - (nWidth - 1) / 2;
		nMaxX = nX + nWidth / 2;
		nMinY = nMaxY = nY;
		nMinZ = nZ - (nHeight - 1) / 2;
		nMaxZ = nZ + nHeight / 2;
		break;
	case D3DX_UTIL::DIRECTION_6::BOTTOM:
		nMinX = nX - nWidth / 2;
		nMaxX = nX + (nWidth - 1) / 2;
		nMinY = nMaxY = nY;
		nMinZ = nZ - nHeight / 2;
		nMaxZ = nZ + (nHeight - 1) / 2;
		break;
	}

	if (nMinX < 0) nMinX = 0; if (nMaxX > MAPSIZE_X - 1) nMaxX = MAPSIZE_X - 1;
	if (nMinY < 0) nMinY = 0; if (nMaxY > MAPSIZE_Y - 1) nMaxY = MAPSIZE_Y - 1;
	if (nMinZ < 0) nMinZ = 0; if (nMaxZ > MAPSIZE_Z - 1) nMaxZ = MAPSIZE_Z - 1;

	m_mapTrapBuildData[pTrap].enDir = enDir;

	for (int x = nMinX; x <= nMaxX; x++)
	{
		for (int y = nMinY; y <= nMaxY; y++)
		{
			for (int z = nMinZ; z <= nMaxZ; z++)
			{
				switch (enDir)
				{
					case D3DX_UTIL::DIRECTION_6::FRONT:
						m_arrGridBox[x][y][z].pFront = pTrap;
					break;
					case D3DX_UTIL::DIRECTION_6::REAR:
						m_arrGridBox[x][y][z].pRear = pTrap;
					break;
					case D3DX_UTIL::DIRECTION_6::LEFT:
						m_arrGridBox[x][y][z].pLeft = pTrap;
					break;
					case D3DX_UTIL::DIRECTION_6::RIGHT:
						m_arrGridBox[x][y][z].pRight = pTrap;
					break;
					case D3DX_UTIL::DIRECTION_6::TOP:
						m_arrGridBox[x][y][z].pTop = pTrap;
					break;
					case D3DX_UTIL::DIRECTION_6::BOTTOM:
						m_arrGridBox[x][y][z].pBottom = pTrap;
					break;
				}
				m_mapTrapBuildData[pTrap].vecXYZ.push_back(ST_INDEX_XYZ(x, y, z));
			}
		}
	}

	return true;
}
bool cMapData::ClearTrap(Trap* pTrap)
{
	std::map<Trap*, ST_TRAPBUILDDATA>::iterator iter;
	iter = m_mapTrapBuildData.find(pTrap);

	if (iter == m_mapTrapBuildData.end())
		return false;

	int nX, nY, nZ;
	for (size_t i = 0; i = iter->second.vecXYZ.size(); i++)
	{
		nX = iter->second.vecXYZ[i].x;
		nY = iter->second.vecXYZ[i].y;
		nZ = iter->second.vecXYZ[i].z;

		switch (iter->second.enDir)
		{
			case D3DX_UTIL::DIRECTION_6::FRONT:
				m_arrGridBox[nX][nY][nZ].pFront = nullptr;
			break;
			case D3DX_UTIL::DIRECTION_6::REAR:
				m_arrGridBox[nX][nY][nZ].pRear = nullptr;
			break;
			case D3DX_UTIL::DIRECTION_6::LEFT:
				m_arrGridBox[nX][nY][nZ].pLeft = nullptr;
			break;
			case D3DX_UTIL::DIRECTION_6::RIGHT:
				m_arrGridBox[nX][nY][nZ].pRight = nullptr;
			break;
			case D3DX_UTIL::DIRECTION_6::TOP:
				m_arrGridBox[nX][nY][nZ].pTop = nullptr;
			break;
			case D3DX_UTIL::DIRECTION_6::BOTTOM:
				m_arrGridBox[nX][nY][nZ].pBottom = nullptr;
			break;
		}

	}

	iter = m_mapTrapBuildData.erase(iter);
	return true;
}


//bool cMapData::OctaTreeCollisionCheck(cPhysicsBody* body, int nStartX, int nEndX, int nStartY, int nEndY, int nStartZ, int nEndZ)
//{
//
//	if (nStartX == nEndX || nStartY == nEndY || nStartZ == nEndZ) return false;
//
//	switch (body->GetShapeData().enShapeType)
//	{
//	case PHYSICSSHAPETYPE_SPHERE:
//	{
//		/* 세로 체크*/
//		float u, v, dist;
//		bool isCollision = false;
//		/*
//		// 위   = 24~29 
//		// 아래 = 30~35 
//		for (int x = nStartX; x < nEndX; x++)
//		{
//			for (int y = nStartY; y < nEndY; y++)
//			{
//				for (int z = nStartZ; z < nEndZ; z++)
//				{
//					if (D3DXIntersectTri())
//					{
//
//					}
//				}
//			}
//		}
//		*/
//
//		
//		/*
//		_000 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, z* MAPSIZE_CUBE);
//		_001 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
//		_010 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z)* MAPSIZE_CUBE);
//		_011 = D3DXVECTOR3(x * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
//		_100 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD)* MAPSIZE_CUBE, (z)* MAPSIZE_CUBE);
//		_101 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y - MAPSIZE_Y_ZEROSTD)* MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
//		_110 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z)* MAPSIZE_CUBE);
//		_111 = D3DXVECTOR3((x + 1) * MAPSIZE_CUBE, (y + 1 - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, (z + 1) * MAPSIZE_CUBE);
//		*/
//		//아래-1 // 000 - 001 - 101
//		if (CheckAABBRectosahedronContainVec3(
//			nStartX * MAPSIZE_CUBE, nEndX * MAPSIZE_CUBE,
//			nStartY * MAPSIZE_CUBE, nEndY * MAPSIZE_CUBE,
//			nStartZ * MAPSIZE_CUBE, nEndZ * MAPSIZE_CUBE,
//			body->GetTempPhysicsData().vPos + body->GetShapeData().vCenter))
//		{
//			if ((nEndX - nStartX == 1) && (nEndY - nStartY == 1) && (nEndZ - nStartZ == 1))
//			{
//				return true;
//			}
//			else
//			{
//				
//			}
//		}
//		else
//		{
//			D3DXIntersectTri(
//				&D3DXVECTOR3(nStartX * MAPSIZE_CUBE, (nStartY - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, nStartZ * MAPSIZE_CUBE),
//				&D3DXVECTOR3(nStartX * MAPSIZE_CUBE, (nStartY - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, nEndZ * MAPSIZE_CUBE),
//				&D3DXVECTOR3(nEndZ * MAPSIZE_CUBE, (nStartY - MAPSIZE_Y_ZEROSTD) * MAPSIZE_CUBE, nEndZ * MAPSIZE_CUBE),
//				&(body->GetTempPhysicsData().vPos + body->GetShapeData().vCenter),
//				&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
//				&u, &v, &dist
//				);
//		}
//		
//		
//	}
//	break;
//	case PHYSICSSHAPETYPE_CUBOID:
//	{
//
//	}
//	break;
//	case PHYSICSSHAPETYPE_END:
//		break;
//	default:
//		break;
//	}
//}


/* interface */

bool cMapData::MapCollisionCheck(cGameNode* pNode, float fDelta)
{
	cPhysicsBody* pBody = pNode->GetPhysicsBody();
	bool bRet = false;
	
	if (pBody == nullptr) return false;
	

	D3DXMATRIXA16 matWorld, matParentWorld, matR, matT;
	if (pNode->GetParentNode() == nullptr)
		D3DXMatrixIdentity(&matParentWorld);
	else
		matParentWorld = pNode->GetParentNode()->GetMatrixToWorld();
	
	D3DXMatrixRotationAxis(&matR, 
		&pBody->GetTempPhysicsData().vAxis,
		pBody->GetTempPhysicsData().fRotAngle);
	D3DXMatrixTranslation(&matT, 
		pBody->GetTempPhysicsData().vPos.x, 
		pBody->GetTempPhysicsData().vPos.y, 
		pBody->GetTempPhysicsData().vPos.z);

	matWorld = matR * matT * matParentWorld;
	
	ST_SPHERE  stMapSphere;
	ST_FRUSTUM stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
	ST_SPHERE  stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);

	D3DXVECTOR3 vCrushNorm;
	D3DXVECTOR3 vGroundCheckNorm;
	float fDot;

	//this->OctaTreeCollisionCheck(body, 0, MAPSIZE_X, 0, MAPSIZE_Y, 0, MAPSIZE_Z);

	switch (pBody->GetShapeData().enShapeType)
	{
		case PHYSICSSHAPETYPE_SPHERE: // TODO: 추후 알고리즘 개선할 것
		{
			for (int x = 0; x < MAPSIZE_X; x++)
			{
				for (int y = 0; y < MAPSIZE_Y; y++)
				{
					for (int z = 0; z < MAPSIZE_Z; z++)
					{
						//0. 설치되지 않은 박스는 제낀다
						if (m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE) continue;

						stMapSphere.fRadius = static_cast<float>(MAPSIZE_CUBE) / 2.f * sqrtf(3.f);
						stMapSphere.vCenter = D3DXVECTOR3(
							(static_cast<float>(x)+0.5f) * static_cast<float>(MAPSIZE_CUBE),
							(static_cast<float>(y - MAPSIZE_Y_ZEROSTD) + 0.5f) * static_cast<float>(MAPSIZE_CUBE),
							(static_cast<float>(z)+0.5f) * static_cast<float>(MAPSIZE_CUBE));
						//1. 구면충돌을 먼저 해본다
						if (!CheckSphereIntersectSphere(&stMapSphere, &stBodySphere)) 
							continue;

						//2. 다면체 vs 구 충돌을 한다
						if (CheckFrustumIntersectSphere(&m_arrGridBox[x][y][z].stCube, &stBodySphere))
						{
							vCrushNorm = m_arrGridBox[x][y][z].stCube.GetNearestSideNormalVec3(&stBodySphere.vCenter);
							fDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vCrushNorm);

							if (fDot < 0)
							{
								D3DXVec3Normalize(&vGroundCheckNorm, &vCrushNorm);
								if (D3DXVec3Dot(&vGroundCheckNorm, &D3DXVECTOR3(0.f, 1.f, 0.f)) > cosf(PI / 4))
								{
									pBody->GetTempPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
									pBody->GetPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
									D3DXVec3Normalize(&vGroundCheckNorm, &vCrushNorm);
									pBody->GetPhysicsData().bOnGround = true;

									//pBody->UpdateTempPhysics(fDelta);

									D3DXMatrixRotationAxis(&matR,
										&pBody->GetTempPhysicsData().vAxis,
										pBody->GetTempPhysicsData().fRotAngle);
									D3DXMatrixTranslation(&matT,
										pBody->GetTempPhysicsData().vPos.x,
										pBody->GetTempPhysicsData().vPos.y,
										pBody->GetTempPhysicsData().vPos.z);

									matWorld = matR * matT * matParentWorld;

									stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
									stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);
								}
								else
								{//MAPCOLLISION_Y_EPSILON
									float fDeltaY = m_arrGridBox[x][y][z].stCube.GetHighestPoint() - (stBodySphere.vCenter.y - stBodySphere.fRadius);
									if (fabs(MAPCOLLISION_Y_EPSILON * fDelta * pBody->GetTempPhysicsData().vVelocity.y) > fabs(fDeltaY))
									{
										pBody->GetTempPhysicsData().vPos.y += fDeltaY;
										pBody->GetPhysicsData().vPos.y += fDeltaY;
										if (pBody->GetTempPhysicsData().vVelocity.y < 0)
										{
											pBody->GetTempPhysicsData().vVelocity.y = 0;
											pBody->GetPhysicsData().vVelocity.y = 0;
											pBody->GetPhysicsData().bOnGround = true;
										}

										//pBody->UpdateTempPhysics(fDelta);

										D3DXMatrixRotationAxis(&matR,
											&pBody->GetTempPhysicsData().vAxis,
											pBody->GetTempPhysicsData().fRotAngle);
										D3DXMatrixTranslation(&matT,
											pBody->GetTempPhysicsData().vPos.x,
											pBody->GetTempPhysicsData().vPos.y,
											pBody->GetTempPhysicsData().vPos.z);

										matWorld = matR * matT * matParentWorld;

										stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
										stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);
									}
									else
									{
										pBody->GetTempPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
										pBody->GetPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);

										//pBody->UpdateTempPhysics(fDelta);

										/*D3DXMatrixRotationAxis(&matR,
											&pBody->GetTempPhysicsData().vAxis,
											pBody->GetTempPhysicsData().fRotAngle);
										D3DXMatrixTranslation(&matT,
											pBody->GetTempPhysicsData().vPos.x,
											pBody->GetTempPhysicsData().vPos.y,
											pBody->GetTempPhysicsData().vPos.z);

										matWorld = matR * matT * matParentWorld;

										stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
										stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);*/
									}
								}
							}
							bRet =  true;
						}
						
					}
				}
			}
		}
		break;
		case PHYSICSSHAPETYPE_CUBOID:
		{
			for (int x = 0; x < MAPSIZE_X; x++)
			{
				for (int y = 0; y < MAPSIZE_Y; y++)
				{
					for (int z = 0; z < MAPSIZE_Z; z++)
					{
						//0. 설치되지 않은 박스는 제낀다
						if (m_arrGridBox[x][y][z].enKind == GRIDBOXKIND_NONE) continue;

						stMapSphere.fRadius = static_cast<float>(MAPSIZE_CUBE) / 2.f * sqrtf(3.f);
						stMapSphere.vCenter = D3DXVECTOR3(
							(static_cast<float>(x)+0.5f) * static_cast<float>(MAPSIZE_CUBE),
							(static_cast<float>(y - MAPSIZE_Y_ZEROSTD)+0.5f) * static_cast<float>(MAPSIZE_CUBE),
							(static_cast<float>(z)+0.5f) * static_cast<float>(MAPSIZE_CUBE));
						//1. 구면충돌을 먼저 해본다
						if (!CheckSphereIntersectSphere(&stMapSphere, &stBodySphere)) 
							continue;

						//2. 다면체 vs 다면체 충돌을 한다d
						if (CheckOBBCollision(&m_arrGridBox[x][y][z].stCube, &stBodyFrustum))
						{
							vCrushNorm = m_arrGridBox[x][y][z].stCube.GetNearestSideNormalVec3(&stBodyFrustum);
							fDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vCrushNorm);
							if (fDot < 0.0001f)
							{
								D3DXVec3Normalize(&vGroundCheckNorm, &vCrushNorm);
								if (D3DXVec3Dot(&vGroundCheckNorm, &D3DXVECTOR3(0.f, 1.f, 0.f)) > cosf(PI / 4))
								{
									pBody->GetTempPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
									pBody->GetPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
									pBody->GetPhysicsData().bOnGround = true;

									//pBody->UpdateTempPhysics(fDelta);

									/*D3DXMatrixRotationAxis(&matR,
										&pBody->GetTempPhysicsData().vAxis,
										pBody->GetTempPhysicsData().fRotAngle);
									D3DXMatrixTranslation(&matT,
										pBody->GetTempPhysicsData().vPos.x,
										pBody->GetTempPhysicsData().vPos.y,
										pBody->GetTempPhysicsData().vPos.z);

									matWorld = matR * matT * matParentWorld;

									stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
									stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);*/
								}
								else
								{
									float fDeltaY = m_arrGridBox[x][y][z].stCube.GetHighestPoint() - stBodyFrustum.GetLowestPoint();
									if (fabs(MAPCOLLISION_Y_EPSILON * fDelta * pBody->GetTempPhysicsData().vVelocity.y) > fabs(fDeltaY))
									{
										pBody->GetTempPhysicsData().vPos.y += fDeltaY;
										pBody->GetPhysicsData().vPos.y += fDeltaY;
										if (pBody->GetTempPhysicsData().vVelocity.y < 0)
										{
											pBody->GetTempPhysicsData().vVelocity.y = 0;
											pBody->GetPhysicsData().vVelocity.y = 0;
											pBody->GetPhysicsData().bOnGround = true;
										}
										
										//pBody->UpdateTempPhysics(fDelta);

										/*D3DXMatrixRotationAxis(&matR,
											&pBody->GetTempPhysicsData().vAxis,
											pBody->GetTempPhysicsData().fRotAngle);
										D3DXMatrixTranslation(&matT,
											pBody->GetTempPhysicsData().vPos.x,
											pBody->GetTempPhysicsData().vPos.y,
											pBody->GetTempPhysicsData().vPos.z);

										matWorld = matR * matT * matParentWorld;

										stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
										stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);*/
									}
									else // 벽에 부딪힌 경우 아님?
									{
										pBody->GetTempPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
										pBody->GetPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);

										//pBody->UpdateTempPhysics(fDelta);

										//D3DXMatrixRotationAxis(&matR,
										//	&pBody->GetTempPhysicsData().vAxis,
										//	pBody->GetTempPhysicsData().fRotAngle);
										//D3DXMatrixTranslation(&matT,
										//	pBody->GetTempPhysicsData().vPos.x,
										//	pBody->GetTempPhysicsData().vPos.y,
										//	pBody->GetTempPhysicsData().vPos.z);

										//matWorld = matR * matT * matParentWorld;

										//stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
										//stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);
									}
								}		
								// 가속도와 힘도 제거할지는 조금 더 지켜보자.
							}
							bRet = true;
						}
					}
				}
			}
		}
		break;
		case PHYSICSSHAPETYPE_END:
		break;
		default:
		break;
	}

	return bRet;
}
