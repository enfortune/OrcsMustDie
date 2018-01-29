#include "stdafx.h"
#include "cInGameUIGauge.h"
#include "cGameSprite.h"


cInGameUIGauge::cInGameUIGauge()
	: m_pPlayer(nullptr)
	, m_fRatioHP(1.0f)
	, m_fRatioMP(1.0f)
	, m_pSpriteGaugeBack(nullptr)
	, m_pSpriteHPFront(nullptr)
	, m_pSpriteMPFront(nullptr)
	, m_vPos(D3DXVECTOR2(0.0f, 0.0f))
	, m_fRotAngle(0.0f)
	, m_vScale(D3DXVECTOR2(1.0f, 1.0f))
{
}

cInGameUIGauge::~cInGameUIGauge()
{
	this->Delete();
}


void cInGameUIGauge::UpdateRatio()
{
	// TODO: 플레이어 클래스 틀 잡히면 코드 추가할 것

	m_fRatioHP = 0.5f;
	m_fRatioMP = 0.7f;
}

void cInGameUIGauge::Setup(void* pPlayer)
{
	cGameNode::Setup(false);

	m_pPlayer = pPlayer;

	m_pSpriteGaugeBack = new cGameSprite;
	m_pSpriteGaugeBack->Setup("Resource/Image/UI/gauge_back.png");
	m_pSpriteGaugeBack->SetZorder(0.1f);
	m_pSpriteHPFront = new cGameSprite;
	m_pSpriteHPFront->Setup("Resource/Image/UI/hp_front.png");
	m_pSpriteHPFront->SetZorder(0.0f);
	m_pSpriteHPFront->SetPosition(D3DXVECTOR2(-14, -23));
	m_pSpriteMPFront = new cGameSprite;
	m_pSpriteMPFront->Setup("Resource/Image/UI/mp_front.png");
	m_pSpriteMPFront->SetZorder(0.0f);
	m_pSpriteMPFront->SetPosition(D3DXVECTOR2(48, 21));

}
void cInGameUIGauge::Update(float fDelta)
{
	UpdateRatio();
	
	m_pSpriteHPFront->SetCropRect(0.f, 0.f, m_fRatioHP, 1.f);
	m_pSpriteMPFront->SetCropRect(0.f, 0.f, m_fRatioMP, 1.f);

	cGameNode::Update(fDelta);
}
void cInGameUIGauge::Render()
{
	m_pSpriteGaugeBack->Render(&this->GetMatrixToWorld());
	m_pSpriteHPFront->Render(&this->GetMatrixToWorld());
	m_pSpriteMPFront->Render(&this->GetMatrixToWorld());


	cGameNode::Render();
}
void cInGameUIGauge::Delete()
{
	SAFE_DELETE(m_pSpriteGaugeBack)
	SAFE_DELETE(m_pSpriteHPFront)
	SAFE_DELETE(m_pSpriteMPFront)
}


D3DXMATRIXA16 cInGameUIGauge::GetMatirixToParent()
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