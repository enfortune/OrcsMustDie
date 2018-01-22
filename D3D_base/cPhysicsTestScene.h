#pragma once
#include "cGameScene.h"
class cMapData;
class cPhysicsTestScene : public cGameScene
{
	
	cMapData* m_pMapData;

public:
	cPhysicsTestScene();
	~cPhysicsTestScene();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

