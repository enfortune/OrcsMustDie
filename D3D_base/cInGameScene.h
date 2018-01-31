#pragma once
#include "cGameScene.h"

class cInGameUILayer;
class cSkinnedMesh;
class cCamera;
class cGrid;
class cPlayer;
class cPlayerCamera;
class cPhysicsNode;

class cInGameScene : public cGameScene
{
	cPlayerCamera* m_pCamera;
	cGrid* m_pGrid;

	cInGameUILayer* m_pUILayer;

	/* for test */
	cPlayer* m_pPlayer_S;

	cPhysicsNode* m_pPhysicsNode;
	
public:
	cInGameScene();
	~cInGameScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

