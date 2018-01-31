#include "stdafx.h"
#include "TrapType.h"

#include "cSkinnedMesh.h"

void TrapType::render(D3DXMATRIXA16 & matrixWorld)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixWorld);
	//pModel_->Render(matrixWorld);

	frustumInteractionLocal_.Render(g_pD3DDevice);
}
