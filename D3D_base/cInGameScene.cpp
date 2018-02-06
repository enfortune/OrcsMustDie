#include "stdafx.h"
#include "cInGameScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cInGameUILayer.h"
#include "cPlayerCamera.h"
#include "cPlayer.h"
#include "cPhysicsSpace.h"
#include "cPhysicsBody.h"
#include "cTransformData.h"
#include "cEnemy.h"
#include "cEnemy2.h"
#include "cBoss.h"
#include "cEnemyBase.h"
#include "cMapData.h"
#include "TrapTypeManager.h"
#include "cRay.h"
#include "TrapType.h"

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
	//this->AddChild(m_pMap);

	m_pPhysicsNode = new cPhysicsSpace;
	m_pPhysicsNode->Setup(m_pMap, &m_vTrap);
	//m_pPhysicsNode->GetSpaceData().vGravity = D3DXVECTOR3(0, 0, 0);
	this->AddChild(m_pPhysicsNode);

	m_pTrapTypeManager = new TrapTypeManager;
	m_pTrapTypeManager->init();

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

	for (int i = 0; i < 30; i++)
	{
		cEnemy* tempEnemy;
		tempEnemy = new cEnemy;
		tempEnemy->setPlayer(m_pPlayer_S);

		tempEnemy->Setup(true, D3DXVECTOR3(0, 0, 0));

		m_vEnemyBase.push_back(tempEnemy);
	}
	for (int i = 0; i < 10; i++)
	{
		cEnemy2* tempEnemy;
		tempEnemy = new cEnemy2;
		tempEnemy->setPlayer(m_pPlayer_S);

		tempEnemy->Setup(true, D3DXVECTOR3(0, 0, 0));
		m_vEnemyBase.push_back(tempEnemy);
	}

	for (int i = 0; i < 10; i++)
	{
		if (i < 5)
		{
			m_vEnemyBase[i]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(27 + i, 2, 53);
		}
		else
		{
			m_vEnemyBase[i]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(22 + i, 2, 54);
		}

		m_pPhysicsNode->AddChild(m_vEnemyBase[i]);

	}

	cBoss* tempEnemy;
	tempEnemy = new cBoss;
	tempEnemy->setPlayer(m_pPlayer_S);

	tempEnemy->Setup(true, D3DXVECTOR3(0, 0, 0));

	m_vEnemyBase.push_back(tempEnemy);

	m_pPlayer_S->setEnemy(&m_vEnemyBase);

	m_fRoundTime = 0;
	m_bRound1 = false;
	m_bRound2 = false;
	m_bRound3 = false;

	m_nEnemyDeadCount = 0;

	g_pSoundManager->AddSound("MainBGM", "Resource/Sound/Map/WrathGate.mp3", true, false);
	g_pSoundManager->Play("MainBGM",0.3f);

}
void cInGameScene::Update(float fDelta)
{
	m_pCamera->Update();
	m_pMap->Update(fDelta);
	m_pPlayer_S->SetRotationY(m_pCamera->GetCamRotAngle().y);

	m_fRoundTime += fDelta;

	if (m_fRoundTime >= 10 && m_bRound1 == false)
	{
		for (int i = 0; i < 15; i++)
		{
			if (i < 5)
			{
				m_vEnemyBase[10 + i - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(27 + i, 2, 52);
			}
			else if (i < 10)
			{
				m_vEnemyBase[10 + i - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(22 + i, 2, 53);
			}
			else
			{
				m_vEnemyBase[10 + i - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(17 + i, 2, 54);
			}
			m_pPhysicsNode->AddChild(m_vEnemyBase[10 + i - m_nEnemyDeadCount]);
		}

		m_bRound1 = true;
	}
	if (m_fRoundTime >= 20 && m_bRound2 == false)
	{
		for (int i = 0; i < 5; i++)
		{
			m_vEnemyBase[25 + i - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(27 + i, 2, 53);

			m_pPhysicsNode->AddChild(m_vEnemyBase[25 + i - m_nEnemyDeadCount]);
		}
		for (int i = 0; i < 10; i++)
		{
			if (i < 5)
			{
				m_vEnemyBase[30 + i - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(27 + i, 2, 54);
			}
			else
			{
				m_vEnemyBase[30 + i - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(22 + i, 2, 55);
			}

			m_pPhysicsNode->AddChild(m_vEnemyBase[30 + i - m_nEnemyDeadCount]);
		}

		m_bRound2 = true;
	}
	if (m_fRoundTime >= 30 && m_bRound3 == false)
	{
		m_vEnemyBase[40 - m_nEnemyDeadCount]->GetPhysicsBody()->GetPhysicsData().vPos = D3DXVECTOR3(32, 2, 55);
		m_pPhysicsNode->AddChild(m_vEnemyBase[40 - m_nEnemyDeadCount]);
		m_bRound3 = true;
	}

	for (std::vector<cEnemyBase*>::iterator i = m_vEnemyBase.begin(); i != m_vEnemyBase.end();)
	{

		if ((*i)->fDeadCount > 4.f)
		{
			(*i)->RemoveFromParent();
			SAFE_RELEASE((*i));
			i = m_vEnemyBase.erase(i);
			m_nEnemyDeadCount++;
		}
		else
		{
			i++;
		}
	}
	cRay rayRay = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_pPlayer_S->SetIsBattle(true);
	}
	if (g_pKeyManager->IsOnceKeyDown('2'))
	{
		m_pPlayer_S->SetIsBattle(false);
		m_pPlayer_S->SetPlayerTrapType(PLAYERTRAPTYPE_BARRICADE);
	}

	if (m_pPlayer_S->GetIsBattle() == false)
	{
		if (m_pPlayer_S->GetPlayerTrapType() == PLAYERTRAPTYPE_BARRICADE)
		{
			TrapType * pTrapType = m_pTrapTypeManager->find("Barricade");
			cRay ray = cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y);

			DIRECTION_6 direction = DIRECTION_6::TOP;

			D3DXVECTOR3 vertexCenter {};
			D3DXMATRIXA16 matrixRender {};

			m_pMap->GetBuildPostion(vertexCenter, direction, ray, 100, pTrapType->getWidth(), pTrapType->getHeight());
			D3DXMatrixTranslation(&matrixRender, vertexCenter.x, vertexCenter.y, vertexCenter.z);

			pTrapType->render(matrixRender);

			if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
			{
				MakeTrap(pTrapType, cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y));
			}
		}
	}

	//if (g_pKeyManager->IsOnceKeyDown('2'))
	//{
	//	m_pPlayer_S->SetIsBattle(false);
	//}

	cGameScene::Update(fDelta);

	
}
void cInGameScene::Render()
{
	m_pGrid->Render();
	m_pMap->Render();
	for (int i = 0; i < m_vTrap.size(); i++)
	{
		m_vTrap[i].render();
	}

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
	SAFE_DELETE(m_pTrapTypeManager);

	for (int i = 0; i < m_vEnemyBase.size(); i++)
	{
		SAFE_RELEASE(m_vEnemyBase[i]);
	}
	m_vEnemyBase.clear();
}

bool cInGameScene::IsMakeTrap(OUT D3DXVECTOR3 &center,TrapType* tType, cRay ray)
{
	D3DXVECTOR3 vCenterPos = { 0,0,0 };
	if (tType->isConstructible(TrapType::eInstallPosition::FLOOR))
	{
		DIRECTION_6 Dir_6;
		Dir_6 = DIRECTION_6::TOP;
		if (m_pMap->IsEnableToBuild(ray, 100, tType->getWidth(), tType->getHeight()))
		{
			m_pMap->GetBuildPostion(vCenterPos, Dir_6, ray, 100, tType->getWidth(), tType->getHeight());

			center = vCenterPos;
			return true;
		}
	}
	if (tType->isConstructible(TrapType::eInstallPosition::CEILING))
	{
		DIRECTION_6 Dir_6;
		Dir_6 = DIRECTION_6::BOTTOM;
		if (m_pMap->IsEnableToBuild(ray, 100, tType->getWidth(), tType->getHeight()))
		{
			m_pMap->GetBuildPostion(vCenterPos, Dir_6, ray, 100, tType->getWidth(), tType->getHeight());

			center = vCenterPos;
			return true;
		}
	}
	if (tType->isConstructible(TrapType::eInstallPosition::WALL))
	{
		DIRECTION_6 Dir_6;
		Dir_6 = DIRECTION_6::LEFT;
		if (m_pMap->IsEnableToBuild(ray, 100, tType->getWidth(), tType->getHeight()))
		{
			m_pMap->GetBuildPostion(vCenterPos, Dir_6, ray, 100, tType->getWidth(), tType->getHeight());

			center = vCenterPos;
			return true;
		}
		Dir_6 = DIRECTION_6::RIGHT;
		if (m_pMap->IsEnableToBuild(ray, 100, tType->getWidth(), tType->getHeight()))
		{
			m_pMap->GetBuildPostion(vCenterPos, Dir_6, ray, 100, tType->getWidth(), tType->getHeight());

			center = vCenterPos;
			return true;
		}
		Dir_6 = DIRECTION_6::FRONT;
		if (m_pMap->IsEnableToBuild(ray, 100, tType->getWidth(), tType->getHeight()))
		{
			m_pMap->GetBuildPostion(vCenterPos, Dir_6, ray, 100, tType->getWidth(), tType->getHeight());

			center = vCenterPos;
			return true;
		}
		Dir_6 = DIRECTION_6::REAR;
		if (m_pMap->IsEnableToBuild(ray, 100, tType->getWidth(), tType->getHeight()))
		{
			m_pMap->GetBuildPostion(vCenterPos, Dir_6, ray, 100, tType->getWidth(), tType->getHeight());

			center = vCenterPos;
			return true;
		}
	}

	return false;
}

void cInGameScene::MakeTrap(TrapType* tType, cRay ray)
{
	D3DXVECTOR3 vCenter = { 0,0,0 };
	if (IsMakeTrap(vCenter,tType, ray))
	{
		D3DXMATRIXA16 matT;
		D3DXMatrixTranslation(&matT, vCenter.x, vCenter.y, vCenter.z);

		m_vTrap.emplace_back();
		m_vTrap[m_vTrap.size() - 1].init(*tType, matT);

		m_pMap->BuildTrap(&m_vTrap[m_vTrap.size() - 1],ray,10, tType->getWidth(), tType->getHeight());
	}

}
void cInGameScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCamera->WndProc(hWnd, message, wParam, lParam);
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}