#pragma once
#include "cGameScene.h"

class cMapToolUILayer;
class cMapData;
class cMapMaker;
class cMapToolCamera;

class cMapToolScene : public cGameScene
{
	cMapToolCamera* m_pMapToolCamera;


	cMapToolUILayer* m_pUILayer;
	cMapData*		m_pMapData;
	cMapMaker*		m_pMapMaker;
	
public:
	cMapToolScene();
	virtual ~cMapToolScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

