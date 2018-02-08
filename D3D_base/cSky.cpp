#include "stdafx.h"
#include "cSky.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"

cSky::cSky()
	: m_vPos(0, 0, 0)
	, m_vScl(0, 0, 0)
{
}

cSky::~cSky()
{
	Delete();
}

void cSky::Setup()
{
	cGameNode::Setup();

	m_pskyUp = new cSkinnedMeshEX;
	m_pskyUp->Setup("Resource/XFile/Map", "Resource/XFile/Map/SkyWorld.X");
	m_pskyUp->SetAnimationSet(0, 0);

	m_pskyDown = new cSkinnedMeshEX;
	m_pskyDown->Setup("Resource/XFile/Map", "Resource/XFile/Map/SkyWorld.X");
	m_pskyDown->SetAnimationSet(0, 0);

}

void cSky::Update(float fDelta)
{

	cGameNode::Update(fDelta);
}

void cSky::Render()
{

	if (m_pskyUp)
	{
		D3DXMATRIXA16 matT, matS, matWorld;
		D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
		D3DXMatrixScaling(&matS, m_vScl.x, m_vScl.y, m_vScl.z);

		matWorld = matS * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pskyUp->Render();
	}

	if (m_pskyDown)
	{
		D3DXMATRIXA16 matT, matS, matR, matWorld;
		D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);
		D3DXMatrixScaling(&matS, m_vScl.x, m_vScl.y, m_vScl.z);
		D3DXMatrixRotationX(&matR, D3DX_PI);
		matWorld = matS * matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pskyDown->Render();
	}

	cGameNode::Render();
}

void cSky::Delete()
{
	SAFE_DELETE(m_pskyUp);
	SAFE_DELETE(m_pskyDown);

}

void cSky::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}
