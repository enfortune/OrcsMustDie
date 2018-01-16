#pragma once
#include "cGameNode.h"
#include "iGameUIDelegate.h"
class cGameUILayer : public cGameNode, public iGameUIDelegate
{
	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle); // Z angle
	SYNTHESIZE_REF(D3DXVECTOR2, m_vScale, Scale);

public:
	cGameUILayer();
	virtual ~cGameUILayer();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	virtual D3DXMATRIXA16 GetMatirixToParent() override;
};

