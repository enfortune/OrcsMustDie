#include "stdafx.h"
#include "TrapType.h"

#include "TrapTypeComponent.h"

#include "cSkinnedMeshEX.h"

void TrapType::render(D3DXMATRIXA16 & matrixWorld, int modelIndex)
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
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

float TrapType::getFrustumMaxLength() const
{
	D3DXVECTOR3 vectorLength = frustumLocal_.vFar_00 - frustumLocal_.vNear_11;
	
	if (vectorLength.x < 0) vectorLength.x = -vectorLength.x;
	if (vectorLength.y < 0) vectorLength.y = -vectorLength.y;
	if (vectorLength.z < 0) vectorLength.z = -vectorLength.z;

	return sqrtf(vectorLength.x * vectorLength.x + vectorLength.y * vectorLength.y
		+ vectorLength.z * vectorLength.z);
}

void TrapType::render(D3DXMATRIXA16 & matrixWorld)
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIXA16 matrixTotal = matrixLocalList_[0] * matrixWorld;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matrixTotal);

	modelList_[0].Render();
	
	FRUSTUM frustum = frustumInteractionLocal_.TransformCoord(&matrixWorld);
	frustum.Render(g_pD3DDevice);
}
