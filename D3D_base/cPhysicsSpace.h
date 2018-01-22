#pragma once
#include "cPhysicsNode.h"
class iGameMap;

class cPhysicsSpace : public cPhysicsNode
{
protected:
	

public:
	cPhysicsSpace();
	virtual ~cPhysicsSpace();

	virtual void Setup(iGameMap* pMapData = nullptr) override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

