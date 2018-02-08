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
class cSubject;
class cGameSprite;

class QuickSlot;
class GraphFindPath;

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
	cSubject*			m_pSubject;
	float				m_fRoundTime;
	bool				m_bRound1, m_bRound2, m_bRound3;

	QuickSlot * pQuickSlot_ {};
	GraphFindPath * pGraphFindPath_ {};

	cGameSprite* m_pMouse;
	cGameSprite* m_pAim;

public:
	cInGameScene();
	~cInGameScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	void CursorSetup();
	void CursorUpdate();
	void CursorRender();
	void CursorDelete();

	bool IsMakeTrap(OUT D3DXVECTOR3 &center, OUT DIRECTION_6 & direction, TrapType* tType, cRay ray);
	void MakeTrap(TrapType* tType, cRay ray);
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

