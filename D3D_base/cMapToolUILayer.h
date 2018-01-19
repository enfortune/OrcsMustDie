#pragma once
#include "cGameUILayer.h"
class cMapToolUILayer : public cGameUILayer
{
public:
	cMapToolUILayer();
	virtual ~cMapToolUILayer();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	/* delegate */
	virtual bool UIButtonAction(std::string sTag, BUTTON_STATE enState);
};

