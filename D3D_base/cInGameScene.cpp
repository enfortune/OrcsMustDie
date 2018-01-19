#include "stdafx.h"
#include "cInGameScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cInGameUILayer.h"

#include "cSkinnedMesh.h"

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

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);
	m_pGrid = new cGrid;
	m_pGrid->Setup();

	float fScaleX = static_cast<float>(SCREEN_WIDTH) / 1920.f;
	float fScaleY = static_cast<float>(SCREEN_HEIGHT) / 1080.f;

	m_pUILayer = new cInGameUILayer;
	m_pUILayer->Setup(NULL);
	m_pUILayer->SetScale(D3DXVECTOR2(fScaleX, fScaleY));
	this->AddChild(m_pUILayer);

	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup("XFileSample","XFileSample/sylva.X");

	m_pSkinnedMesh->SetAnimationSet(0, 6);

}
void cInGameScene::Update(float fDelta)
{
	m_pCamera->Update();

	m_pSkinnedMesh->Update();
	m_pSkinnedMesh->UpdateAnimation(fDelta);
	cGameScene::Update(fDelta);

	
}
void cInGameScene::Render()
{
	m_pGrid->Render();

	m_pSkinnedMesh->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	cGameScene::Render();
}
void cInGameScene::Delete()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pUILayer);
	SAFE_DELETE(m_pSkinnedMesh);

}
void cInGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCamera->WndProc(hWnd, message, wParam, lParam);
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}