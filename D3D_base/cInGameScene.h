#pragma once
#include "cGameScene.h"
#include "Trap.h"

class cInGameUILayer;
class cSkinnedMesh;
class cCamera;
class cGrid;
class cPlayer;
class cPlayerCamera;
class cPhysicsNode;
class cEnemy;
class cMapData;
class TrapTypeManager;
class cRay;

class cInGameScene : public cGameScene
{
	cPlayerCamera* m_pCamera;
	cGrid* m_pGrid;

	cInGameUILayer* m_pUILayer;
	std::vector<cEnemy*> m_vEnemy;

	/* for test */
	cPlayer*			m_pPlayer_S;
	cPhysicsNode*		m_pPhysicsNode;
	std::vector<Trap>	m_vTrap;
	TrapTypeManager*	m_pTrapTypeManager;
	cMapData*			m_pMap;
	
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

