#include "stdafx.h"
#include "cInGameScene.h"
#include "cInGameUILayer.h"

#define SCREEN_WIDTH GetRectWidth(GetScreenRect())
#define SCREEN_HEIGHT GetRectHeight(GetScreenRect())

cInGameScene::cInGameScene()
	: m_pUILayer(NULL)
{
}

cInGameScene::~cInGameScene()
{
	this->Delete();
}

void cInGameScene::Setup()
{
	cGameScene::Setup();

	float fScaleX = static_cast<float>(SCREEN_WIDTH) / 1920.f;
	float fScaleY = static_cast<float>(SCREEN_HEIGHT) / 1080.f;

	m_pUILayer = new cInGameUILayer;
	m_pUILayer->Setup(NULL);
	m_pUILayer->SetScale(D3DXVECTOR2(fScaleX, fScaleY));
	this->AddChild(m_pUILayer);
}
void cInGameScene::Update(float fDelta)
{


	cGameScene::Update(fDelta);
}
void cInGameScene::Render()
{


	cGameScene::Render();
}
void cInGameScene::Delete()
{
	SAFE_RELEASE(m_pUILayer);
}
void cInGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}