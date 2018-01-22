#include "stdafx.h"
#include "cPhysicsNode.h"
#include "cPhysicsBody.h"
#include "iGameMap.h"


cPhysicsNode::cPhysicsNode()
{
	long long currentTick {};

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *> (&currentTick));
}


cPhysicsNode::~cPhysicsNode()
{
}


void cPhysicsNode::Setup(iGameMap* pMap)
{
	cGameNode::Setup(true);

	m_pMap = pMap;
}
void cPhysicsNode::Update(float fDelta)
{
	// 상호작용
	std::set<cGameNode*>::iterator itSour, itDest;

	for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
	{
		(*itSour)->UpdateTempPhysics();
		for (itDest = ++itSour; itDest != m_setChild.end(); itDest++)
		{
			
		}
	}

	
	/*for each (cGameNode* node in m_setChild)
	{
		node->UpdatePhysics(fDelta);
		node->Update(fDelta);
	}*/
}
void cPhysicsNode::Render()
{


	cGameNode::Render();
}

void cPhysicsNode::Delete()
{

}

void cPhysicsNode::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}


void cPhysicsNode::UpdatePhysics(float fDelta)
{
	cGameNode::UpdatePhysics(fDelta);

}

