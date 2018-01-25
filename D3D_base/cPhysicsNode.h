#pragma once
#include "cGameNode.h"
class iGameMap;

class cPhysicsNode : public cGameNode
{
	iGameMap* m_pMap;
	
public:
	cPhysicsNode();
	virtual ~cPhysicsNode();

	virtual void Setup(iGameMap* pMap);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void CollisionWithMap(cGameNode* pNode);
};

