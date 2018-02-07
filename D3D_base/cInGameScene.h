#pragma once
#include "cGameScene.h"
#include "Trap.h"

class cInGameUILayer;
class cSkinnedMesh;
class cCamera;
class cPlayer;
class cPlayerCamera;
class cPhysicsSpace;
class cEnemy;
class cEnemyBase;
class cMapData;
class TrapTypeManager;
class cRay;
class cSky;

#include "GraphFindPath.h"
class QuickSlot;

class cInGameScene : public cGameScene
{
	cPlayerCamera* m_pCamera;

	cInGameUILayer* m_pUILayer;
	std::vector<cEnemyBase*>	m_vEnemyBase;

	int						m_nEnemyDeadCount;

	/* for test */
	cPlayer*			m_pPlayer_S;
	cPhysicsSpace*		m_pPhysicsNode;
	std::vector<Trap>	m_vTrap;
	TrapTypeManager*	m_pTrapTypeManager;
	cMapData*			m_pMap;
	cSky*				m_pSkyMap;
	float				m_fRoundTime;
	bool				m_bRound1, m_bRound2, m_bRound3;

	GraphFindPath pGraphFindPath_;
	QuickSlot * pQuickSlot_ {};
	
public:
	cInGameScene();
	~cInGameScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	bool IsMakeTrap(OUT D3DXVECTOR3 &center, TrapType* tType, cRay ray);
	void MakeTrap(TrapType* tType, cRay ray);
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

