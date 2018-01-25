#pragma once
#include "cGameNode.h"
class iGameMap;

typedef struct ST_PHYSICSSPACEDATA
{
	D3DXVECTOR3 vGravity;

	ST_PHYSICSSPACEDATA()
	{
		vGravity = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}
}PHYSICSSPACEDATA;

class cPhysicsNode : public cGameNode
{
	iGameMap* m_pMap;

	ST_PHYSICSSPACEDATA m_stSpaceData;

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

