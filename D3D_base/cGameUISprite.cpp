#include "stdafx.h"
#include "cGameUISprite.h"
#include "cGameSprite.h"

cGameUISprite::cGameUISprite()
	: m_pSprite(nullptr)
	, m_vPos(0.0f, 0.0f)
	, m_fRotAngle(0.0f)
	, m_vScale(1.0f, 1.0f)
{
}


cGameUISprite::~cGameUISprite()
{
	this->Delete();
}

void cGameUISprite::Setup(std::string sFileName)
{
	cGameNode::Setup();

	m_pSprite = new cGameSprite;
	m_pSprite->Setup(sFileName);
}
void cGameUISprite::Render()
{
	m_pSprite->Render(&this->GetMatrixToWorld());

	cGameNode::Render();
}
void cGameUISprite::Delete()
{
	SAFE_DELETE(m_pSprite);
}

std::vector<D3DXVECTOR2> cGameUISprite::GetBoundingBox()
{
	return m_pSprite->GetBoundingBox(&this->GetMatrixToWorld());
}
D3DXMATRIXA16 cGameUISprite::GetMatirixToParent()
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