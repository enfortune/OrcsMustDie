#pragma once
#include "cGameNode.h"

class cPhysicsNode : public cGameNode
{
public:
	cPhysicsNode();
	virtual ~cPhysicsNode();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void Render();

	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
};

