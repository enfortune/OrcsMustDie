#include "stdafx.h"
#include "cInGameUIScore.h"
#include "cGameUISprite.h"
#include "cGameUILabel.h"
#include "cGameText.h"

#include "cPlayer.h"

cInGameUIScore::cInGameUIScore()
	: m_pPlayer(nullptr)
	, m_nScore(0)
	, m_pSpriteBack(nullptr)
	, m_pLabel(nullptr)
	, m_vPos(0.f, 0.f)
{
}


cInGameUIScore::~cInGameUIScore()
{
	this->Delete();
}

void cInGameUIScore::Setup(cPlayer * pPlayer)
{
	cGameNode::Setup(false);

	m_pPlayer = pPlayer;

	m_pSpriteBack = new cGameUISprite;
	m_pSpriteBack->Setup("Resource/Image/UI/score.png");
	this->AddChild(m_pSpriteBack);

	m_pLabel = new cGameUILabel;
	m_pLabel->Setup(cFontManager::FT_INGAMEUI_SCORE, std::to_string(m_nScore));
	m_pLabel->SetPosition(D3DXVECTOR2(24, 16));
	m_pLabel->GetText()->SetColor(D3DXCOLOR(0.75f, 0.75f, 1.0f, 1.0f));
	m_pSpriteBack->AddChild(m_pLabel);
}
void cInGameUIScore::Update(float fDelta)
{
	m_nScore = m_pPlayer->GetPlayerGold();
	m_pLabel->SetText(std::to_string(m_nScore));

	cGameNode::Update(fDelta);
}
void cInGameUIScore::Render()
{
	cGameNode::Render();
}
void cInGameUIScore::Delete()
{
	SAFE_RELEASE(m_pSpriteBack);
	SAFE_RELEASE(m_pLabel);
}
D3DXMATRIXA16 cInGameUIScore::GetMatirixToParent()
{
	D3DXMATRIXA16 matRet, matT;
	D3DXMatrixIdentity(&matRet);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, 0.0f);
	matRet = matT;
	return matRet;
}