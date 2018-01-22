#include "stdafx.h"
#include "cMapToolScene.h"

#include "cMapToolUILayer.h"
#include "cMapData.h"
#include "cMapMaker.h"
#include "cMapToolCamera.h"
#include "cGrid.h"

cMapToolScene::cMapToolScene()
	: m_pGrid(nullptr)
	, m_pMapToolCamera(nullptr)
	, m_pUILayer(nullptr)
	, m_pMapData(nullptr)
	, m_pMapMaker(nullptr)
{
}

cMapToolScene::~cMapToolScene()
{
	this->Delete();
}
void cMapToolScene::Setup()
{
	cGameScene::Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pMapToolCamera = new cMapToolCamera;
	m_pMapToolCamera->Setup(nullptr);

	m_pUILayer = new cMapToolUILayer;
	m_pUILayer->Setup();
	this->AddChild(m_pUILayer);

	m_pMapData = new cMapData;
	m_pMapData->Setup();
	this->AddChild(m_pMapData);

	m_pMapMaker = new cMapMaker;
	m_pMapMaker->Setup(m_pMapData);
	this->AddChild(m_pMapMaker);
}
void cMapToolScene::Update(float fDelta)
{
	m_pMapToolCamera->Update();

	cGameScene::Update(fDelta);
}
void cMapToolScene::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);

	m_pGrid->Render();

	cGameScene::Render();

}
void cMapToolScene::Delete()
{
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pUILayer);
	SAFE_RELEASE(m_pMapData);
	SAFE_RELEASE(m_pMapMaker);
}
void cMapToolScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pMapToolCamera->WndProc(hWnd, message, wParam, lParam);

	cGameScene::WndProc(hWnd, message, wParam, lParam);
}