#pragma once
#include "cGameUILayer.h"

class cInGameUIGauge;
class cInGameUIScore;

class cPlayer;

class cInGameUILayer : public cGameUILayer
{
	void* m_pTarget;

	cInGameUIGauge* m_pUIGauge;
	cInGameUIScore* m_pUIScore;

public:
	cInGameUILayer();
	virtual ~cInGameUILayer();

	virtual void Setup(cPlayer * pPlayer = nullptr);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	virtual bool UIButtonAction(std::string sTag, BUTTON_STATE enState) override;
};

