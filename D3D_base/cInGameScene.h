#pragma once
#include "cGameScene.h"

class cInGameUILayer;

class cInGameScene : public cGameScene
{
	cInGameUILayer* m_pUILayer;
	
public:
	cInGameScene();
	~cInGameScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

