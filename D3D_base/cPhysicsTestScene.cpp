#include "stdafx.h"
#include "cPhysicsTestScene.h"

#include "cMapData.h"

cPhysicsTestScene::cPhysicsTestScene()
	: m_pMapData(nullptr)
{
}

cPhysicsTestScene::~cPhysicsTestScene()
{
	this->Delete();
}

void cPhysicsTestScene::Setup()
{
	cGameScene::Setup();

	m_pMapData = new cMapData;
	m_pMapData->Setup();
}
void cPhysicsTestScene::Update(float fDelta)
{



	cGameScene::Update(fDelta);
}
void cPhysicsTestScene::Render()
{



	cGameScene::Render();
}
void cPhysicsTestScene::Delete()
{
	SAFE_RELEASE(m_pMapData);
}

void cPhysicsTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	cGameScene::WndProc(hWnd, message, wParam, lParam);
}