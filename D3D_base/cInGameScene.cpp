#include "stdafx.h"
#include "cInGameScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cInGameUILayer.h"
#include "cPlayerCamera.h"
#include "cPlayer.h"
#include "cPhysicsNode.h"
#include "cPhysicsBody.h"
#include "cTransformData.h"

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
	m_pPhysicsNode = new cPhysicsNode;
	m_pPhysicsNode->Setup(NULL);
	m_pPhysicsNode->GetSpaceData().vGravity = D3DXVECTOR3(0, 0, 0);
	this->AddChild(m_pPhysicsNode);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	float fScaleX = static_cast<float>(SCREEN_WIDTH) / 1920.f;
	float fScaleY = static_cast<float>(SCREEN_HEIGHT) / 1080.f;

	m_pUILayer = new cInGameUILayer;
	m_pUILayer->Setup(NULL);
	m_pUILayer->SetScale(D3DXVECTOR2(fScaleX, fScaleY));
	this->AddChild(m_pUILayer);

	m_pPlayer_S = new cPlayer;
	m_pPlayer_S->Setup();

	m_pCamera = new cPlayerCamera;
	m_pCamera->Setup(&m_pPlayer_S->GetTransformData()->GetPosition(), m_pPlayer_S->GetRotationY());

	m_pPhysicsNode->AddChild(m_pPlayer_S);
}
void cInGameScene::Update(float fDelta)
{
	m_pCamera->Update();
	m_pPlayer_S->SetRotationY(m_pCamera->GetCamRotAngle().y);

	cGameScene::Update(fDelta);

	
}
void cInGameScene::Render()
{
	m_pGrid->Render();


	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	cGameScene::Render();
}
void cInGameScene::Delete()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pUILayer);
	SAFE_DELETE(m_pPlayer_S);

}
void cInGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCamera->WndProc(hWnd, message, wParam, lParam);
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}