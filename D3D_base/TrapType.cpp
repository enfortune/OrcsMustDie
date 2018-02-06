#include "stdafx.h"
#include "TrapType.h"

#include "cSkinnedMeshEX.h"

void TrapType::render(D3DXMATRIXA16 & matrixWorld, int modelIndex, TrapComponentRender & checkAuthority)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixWorld);
	modelList_[modelIndex].Render();
}

TrapType::~TrapType()
{
	delete pTypeComponentAttackable_;
	delete pTypeComponentBlockable_;
	delete pTypeComponentTriggerable_;
}

void TrapType::render(D3DXMATRIXA16 & matrixWorld)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixWorld);

	modelList_[0].Render();
	frustumInteractionLocal_.Render(g_pD3DDevice);
}
