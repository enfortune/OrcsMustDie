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
#include "cEnemy.h"
#include "cMapData.h"

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

	m_pMap = new cMapData;
	m_pMap->Setup();
	m_pMap->LoadData("SampleStageMap.map");
	this->AddChild(m_pMap);

	m_pPhysicsNode = new cPhysicsNode;
	m_pPhysicsNode->Setup(m_pMap);
	//m_pPhysicsNode->GetSpaceData().vGravity = D3DXVECTOR3(0, 0, 0);
	this->AddChild(m_pPhysicsNode);

	m_pPlayer_S = new cPlayer;
	m_pPlayer_S->Setup();
	m_pPlayer_S->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(47, 0, 29);

	m_pCamera = new cPlayerCamera;
	m_pCamera->Setup(&m_pPlayer_S->GetTransformData()->GetPosition(), m_pPlayer_S->GetRotationY());
	m_pPhysicsNode->AddChild(m_pPlayer_S);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pUILayer = new cInGameUILayer;
	m_pUILayer->Setup(m_pPlayer_S);

	float fScaleX = static_cast<float>(SCREEN_WIDTH) / 1920.f;
	float fScaleY = static_cast<float>(SCREEN_HEIGHT) / 1080.f;

	m_pUILayer->SetScale(D3DXVECTOR2(fScaleX, fScaleY));
	this->AddChild(m_pUILayer);

	for (int i = 0; i < 10; i++)
	{
		cEnemy* tempEnemy;
		tempEnemy = new cEnemy;
		if (i < 5)
		{
			tempEnemy->Setup(true, D3DXVECTOR3(27 + i, 2, 53));
		}
		else
		{
			tempEnemy->Setup(true, D3DXVECTOR3(22 + i, 2, 54));
		}
		tempEnemy->setPlayer(m_pPlayer_S);
		m_pPhysicsNode->AddChild(tempEnemy);

		m_vEnemy.push_back(tempEnemy);
	}

	m_pPlayer_S->setEnemy(&m_vEnemy);
}
void cInGameScene::Update(float fDelta)
{
	m_pCamera->Update();
	m_pPlayer_S->SetRotationY(m_pCamera->GetCamRotAngle().y);

	for (std::vector<cEnemy*>::iterator i = m_vEnemy.begin(); i != m_vEnemy.end();)
	{

		if ((*i)->fDeadCount > 4.f)
		{
			(*i)->RemoveFromParent();
			SAFE_RELEASE((*i));
			i = m_vEnemy.erase(i);
		}
		else
		{
			i++;
		}
	}

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
	SAFE_RELEASE(m_pPlayer_S);
	SAFE_RELEASE(m_pMap);

	for (int i = 0; i < m_vEnemy.size(); i++)
	{
		SAFE_RELEASE(m_vEnemy[i]);
	}
	m_vEnemy.clear();
}
void cInGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCamera->WndProc(hWnd, message, wParam, lParam);
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}