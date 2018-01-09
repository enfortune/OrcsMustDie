#include "stdafx.h"
#include "cPhysicsNode.h"
#include "cPhysicsBody.h"


cPhysicsNode::cPhysicsNode()
{
}


cPhysicsNode::~cPhysicsNode()
{
}


void cPhysicsNode::Setup()
{
	cGameNode::Setup();
}
void cPhysicsNode::Update(float fDelta)
{
	for each (cGameNode* node in m_setChild)
	{
		node->UpdatePhysics();
		node->Update();
	}
}
void cPhysicsNode::Render()
{


	cGameNode::Render();
}

void cPhysicsNode::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}


void cPhysicsNode::UpdatePhysics(float fDelta)
{
	cGameNode::UpdatePhysics(fDelta);

}

