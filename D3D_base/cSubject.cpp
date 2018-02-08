#include "stdafx.h"
#include "cSubject.h"
#include "cSkinnedMeshEX.h"

cSubject::cSubject()
	: m_vZPos(0, 0, 0)
	, m_vZScl(0, 0, 0)
	, m_vAPos(0, 0, 0)
	, m_nAnduinHp(0)
{
}

cSubject::~cSubject()
{
	Delete();
}

void cSubject::Setup()
{
	cGameNode::Setup();

	m_pZeppelin = new cSkinnedMeshEX;
	m_pZeppelin->Setup("Resource/XFile/Object", "Resource/XFile/Object/zeppelin.X");
	m_pZeppelin->SetAnimationSet(0, 0);

	m_pAnduin = new cSkinnedMeshEX;
	m_pAnduin->Setup("Resource/XFile/Object", "Resource/XFile/Object/anduin.X");
	m_pAnduin->SetAnimationSet(0, 0);

	m_nAnduinHp = 300;
}

void cSubject::Update(float fDelta)
{
	m_pZeppelin->Update(fDelta);
	m_pAnduin->Update(fDelta);

	cGameNode::Update(fDelta);
}

void cSubject::Render()
{

	if (m_pZeppelin)
	{
		D3DXMATRIXA16 matT, matS, matWorld;
		D3DXMatrixTranslation(&matT, m_vZPos.x, m_vZPos.y, m_vZPos.z);
		D3DXMatrixScaling(&matS, m_vZScl.x, m_vZScl.y, m_vZScl.z);

		matWorld = matS * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pZeppelin->Render();
	}

	if (m_pAnduin)
	{
		D3DXMATRIXA16 matT, matR, matWorld;
		D3DXMatrixTranslation(&matT, m_vAPos.x, m_vAPos.y, m_vAPos.z);
		D3DXMatrixRotationY(&matR, D3DX_PI);
		matWorld = matR * matT;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pAnduin->Render();
	}

	cGameNode::Render();
}

void cSubject::Delete()
{
	SAFE_DELETE(m_pZeppelin);
	SAFE_DELETE(m_pAnduin);
}

void cSubject::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
