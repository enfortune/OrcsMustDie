#include "stdafx.h"
#include "cInGameUILayer.h"

#include "cInGameUIGauge.h"
#include "cInGameUIScore.h"

cInGameUILayer::cInGameUILayer()
	: m_pUIGauge(nullptr)
	, m_pUIScore(nullptr)
{
}


cInGameUILayer::~cInGameUILayer()
{
	this->Delete();
}

void cInGameUILayer::Setup(cPlayer * pPlayer)
{
	cGameUILayer::Setup();

	// TODO: 게임스페이스노드쪽 구현되면 실제 포인터 연결할 것

	m_pUIGauge = new cInGameUIGauge;
	m_pUIGauge->Setup(pPlayer);
	m_pUIGauge->SetPosition(D3DXVECTOR2(220, 64));
	this->AddChild(m_pUIGauge);

	m_pUIScore = new cInGameUIScore;
	m_pUIScore->Setup(nullptr/*m_pTarget->GetScoreBoard*/);
	m_pUIScore->SetPosition(D3DXVECTOR2(155, 1080 - 57));
	this->AddChild(m_pUIScore);
}

void cInGameUILayer::Update(float fDelta)
{


	cGameUILayer::Update();
}
void cInGameUILayer::Render()
{


	cGameUILayer::Render();
}
void cInGameUILayer::Delete()
{
	SAFE_RELEASE(m_pUIGauge);
	SAFE_RELEASE(m_pUIScore);
} 
void cInGameUILayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameUILayer::WndProc(hWnd, message, wParam, lParam);
}

/* Delegate */

bool cInGameUILayer::UIButtonAction(std::string sTag, BUTTON_STATE enState)
{
	return true;
}
