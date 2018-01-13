#pragma once
#include "cGameScene.h"

class cGameSprite;

class cTestScene :
	public cGameScene
{
private:
	cGameSprite* m_spriteGirl;

	
public:
	cTestScene();
	~cTestScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Destroy() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

