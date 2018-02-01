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
