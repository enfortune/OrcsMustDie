#include "stdafx.h"
#include "TrapTypeBarricade.h"

#include "TrapTypeComponent.h"

#include "cSkinnedMeshEX.h"

TrapTypeBarricade::TrapTypeBarricade()
{
	//pModel_ = ;
	modelList_.resize(1);
	modelList_[0].Setup("Resource/Trap/Barricade", "Barricade.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotation {};
	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixRotationX(&matrixRotation, D3DX_PI * 0.5f);

	matrixLocal_ = matrixScale * matrixRotation;

	frustumLocal_.vNear_00 = {-0.5f, 0.0f, -0.5f};
	frustumLocal_.vNear_01 = {-0.5f, 0.3f, -0.5f};
	frustumLocal_.vNear_10 = {-0.5f, 0.0f, +0.5f};
	frustumLocal_.vNear_11 = {-0.5f, 0.3f, +0.5f};
	frustumLocal_.vFar_00 = {0.5f, 0.0f, -0.5f};
	frustumLocal_.vFar_01 = {0.5f, 0.3f, -0.5f};
	frustumLocal_.vFar_10 = {0.5f, 0.0f, +0.5f};
	frustumLocal_.vFar_11 = {0.5f, 0.3f, +0.5f};

	isBlockable_ = true;

	width_ = 2;
	height_ = 2;

	frustumInteractionLocal_ = frustumLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;

	pTypeComponentBlockable_ = new TrapTypeComponentBlockable;
	pTypeComponentBlockable_->hpMax_ = 80;
}
