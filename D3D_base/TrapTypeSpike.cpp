#include "stdafx.h"
#include "TrapTypeSpike.h"

#include "TrapTypeComponent.h"

TrapTypeSpike::TrapTypeSpike()
{
	modelList_.resize(1);
	modelList_[0].Setup("Resource/Trap/Barricade", "Resource/Trap/Barricade/Barricade.X");
	//modelList_[1].Setup("Resource/Trap/Barricade", "Resource/Trap/Barricade/Barricade.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotation {};
	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixRotationX(&matrixRotation, D3DX_PI * 0.5f);

	matrixLocal_ = matrixScale * matrixRotation;

	frustumLocal_.vNear_00 = {-1.0f, 0.0f, -1.0f};
	frustumLocal_.vNear_01 = {-1.0f, 0.6f, -1.0f};
	frustumLocal_.vNear_10 = {-1.0f, 0.0f, +1.0f};
	frustumLocal_.vNear_11 = {-1.0f, 0.6f, +1.0f};
	frustumLocal_.vFar_00 = {1.0f, 0.0f, -1.0f};
	frustumLocal_.vFar_01 = {1.0f, 0.6f, -1.0f};
	frustumLocal_.vFar_10 = {1.0f, 0.0f, +1.0f};
	frustumLocal_.vFar_11 = {1.0f, 0.6f, +1.0f};

	D3DXMATRIXA16 matrixInverse {};
	D3DXMatrixInverse(&matrixInverse, nullptr, &matrixLocal_);

	frustumLocal_ = frustumLocal_.TransformCoord(&matrixInverse);
	isBlockable_ = false;

	width_ = 2;
	height_ = 2;

	frustumInteractionLocal_ = frustumLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;
	isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::ENEMY)] = true;

	timerType_ = TrapType::eTimerType::COOLDOWN;

	pTypeComponentAttackable_ = new TrapTypeComponentAttackable;

	pTypeComponentAttackable_->cooldownMax_ = 3.0f;
	pTypeComponentAttackable_->damage_ = 3;
	pTypeComponentAttackable_->duration_ = 1.5f;
	pTypeComponentAttackable_->frustumAttackLocal_ = frustumInteractionLocal_;
}

void TrapComponentRenderSpike::render(TrapType & trapType, D3DXMATRIXA16 & matrixWorld)
{
	trapType.render(matrixWorld, 1, *this);
}
