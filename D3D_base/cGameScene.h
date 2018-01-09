#pragma once
#include "cGameNode.h"
class cGameScene :
	public cGameNode
{
	SYNTHESIZE_REF(bool, m_bUpdateEnable, UpdateEnable);
	//SYNTHESIZE_REF(bool, m_bRenderEnable, RenderEnable);

public:
	cGameScene();
	virtual ~cGameScene();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void Render();

	//void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

