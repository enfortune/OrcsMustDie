#include "stdafx.h"
#include "cMapToolGrid.h"


cMapToolGrid::cMapToolGrid()
	: m_pVB(NULL)
	, m_fHeight(0.f)
{
}


cMapToolGrid::~cMapToolGrid()
{
	SAFE_RELEASE(m_pVB);
}

void cMapToolGrid::Setup(int nNumTile, float fInterval)
{
	float fMax = nNumTile * fInterval;
	float fMin = 0.f;

	ST_PC_VERTEX v;

	for (int i = 1; i <= nNumTile; i++)
	{
		if (i % 5 == 0)
			v.c = D3DCOLOR_XRGB(255, 255, 255);
		else
			v.c = D3DCOLOR_XRGB(128, 128, 128);

		v.p = D3DXVECTOR3(fMin, 0, i * fInterval);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, i * fInterval);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(fMin, 0, 0);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, 0);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(i * fInterval, 0, fMin);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(i * fInterval, 0, fMax);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(0, 0, fMin);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(0, 0, fMax);
		m_vecVertex.push_back(v);
	}

	v.c = D3DCOLOR_XRGB(255, 0, 0);
	v.p = D3DXVECTOR3(fMin, 0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(fMax, 0, 0); m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 255, 0);
	v.p = D3DXVECTOR3(0, fMin, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, fMax, 0); m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 0, 255);
	v.p = D3DXVECTOR3(0, 0, fMin); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, 0, fMax); m_vecVertex.push_back(v);

	g_pD3DDevice->CreateVertexBuffer(
		m_vecVertex.size() * sizeof(ST_PC_VERTEX),
		0,
		ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	ST_PC_VERTEX* pV = NULL;
	m_pVB->Lock(0, 0, (void**)&pV, 0);

	memcpy(pV, &m_vecVertex[0],
		m_vecVertex.size() * sizeof(ST_PC_VERTEX));

	m_pVB->Unlock();

}

void cMapToolGrid::Render()
{
	D3DXMATRIXA16	matWorld, matT;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matT, 0.f, m_fHeight, 0.f);
	matWorld = matT;
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	//g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
	//	m_vecVertex.size() / 2,
	//	&m_vecVertex[0],
	//	sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetStreamSource(0,
		m_pVB, 0, sizeof(ST_PC_VERTEX));

	g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST,
		0, m_vecVertex.size() / 2);
}
