#include "stdafx.h"
#include "cGameUILayer.h"


cGameUILayer::cGameUILayer()
	: m_vPos(D3DXVECTOR2(0.0f, 0.0f))
	, m_fRotAngle(0.0f)
	, m_vScale(D3DXVECTOR2(1.0f, 1.0f))
{
}


cGameUILayer::~cGameUILayer()
{
	this->Delete();
}

void cGameUILayer::Setup()
{
	cGameNode::Setup(false);
}
void cGameUILayer::Update(float fDelta)
{
	cGameNode::Update(fDelta);
}
void cGameUILayer::Render()
{
	cGameNode::Render();
}
void cGameUILayer::Delete()
{
	
}

void cGameUILayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}

D3DXMATRIXA16 cGameUILayer::GetMatirixToParent()
{
	D3DXMATRIXA16 matRet, matS, matR, matT;
	D3DXMatrixIdentity(&matRet);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, 0.0f);
	//D3DXMatrixRotationAxis(&matR, &m_vAxis, m_fRotAngle); // use Axis Z
	D3DXMatrixRotationZ(&matR, m_fRotAngle);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, 0.0f);

	matRet = matS * matR * matT;

	return matRet;
}