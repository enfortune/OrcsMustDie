#include "stdafx.h"
#include "TrapType.h"

#include "TrapTypeComponent.h"

#include "cSkinnedMeshEX.h"

void TrapType::render(D3DXMATRIXA16 & matrixWorld, int modelIndex)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixWorld);
	modelList_[modelIndex].Render();
	
	FRUSTUM temp = frustumLocal_.TransformCoord(&matrixWorld);
	temp.Render(g_pD3DDevice);
}

TrapType::~TrapType()
{
	delete pTypeComponentAttackable_;
	delete pTypeComponentBlockable_;
	delete pTypeComponentTriggerable_;
}

void TrapType::render(D3DXMATRIXA16 & matrixWorld)
{
	D3DXMATRIXA16 matrixTotal = matrixLocal_ * matrixWorld;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixTotal);

	modelList_[0].Render();
	frustumInteractionLocal_.Render(g_pD3DDevice);
}
