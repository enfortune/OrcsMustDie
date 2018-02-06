#pragma once
#include "cPhysicsNode.h"
class iGameMap;
class Trap;

class cPhysicsSpace : public cPhysicsNode
{
protected:
	std::vector<Trap>* m_pVecTrap;

public:
	cPhysicsSpace();
	virtual ~cPhysicsSpace();

	virtual void Setup(iGameMap* pMapData = nullptr, std::vector<Trap>* pVecTrap = nullptr);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
	void cPhysicsSpace::CollisionWithTrap(cGameNode* pNode);
};

