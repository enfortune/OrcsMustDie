#include "stdafx.h"
#include "cHeightMap.h"


cHeightMap::cHeightMap()
	: m_nTileCount(0)
	, m_pMesh(NULL)
{
}


cHeightMap::~cHeightMap()
{
	SAFE_RELEASE(m_pMesh);
}

void cHeightMap::Setup(char* szFolder, char* szRaw, char* szTex)
{
	std::string sFolder(szFolder);
	std::string sRaw = sFolder + std::string("/") + std::string(szRaw);
	std::string sTex = sFolder + std::string("/") + std::string(szTex);

	FILE* fp = NULL;
	fopen_s(&fp, sRaw.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	int nRow = (int)sqrt((float)nFileSize);
	int nCol = nRow;
	m_nTileCount = nCol - 1;

	fseek(fp, 0, SEEK_SET);

	std::vector<ST_PTN_VERTEX> vecVertex(nFileSize);
	m_vecVertex.resize(nFileSize);

	for (int i = 0; i < nFileSize; i++)
	{
		ST_PTN_VERTEX v;
		v.p = D3DXVECTOR3(i % nCol,
			((unsigned char)fgetc(fp)) / 10.0f, i / nCol);

		v.n = D3DXVECTOR3(0, 1, 0);

		v.t = D3DXVECTOR2((i % nCol) / (float)nCol,
			(i / nCol) / (float)nCol);

		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
	}
	fclose(fp);

	// normal vector
	for (int x = 1; x < m_nTileCount; x++)
	{
		for (int z = 1; z < m_nTileCount; z++)
		{
			int left = (z + 0) * nCol + (x - 1);
			int right = (z + 0) * nCol + (x + 1);
			int up = (z + 1) * nCol + (x + 0);
			int down = (z - 1) * nCol + (x + 0);

			D3DXVECTOR3 vLeftToRight = m_vecVertex[right] - m_vecVertex[left];
			D3DXVECTOR3 vDownToUp = m_vecVertex[up] - m_vecVertex[down];
			D3DXVECTOR3 vNormal;
			D3DXVec3Cross(&vNormal, &vDownToUp, &vLeftToRight);
			D3DXVec3Normalize(&vNormal, &vNormal);

			int nIndex = z * nCol + x;
			vecVertex[nIndex].n = vNormal;
		}
	}

	std::vector<DWORD> vecIndex;
	for (int x = 0; x < m_nTileCount; x++)
	{
		for (int z = 0; z < m_nTileCount; z++)
		{
			int _0 = (z + 0) * nCol + (x + 0);
			int _1 = (z + 1) * nCol + (x + 0);
			int _2 = (z + 0) * nCol + (x + 1);
			int _3 = (z + 1) * nCol + (x + 1);

			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);

			vecIndex.push_back(_3);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
		}
	}

	D3DXCreateMeshFVF(vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PTN_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PTN_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (void**)&pV);
	memcpy(pV, &vecVertex[0],
		vecVertex.size() * sizeof(ST_PTN_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (void**)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);
	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);
}

void cHeightMap::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMesh->DrawSubset(0);
}

bool cHeightMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	if (x < 0.0f || z < 0.0f
		|| x >= (m_nTileCount + 1) || z >= (m_nTileCount + 1))
	{
		return false;
	}

	int nX = x;
	int nZ = z;

	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;


	int _0 = (nZ + 0) * (m_nTileCount + 1) + (nX + 0);
	int _1 = (nZ + 1) * (m_nTileCount + 1) + (nX + 0);
	int _2 = (nZ + 0) * (m_nTileCount + 1) + (nX + 1);
	int _3 = (nZ + 1) * (m_nTileCount + 1) + (nX + 1);

	//_1.._3
	//.
	//.
	//_0.._2

	// ¾Æ·¡ »ï°¢Çü
	if (fDeltaX + fDeltaZ < 1.0f)
	{
		D3DXVECTOR3 v01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 v02 = m_vecVertex[_2] - m_vecVertex[_0];
		y = (v01 * fDeltaZ + v02 * fDeltaX).y + m_vecVertex[_0].y;
		return true;
	}
	// À§ »ï°¢Çü
	else
	{
		fDeltaX = 1.0f - fDeltaX;
		fDeltaZ = 1.0f - fDeltaZ;
		D3DXVECTOR3 v31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 v32 = m_vecVertex[_2] - m_vecVertex[_3];
		y = (v31 * fDeltaX + v32 * fDeltaZ).y + m_vecVertex[_3].y;
		return true;
	}

	return false;
}
