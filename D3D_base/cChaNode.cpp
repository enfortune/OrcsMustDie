#include "stdafx.h"
#include "cChaNode.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"


cChaNode::cChaNode()
{
}


cChaNode::~cChaNode()
{
	Delete();
}


void cChaNode::Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup)
{
	cGameNode::Setup(true);

}

void cChaNode::Update(float fDelta)
{


	cGameNode::Update(fDelta);
}

void cChaNode::Render()
{


	cGameNode::Render();
}

void cChaNode::Delete()
{
}

void cChaNode::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cChaNode::Idle()
{
}

void cChaNode::Move(D3DXVECTOR3 vGoal, float fDelta)
{
}

void cChaNode::Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum)
{
}

void cChaNode::Attack(float fDelta)
{
}

void cChaNode::Dead()
{
}

void cChaNode::Dijkstra(int tpnum, float fDelta)
{
}

void cChaNode::find(int tpNum, float fDelta)
{
}

void cChaNode::getDamage(int nDamage)
{
}

void cChaNode::HpManager()
{
}
