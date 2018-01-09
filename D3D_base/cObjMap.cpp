#include "stdafx.h"
#include "cObjMap.h"
#include "cObjLoader.h"


cObjMap::cObjMap(char * szFolder, char * szFile, D3DXMATRIXA16* pMat)
{
	Load(szFolder, szFile, pMat);
}


cObjMap::~cObjMap()
{
}

void cObjMap::Load(char * szFolder, char * szFile, D3DXMATRIXA16* pMat)
{
	cObjLoader loader;
	loader.LoadSurface(m_vecSurface, szFolder, szFile, pMat);
}

bool cObjMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	D3DXVECTOR3 vRayPos(x, 1000.0f, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	float u, v, d;
	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		if (D3DXIntersectTri(&m_vecSurface[i],
			&m_vecSurface[i + 1],
			&m_vecSurface[i + 2],
			&vRayPos,
			&vRayDir,
			&u, &v, &d))
		{
			y = vRayPos.y - d;
			return true;
		}
	}

	return false;
}
