#include "stdafx.h"
#include "TrapTypeHealingWell.h"

#include "Trap.h"

#include "cPlayer.h"

TrapTypeHealingWell::TrapTypeHealingWell()
{
	modelList_.resize(2);
	modelList_[0].Setup("Resource/Trap/HealingWell", "Resource/Trap/HealingWell/HealingWell.X");
	modelList_[1].Setup("Resource/Trap/HealingWell", "Resource/Trap/HealingWell/HealthPotionDrop.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotationX {}, matrixRotationY {}, matrixTranslation {};
	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixRotationX(&matrixRotationX, D3DX_PI * 0.5f);
	D3DXMatrixRotationY(&matrixRotationY, D3DX_PI * 0.5f);
	D3DXMatrixTranslation(&matrixTranslation, 0.0f, -1.0f, 0.0f);

	matrixLocalList_.resize(2);
	matrixLocalList_[0] = matrixScale * matrixRotationX * matrixRotationY /** matrixTranslation*/;
	matrixLocalList_[1] = matrixScale * matrixRotationX * matrixRotationY /** matrixTranslation*/;

	frustumLocal_.vNear_00 = {-1.0f, -1.0f, -1.0f};
	frustumLocal_.vNear_01 = {-1.0f, 1.0f, -1.0f};
	frustumLocal_.vNear_10 = {-1.0f, -1.0f, +1.0f};
	frustumLocal_.vNear_11 = {-1.0f, 1.0f, +1.0f};
	frustumLocal_.vFar_00 = {1.0f, -1.0f, -1.0f};
	frustumLocal_.vFar_01 = {1.0f, 1.0f, -1.0f};
	frustumLocal_.vFar_10 = {1.0f, -1.0f, +1.0f};
	frustumLocal_.vFar_11 = {1.0f, 1.0f, +1.0f};

	frustumInteractionLocal_ = frustumLocal_;

	width_ = 1;
	height_ = 1;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::WALL)] = true;

	pTypeComponentTriggerable_ = new TrapTypeComponentTriggerableHealingWell;
	static_cast<TrapTypeComponentTriggerableHealingWell *> (pTypeComponentTriggerable_)->timerCreateHealingPotionMax_
		= 20.0f;

	moneyCost_ = 800;
}

std::unique_ptr<TrapComponentTriggerable> TrapTypeComponentTriggerableHealingWell::newComponentObject() const
{ 
	return std::make_unique<TrapComponentTriggerableHealingWell>(
		const_cast<TrapTypeComponentTriggerableHealingWell *> (this));
}

TrapComponentTriggerableHealingWell::TrapComponentTriggerableHealingWell(TrapTypeComponentTriggerable * pParent)
{ pParent_ = pParent; }

void TrapComponentTriggerableHealingWell::interaction(Trap & trap, cPlayer & player)
{
	if (timerCreateHealingPotion_ <= 0.0f)
	{
		if (CheckOBBCollision(&(player.GetFrustumInWorld()), &(trap.getInteractionArea())))
		{
			player.PlayerHPHealed(HEALING_AMOUNT);

			timerCreateHealingPotion_ = static_cast<TrapTypeComponentTriggerableHealingWell *> (
				pParent_)->timerCreateHealingPotionMax_;
		}
	}
}

void TrapComponentTriggerableHealingWell::update(Trap & trap, float fDelta)
{
	timerCreateHealingPotion_ -= fDelta;

	if (timerCreateHealingPotion_ < 0.0f)
		timerCreateHealingPotion_ = 0.0f;
}
