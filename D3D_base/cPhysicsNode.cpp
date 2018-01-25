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
	// ��ȣ�ۿ�
	std::set<cGameNode*>::iterator itSour, itDest;

	for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
	{
		(*itSour)->Update(fDelta);

		(*itSour)->GetPhysicsBody()->GetPhysicsData().vVelocity += m_stSpaceData.vGravity * fDelta;

		(*itSour)->UpdateTempPhysics(fDelta);
	}


	for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
	{
		itDest = itSour;
		itDest++;
		for (; itDest != m_setChild.end(); itDest++)
		{
			(*itSour)->CollisionWithNode(*itDest);
		}
		this->CollisionWithMap((*itSour));

		(*itSour)->UpdatePhysics(fDelta);	
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

void cPhysicsNode::CollisionWithMap(cGameNode* pNode)
{
	// TODO: �� �浹 �ڵ� �߰� �ʿ�
	bool a = false;
	if (m_pMap) a = m_pMap->MapCollisionCheck(pNode);
	bool b = a;
}
