#include "stdafx.h"
#include "cAseNode.h"
#include "cMtlTex.h"


cAseNode::cAseNode()
	: m_pMtlTex(NULL)
	, m_pVB(NULL)
	, m_nNumTri(0)
{
}


cAseNode::~cAseNode()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
}

void cAseNode::Update(int nFrame, D3DXMATRIXA16* pMatParent)
{
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	CalcLocalR(matR, nFrame);
	CalcLocalT(matT, nFrame);

	m_matLocalTM = matR * matT;
	m_matWorldTM = m_matLocalTM;
	if (pMatParent)
	{
		m_matWorldTM = m_matLocalTM * (*pMatParent);
	}

	for each (auto child in m_vecChild)
	{
		child->Update(nFrame, &m_matWorldTM);
	}
}

void cAseNode::Render()
{
	if (m_pMtlTex)
	{
		//D3DXMatrixIdentity(&m_matWorldTM);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetFVF(ST_PTN_VERTEX::FVF);
		/*g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PTN_VERTEX));*/

		g_pD3DDevice->SetStreamSource(0, m_pVB,
			0, sizeof(ST_PTN_VERTEX));
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,
			0, m_nNumTri);
	}

	for each(auto child in m_vecChild)
	{
		child->Render();
	}
}

void cAseNode::AddChild(cAseNode * pChild)
{
	m_vecChild.push_back(pChild);
}

void cAseNode::Destroy()
{
	for each(auto child in m_vecChild)
	{
		child->Destroy();
	}
	this->Release();
	//SAFE_RELEASE(this);
}

void cAseNode::CalcOriginalLocalTM(D3DXMATRIXA16* pMatParent)
{
	m_matLocalTM = m_matWorldTM;
	if (pMatParent)
	{
		D3DXMATRIXA16 matInv;
		D3DXMatrixInverse(&matInv, 0, pMatParent);
		m_matLocalTM = m_matWorldTM * matInv;
	}

	for each(auto child in m_vecChild)
	{
		child->CalcOriginalLocalTM(&m_matWorldTM);
	}
}

void cAseNode::CalcLocalR(D3DXMATRIXA16 & matR, int nFrame)
{
	// Rotation
	if (m_vecRotTrack.empty())
	{
		matR = m_matLocalTM;
		matR._41 = matR._42 = matR._43 = 0.0f;
		return;
	}

	if (nFrame <= m_vecRotTrack.front().frame)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.front().q);
		return;
	}

	if (nFrame >= m_vecRotTrack.back().frame)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.back().q);
		return;
	}

	int nIndex = 0;
	for (size_t i = 0; i < m_vecRotTrack.size(); i++)
	{
		if (nFrame < m_vecRotTrack[i].frame)
		{
			nIndex = i;
			break;
		}
	}

	float t = (nFrame - m_vecRotTrack[nIndex - 1].frame) /
		(float)(m_vecRotTrack[nIndex].frame - m_vecRotTrack[nIndex - 1].frame);

	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q,
		&m_vecRotTrack[nIndex - 1].q,
		&m_vecRotTrack[nIndex].q,
		t);

	D3DXMatrixRotationQuaternion(&matR, &q);
}

void cAseNode::CalcLocalT(D3DXMATRIXA16 & matT, int nFrame)
{
	D3DXMatrixIdentity(&matT);
	if (m_vecPosTrack.empty())
	{
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
		return;
	}

	if (nFrame <= m_vecPosTrack.front().frame)
	{
		matT._41 = m_vecPosTrack.front().v.x;
		matT._42 = m_vecPosTrack.front().v.y;
		matT._43 = m_vecPosTrack.front().v.z;
		return;
	}

	if (nFrame >= m_vecPosTrack.back().frame)
	{
		matT._41 = m_vecPosTrack.back().v.x;
		matT._42 = m_vecPosTrack.back().v.y;
		matT._43 = m_vecPosTrack.back().v.z;
		return;
	}

	int nIndex = 0;
	for (size_t i = 0; i < m_vecPosTrack.size(); i++)
	{
		if (nFrame < m_vecPosTrack[i].frame)
		{
			nIndex = i;
			break;
		}
	}

	float t = (nFrame - m_vecPosTrack[nIndex - 1].frame) /
		(float)(m_vecPosTrack[nIndex].frame - m_vecPosTrack[nIndex - 1].frame);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v,
		&m_vecPosTrack[nIndex - 1].v,
		&m_vecPosTrack[nIndex].v,
		t);


	matT._41 = v.x;
	matT._42 = v.y;
	matT._43 = v.z;
}

int cAseNode::GetFrame()
{
	int nFirst = m_dwFirstFrame * m_dwTicksPerFrame;
	int nLast = m_dwLastFrame * m_dwTicksPerFrame;
	
	return GetTickCount() % (nLast - nFirst) + nFirst;
}

void cAseNode::BuildVB(std::vector<ST_PTN_VERTEX>& vecVertex)
{
	m_nNumTri = vecVertex.size() / 3;

	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PTN_VERTEX),
		0,
		ST_PTN_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	ST_PTN_VERTEX* pV = NULL;
	m_pVB->Lock(0, 0, (void**)&pV, 0);

	memcpy(pV, &vecVertex[0],
		vecVertex.size() * sizeof(ST_PTN_VERTEX));

	m_pVB->Unlock();
}
