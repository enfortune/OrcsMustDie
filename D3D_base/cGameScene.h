#pragma once
#include "cGameNode.h"
class cGameScene : public cGameNode
{
	SYNTHESIZE_REF(bool, m_bUpdateEnable, UpdateEnable); // �� �Ŵ������� ����ϴ� ������.

public:
	cGameScene();
	virtual ~cGameScene();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Destroy() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	
	RECT GetScreenRect();
};

