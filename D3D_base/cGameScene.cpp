#include "stdafx.h"
#include "cGameScene.h"


cGameScene::cGameScene()
	: m_bUpdateEnable(false)
{
}


cGameScene::~cGameScene()
{
}

void cGameScene::Setup()
{
	cGameNode::Setup();
}
void cGameScene::Update(float fDelta)
{
	cGameNode::Setup();
}
void cGameScene::Render()
{
	cGameNode::Render();
}
void cGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}


