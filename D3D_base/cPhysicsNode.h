#pragma once
#include "cGameNode.h"
class iGameMap;

typedef struct ST_PHYSICSSPACEDATA
{
	D3DXVECTOR3 vGravity;

	ST_PHYSICSSPACEDATA()
	{
		vGravity = D3DXVECTOR3(0.0f, -9.8f, 0.0f);
	}
}PHYSICSSPACEDATA;

class cPhysicsNode : public cGameNode
{
	iGameMap* m_pMap;

	SYNTHESIZE_REF(ST_PHYSICSSPACEDATA, m_stSpaceData, SpaceData);

public:
	cPhysicsNode();
	virtual ~cPhysicsNode();

	virtual void Setup(iGameMap* pMap);
	virtual void Update(float fDelta) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void UpdatePhysics(float fDelta);
	virtual void CollisionWithMap(cGameNode* pNode, float fDelta);
};

