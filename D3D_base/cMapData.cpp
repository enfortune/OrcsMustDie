#include "stdafx.h"
#include "cMapData.h"

#include "cHeightMap.h"
#include "cRay.h"

#include "cPhysicsBody.h"

#define MAPCOLLISION_RATIOEPSILON 0.2f

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

bool cMapData::MapCollisionCheck(cGameNode* pNode)
{
	cPhysicsBody* pBody = pNode->GetPhysicsBody();
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
	
	ST_FRUSTUM stMapFrustum;
	ST_SPHERE  stMapSphere;
	ST_FRUSTUM stBodyFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
	ST_SPHERE  stBodySphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);

	D3DXVECTOR3 vCrushNorm;
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

						stMapSphere.fRadius = static_cast<float>(MAPSIZE_CUBE) / 2 * sqrtf(3.f);
						stMapSphere.vCenter = D3DXVECTOR3(
							(static_cast<float>(x)+0.5f) * MAPSIZE_CUBE,
							(static_cast<float>(y)+0.5f) * MAPSIZE_CUBE,
							(static_cast<float>(z)+0.5f) * MAPSIZE_CUBE);
						//1. 구면충돌을 먼저 해본다
						if (CheckSphereIntersectSphere(&stMapSphere, &stBodySphere)) continue;

						//2. 다면체 vs 구 충돌을 한다
						if (CheckFrustumIntersectSphere(&stMapFrustum, &stBodySphere))
						{
							vCrushNorm = stMapFrustum.GetNearestSideNormalVec3(&stBodySphere.vCenter);
							fDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vCrushNorm);
							if (fDot < 0)
							{
								pBody->GetTempPhysicsData().vVelocity + (vCrushNorm * fDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
								pBody->GetPhysicsData().vVelocity = pBody->GetTempPhysicsData().vVelocity;
							}

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

						stMapSphere.fRadius = static_cast<float>(MAPSIZE_CUBE) / 2 * sqrtf(3.f);
						stMapSphere.vCenter = D3DXVECTOR3(
							(static_cast<float>(x)+0.5f) * MAPSIZE_CUBE,
							(static_cast<float>(y)+0.5f) * MAPSIZE_CUBE,
							(static_cast<float>(z)+0.5f) * MAPSIZE_CUBE);
						//1. 구면충돌을 먼저 해본다
						if (CheckSphereIntersectSphere(&stMapSphere, &stBodySphere)) continue;

						//2. 다면체 vs 다면체 충돌을 한다
						if (CheckFrustumIntersectFrustum(&stMapFrustum, &stBodyFrustum))
						{
							vCrushNorm = stMapFrustum.GetNearestSideNormalVec3(&stBodySphere.vCenter);
							fDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vCrushNorm);
							if (fDot < 0)
							{
								pBody->GetTempPhysicsData().vVelocity + (vCrushNorm * fDot) * (1 + pBody->GetTempPhysicsData().fElasticity);
								pBody->GetPhysicsData().vVelocity = pBody->GetTempPhysicsData().vVelocity;
								// 가속도와 힘도 제거할지는 조금 더 지켜보자.
							}
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

	return false;
}