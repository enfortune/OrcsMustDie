#include "stdafx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cAseNode.h"


cAseLoader::cAseLoader()
	: m_fp(NULL)
	, m_pRoot(NULL)
	, m_dwFirstFrame(0)
	, m_dwLastFrame(0)
	, m_dwTicksPerFrame(0)
{
}


cAseLoader::~cAseLoader()
{
}

cAseNode* cAseLoader::Load(char * szFullPath)
{
	fopen_s(&m_fp, szFullPath, "r");

	while (char* szToken = GetToken())
	{
		if (IsEqual(szToken, ID_SCENE))
		{
			ProcessScene();
		}
		else if (IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (IsEqual(szToken, ID_GEOMETRY))
		{
			cAseNode* pNode = ProcessGEOMETRY();
			if (!m_pRoot)
			{
				m_pRoot = pNode;
				SetSceneInfo(m_pRoot);
			}
		}
	}

	fclose(m_fp);

	for each (auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
	m_vecMtlTex.clear();

	m_pRoot->CalcOriginalLocalTM(NULL);

	return m_pRoot;
}

char * cAseLoader::GetToken()
{
	int nReadCnt = 0;
	bool isQuote = false;

	while (true)
	{
		if (feof(m_fp))
			break;

		char c = fgetc(m_fp);

		if (c == '\"')
		{
			if (isQuote)
				break;

			isQuote = true;
			continue;
		}

		if (!isQuote && IsWhite(c))
		{
			if (nReadCnt == 0)
				continue;
			else
				break;
		}

		m_szToken[nReadCnt++] = c;
	}

	if (nReadCnt == 0)
		return NULL;

	m_szToken[nReadCnt] = '\0';

	return m_szToken;
}

int cAseLoader::GetInteager()
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
	return (float)atof(GetToken());
}

bool cAseLoader::IsWhite(char c)
{
	return c < 33;
}

bool cAseLoader::IsEqual(char * str1, char * str2)
{
	return strcmp(str1, str2) == 0;
}

void cAseLoader::ProcessScene()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_FIRSTFRAME))
		{
			m_dwFirstFrame = GetInteager();
		}
		else if (IsEqual(szToken, ID_LASTFRAME))
		{
			m_dwLastFrame = GetInteager();
		}
		else if (IsEqual(szToken, ID_TICKSPERFRAME))
		{
			m_dwTicksPerFrame = GetInteager();
		}

	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MATERIAL_COUNT))
		{
			for each(auto p in m_vecMtlTex)
			{
				SAFE_RELEASE(p);
			}
			m_vecMtlTex.resize(GetInteager());
		}
		else if (IsEqual(szToken, ID_MATERIAL))
		{
			int nIndex = GetInteager();
			m_vecMtlTex[nIndex] = new cMtlTex;
			ProcessMaterial(m_vecMtlTex[nIndex]);
		}

	} while (nLevel > 0);
}

cAseNode* cAseLoader::ProcessGEOMETRY()
{
	cAseNode* pAseNode = new cAseNode;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_NODE_NAME))
		{
			m_mapNode[GetToken()] = pAseNode;
		}
		else if (IsEqual(szToken, ID_NODE_PARENT))
		{
			m_mapNode[GetToken()]->AddChild(pAseNode);
		}
		else if (IsEqual(szToken, ID_NODE_TM))
		{
			ProcessNODE_TM(pAseNode);
		}
		else if (IsEqual(szToken, ID_MESH))
		{
			ProcessMESH(pAseNode);
		}
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			int nMtlIndex = GetInteager();
			pAseNode->SetMtlTex(m_vecMtlTex[nMtlIndex]);
		}
		else if (IsEqual(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pAseNode);
		}

	} while (nLevel > 0);

	return pAseNode;
}

void cAseLoader::ProcessNODE_TM(cAseNode* pNode)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
if (IsEqual(szToken, "{"))
{
	nLevel++;
}
else if (IsEqual(szToken, "}"))
{
	nLevel--;
}
else if (IsEqual(szToken, ID_TM_ROW0))
{
	matWorld._11 = GetFloat();
	matWorld._13 = GetFloat();
	matWorld._12 = GetFloat();
	matWorld._14 = 0.0f;
}
else if (IsEqual(szToken, ID_TM_ROW1))
{
	matWorld._31 = GetFloat();
	matWorld._33 = GetFloat();
	matWorld._32 = GetFloat();
	matWorld._34 = 0.0f;
}
else if (IsEqual(szToken, ID_TM_ROW2))
{
	matWorld._21 = GetFloat();
	matWorld._23 = GetFloat();
	matWorld._22 = GetFloat();
	matWorld._24 = 0.0f;
}
else if (IsEqual(szToken, ID_TM_ROW3))
{
	matWorld._41 = GetFloat();
	matWorld._43 = GetFloat();
	matWorld._42 = GetFloat();
	matWorld._44 = 1.0f;
}

	} while (nLevel > 0);

	pNode->SetWorldTM(matWorld);
}

void cAseLoader::ProcessMESH(cAseNode* pNode)
{
	std::vector<D3DXVECTOR3>	vecV;
	std::vector<D3DXVECTOR2>	vecVT;
	std::vector<D3DXVECTOR3>	vecVN;
	std::vector<ST_PTN_VERTEX>	vecVertex;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_NUMVERTEX))
		{
			vecV.resize(GetInteager());
		}
		else if (IsEqual(szToken, ID_MESH_NUMFACES))
		{
			vecVertex.resize(GetInteager() * 3);
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX_LIST))
		{
			ProcessMESH_VERTEX_LIST(vecV);
		}
		else if (IsEqual(szToken, ID_MESH_FACE_LIST))
		{
			ProcessMESH_FACE_LIST(vecVertex, vecV);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))
		{
			vecVT.resize(GetInteager());
		}
		else if (IsEqual(szToken, ID_MESH_TVERTLIST))
		{
			ProcessMESH_TVERTLIST(vecVT);
		}
		else if (IsEqual(szToken, ID_MESH_TFACELIST))
		{
			ProcessMESH_TFACELIST(vecVertex, vecVT);
		}
		else if (IsEqual(szToken, ID_MESH_NORMALS))
		{
			ProcessMESH_NORMALS(vecVertex);
		}

	} while (nLevel > 0);

	// world vertex -> local vertex
	D3DXMATRIXA16 matWorldInv;
	D3DXMatrixInverse(&matWorldInv, 0, &pNode->GetWorldTM());

	//for each(auto p in vecVertex)
	//{
	//	D3DXVec3TransformCoord(&p.p, &p.p, &matWorldInv);
	//	int a = 0;
	//}

	for (size_t i = 0; i < vecVertex.size(); i++)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matWorldInv);
	}

	//pNode->SetVertex(vecVertex);
	pNode->BuildVB(vecVertex);
}

void cAseLoader::ProcessTM_ANIMATION(cAseNode* node)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_POS_TRACK))
		{
			ProcessPOS_TRACK(node);
		}
		else if (IsEqual(szToken, ID_ROT_TRACK))
		{
			ProcessROT_TRACK(node);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessPOS_TRACK(cAseNode* node)
{
	std::vector<ST_POS_SAMPLE>	vecPosSample;
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_POS_SAMPLE))
		{
			ST_POS_SAMPLE stPosSample;
			stPosSample.frame = GetInteager();
			stPosSample.v.x = GetFloat();
			stPosSample.v.z = GetFloat();
			stPosSample.v.y = GetFloat();
			vecPosSample.push_back(stPosSample);
		}
	} while (nLevel > 0);

	node->SetPosTrack(vecPosSample);
}

void cAseLoader::ProcessROT_TRACK(cAseNode* node)
{
	std::vector<ST_ROT_SAMPLE>	vecRotSample;
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_ROT_SAMPLE))
		{
			ST_ROT_SAMPLE stRotSample;
			stRotSample.frame = GetInteager();

			stRotSample.q.x = GetFloat();
			stRotSample.q.z = GetFloat();
			stRotSample.q.y = GetFloat();
			stRotSample.q.w = GetFloat();

			//stRotSample.q.x *= sinf(stRotSample.q.w / 2.0f);
			//stRotSample.q.y *= sinf(stRotSample.q.w / 2.0f);
			//stRotSample.q.z *= sinf(stRotSample.q.w / 2.0f);
			//stRotSample.q.w = cosf(stRotSample.q.w / 2.0f);

			D3DXQuaternionRotationAxis(&stRotSample.q,
				&D3DXVECTOR3(stRotSample.q.x, stRotSample.q.y, stRotSample.q.z),
				stRotSample.q.w);

			// rot ´©Àû
			if (!vecRotSample.empty())
			{
				stRotSample.q = vecRotSample.back().q * stRotSample.q;
			}

			vecRotSample.push_back(stRotSample);
		}
	} while (nLevel > 0);

	node->SetRotTrack(vecRotSample);
}

void cAseLoader::ProcessMESH_VERTEX_LIST(std::vector<D3DXVECTOR3>& vecV)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX))
		{
			int nIndex = GetInteager();
			vecV[nIndex].x = GetFloat();
			vecV[nIndex].z = GetFloat();
			vecV[nIndex].y = GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST(
	std::vector<ST_PTN_VERTEX>& vecVertex,
	std::vector<D3DXVECTOR3> vecV)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_FACE))
		{
			int nIndex = GetInteager();
			GetToken();
			vecVertex[nIndex * 3 + 0].p = vecV[GetInteager()];
			GetToken();
			vecVertex[nIndex * 3 + 2].p = vecV[GetInteager()];
			GetToken();
			vecVertex[nIndex * 3 + 1].p = vecV[GetInteager()];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTLIST(std::vector<D3DXVECTOR2>& vecVT)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_TVERT))
		{
			int nIndex = GetInteager();
			vecVT[nIndex].x = GetFloat();
			vecVT[nIndex].y = 1.0f - GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACELIST(std::vector<ST_PTN_VERTEX>& vecVertex, std::vector<D3DXVECTOR2> vecVT)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_TFACE))
		{
			int nIndex = GetInteager();
			vecVertex[nIndex * 3 + 0].t = vecVT[GetInteager()];
			vecVertex[nIndex * 3 + 2].t = vecVT[GetInteager()];
			vecVertex[nIndex * 3 + 1].t = vecVT[GetInteager()];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS(std::vector<ST_PTN_VERTEX>& vecVertex)
{
	int arrIndex[3] = { 0, 2, 1 };
	int nSubIndex = 0;
	int nIndex = 0;
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_MESH_FACENORMAL))
		{
			nIndex = GetInteager();
			nSubIndex = 0;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
		{
			GetToken();

			D3DXVECTOR3 n;
			n.x = GetFloat();
			n.z = GetFloat();
			n.y = GetFloat();

			vecVertex[nIndex * 3 + arrIndex[nSubIndex++]].n = n;
		}
	} while (nLevel > 0);
}

void cAseLoader::SetSceneInfo(cAseNode * pRoot)
{
	pRoot->m_dwFirstFrame = m_dwFirstFrame;
	pRoot->m_dwLastFrame = m_dwLastFrame;
	pRoot->m_dwTicksPerFrame = m_dwTicksPerFrame;
}

void cAseLoader::ProcessMaterial(cMtlTex * pMtlTex)
{
	D3DMATERIAL9	stMtl;
	ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_AMBIENT))
		{
			stMtl.Ambient.r = GetFloat();
			stMtl.Ambient.g = GetFloat();
			stMtl.Ambient.b = GetFloat();
			stMtl.Ambient.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_DIFFUSE))
		{
			stMtl.Diffuse.r = GetFloat();
			stMtl.Diffuse.g = GetFloat();
			stMtl.Diffuse.b = GetFloat();
			stMtl.Diffuse.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_SPECULAR))
		{
			stMtl.Specular.r = GetFloat();
			stMtl.Specular.g = GetFloat();
			stMtl.Specular.b = GetFloat();
			stMtl.Specular.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}

	} while (nLevel > 0);

	pMtlTex->SetMaterial(stMtl);
}

void cAseLoader::ProcessMAP_DIFFUSE(cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, ID_BITMAP))
		{
			szToken = GetToken();
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(szToken));
		}

	} while (nLevel > 0);
}
