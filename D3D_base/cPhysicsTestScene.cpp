#include "stdafx.h"
#include "cPhysicsTestScene.h"
#include "cMapData.h"
#include "cPhysicsNode.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cSampleChar.h"

cPhysicsTestScene::cPhysicsTestScene()
	: m_pMapData(nullptr)
	, m_pSpace(nullptr)
	, m_pCamera(nullptr)
	, m_pGrid(nullptr)
	, m_pEnemy(nullptr)
	, m_pEnemy2(nullptr)
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
	m_pMapData->LoadData("SampleMap.map");
	this->AddChild(m_pMapData);

	m_pSpace = new cPhysicsNode;
	m_pSpace->Setup(m_pMapData);
	this->AddChild(m_pSpace);

	m_pCamera = new cCamera;
	m_pCamera->Setup(nullptr);
	m_pGrid = new cGrid;
	m_pGrid->Setup();


	m_pEnemy = new cSampleChar;
	m_pEnemy->Setup(true, D3DXVECTOR3(2.f, 3.f, 2.f));
	m_pSpace->AddChild(m_pEnemy);

	m_pEnemy2 = new cSampleChar;
	m_pEnemy2->Setup(false, D3DXVECTOR3(5.f, 5.f, 5.f));
	m_pSpace->AddChild(m_pEnemy2);

}
void cPhysicsTestScene::Update(float fDelta)
{
	
	m_pCamera->Update();

	cGameScene::Update(fDelta);
}
void cPhysicsTestScene::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pGrid->Render();

	cGameScene::Render();
}
void cPhysicsTestScene::Delete()
{
	SAFE_RELEASE(m_pMapData);
	SAFE_RELEASE(m_pSpace);
	SAFE_RELEASE(m_pEnemy);
	SAFE_RELEASE(m_pEnemy2);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
}

void cPhysicsTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCamera->WndProc(hWnd, message, wParam, lParam);

	cGameScene::WndProc(hWnd, message, wParam, lParam);
}