#pragma once
#include "cGameUILayer.h"
#include "MapDataEnumList.h"

class cMapMaker;
class cGameUIButton;

class cMapToolUILayer : public cGameUILayer
{
	cMapMaker* m_pMaker;

	cGameUIButton* m_pBlockChangeButton[GRIDBOXKIND_END];

public:
	cMapToolUILayer();
	virtual ~cMapToolUILayer();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void LinkMapMaker(cMapMaker* lpMaker) { m_pMaker = lpMaker; }

	/* delegate */
	virtual bool UIButtonAction(std::string sTag, BUTTON_STATE enState);
};

