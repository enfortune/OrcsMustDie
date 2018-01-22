#include "stdafx.h"
#include "cPhysicsSpace.h"


cPhysicsSpace::cPhysicsSpace()
{
}
cPhysicsSpace::~cPhysicsSpace()
{
	this->Delete();
}
void cPhysicsSpace::Setup(iGameMap* pMapData)
{
	cPhysicsNode::Setup(pMapData);

}
void cPhysicsSpace::Update(float fDelta)
{

	/* 충돌처리 들어가야 함*/

	cPhysicsNode::Update(fDelta);
}
void cPhysicsSpace::Render()
{


	cPhysicsNode::Render();
}
void cPhysicsSpace::Delete()
{

}
void cPhysicsSpace::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}