#pragma once
#include "cGameNode.h"

class cPhysicsNode : public cGameNode
{

public:
	cPhysicsNode();
	virtual ~cPhysicsNode();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
};

