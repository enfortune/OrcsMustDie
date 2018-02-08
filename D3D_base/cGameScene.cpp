#include "stdafx.h"
#include "cGameScene.h"


cGameScene::cGameScene()
	: m_bUpdateEnable(false)
{
}


cGameScene::~cGameScene()
{
	this->Delete();
}

void cGameScene::Setup()
{
	cGameNode::Setup(false); // not use TransformData
}
void cGameScene::Update(float fDelta)
{
	cGameNode::Update(fDelta);

}
void cGameScene::Render()
{
	cGameNode::Render();
}
void cGameScene::Delete()
{
}
void cGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}
RECT cGameScene::GetScreenRect()
{
	RECT rcRet;
	GetClientRect(g_hWnd, &rcRet);
	return rcRet;
}

