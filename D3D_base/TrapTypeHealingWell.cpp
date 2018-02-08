#include "stdafx.h"
#include "TrapTypeHealingWell.h"

TrapTypeHealingWell::TrapTypeHealingWell()
{
	modelList_.resize(2);
	modelList_[0].Setup("Resource/Trap/HealingWell", "Resource/Trap/HealingWell/HealingWell.X");
	modelList_[1].Setup("Resource/Trap/HealingWell", "Resource/Trap/HealingWell/HealthPotionDrop.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotation {};
	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixRotationX(&matrixRotation, D3DX_PI * 0.5f);

	matrixLocalList_.resize(1);
	matrixLocalList_[0] = matrixScale * matrixRotation;

	frustumLocal_.vNear_00 = {-1.0f, 0.0f, -1.0f};
	frustumLocal_.vNear_01 = {-1.0f, 2.0f, -1.0f};
	frustumLocal_.vNear_10 = {-1.0f, 0.0f, +1.0f};
	frustumLocal_.vNear_11 = {-1.0f, 2.0f, +1.0f};
	frustumLocal_.vFar_00 = {1.0f, 0.0f, -1.0f};
	frustumLocal_.vFar_01 = {1.0f, 2.0f, -1.0f};
	frustumLocal_.vFar_10 = {1.0f, 0.0f, +1.0f};
	frustumLocal_.vFar_11 = {1.0f, 2.0f, +1.0f};

	frustumInteractionLocal_ = frustumLocal_;

	width_ = 1;
	height_ = 2;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::WALL)] = true;

	pTypeComponentTriggerable_ = new TrapTypeComponentTriggerableHealingWell;

	moneyCost_ = 800;
}

std::unique_ptr<TrapComponentTriggerable> TrapTypeComponentTriggerableHealingWell::newComponentObject() const
{ return std::make_unique<TrapComponentTriggerableHealingWell>(); }

void TrapComponentTriggerableHealingWell::interaction(Trap & trap, cPlayer & player)
{
}

void TrapComponentTriggerableHealingWell::update(Trap & trap, float fDelta)
{
	timerCreateHealingPotion_ -= fDelta;

	if (timerCreateHealingPotion_ < 0.0f)
		timerCreateHealingPotion_ = 0.0f;
}
