#include "stdafx.h"
#include "cMapToolScene.h"

#include "cMapToolUILayer.h"
#include "cMapData.h"
#include "cGrid.h"

cMapToolScene::cMapToolScene()
{
}

cMapToolScene::~cMapToolScene()
{
	this->Delete();
}
void cMapToolScene::Setup()
{
	cGameScene::Setup();

	m_pUILayer = new cMapToolUILayer;
	m_pUILayer->Setup();
	this->AddChild(m_pUILayer);
}
void cMapToolScene::Update(float fDelta)
{


	cGameScene::Update(fDelta);
}
void cMapToolScene::Render()
{

	cGameScene::Render();

}
void cMapToolScene::Delete()
{
	SAFE_RELEASE(m_pUILayer);
}
void cMapToolScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}