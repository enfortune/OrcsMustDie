#include "stdafx.h"
#include "TrapType.h"

#include "cSkinnedMeshEX.h"

TrapType::~TrapType()
{
	delete pTypeComponentAttackable_;
	delete pTypeComponentBlockable_;
	delete pTypeComponentTriggerable_;
}

void TrapType::render(D3DXMATRIXA16 & matrixWorld)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixWorld);

	for (int i = 0; i < static_cast<int> (modelList_.size()); ++i)
		modelList_[i].Render();

	D3DXMATRIXA16 matrixIdentity {};
	D3DXMatrixIdentity(&matrixIdentity);

	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixIdentity);
	frustumInteractionLocal_.Render(g_pD3DDevice);
}
