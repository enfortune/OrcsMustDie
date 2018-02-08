#include "stdafx.h"
#include "TrapTypeHealingWell.h"

#include "Trap.h"

#include "cPlayer.h"

TrapTypeHealingWell::TrapTypeHealingWell()
{
	modelList_.resize(2);
	modelList_[0].Setup("Resource/Trap/HealingWell", "Resource/Trap/HealingWell/HealingWell.X");
	modelList_[1].Setup("Resource/Trap/HealingWell", "Resource/Trap/HealingWell/HealthPotionDrop.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotationX {}, matrixRotationY {};
	D3DXMATRIXA16 matrixScalePotion {}, matrixTranslation {};

	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixScaling(&matrixScalePotion, 0.013f, 0.013f, 0.013f);
	D3DXMatrixRotationX(&matrixRotationX, D3DX_PI * 0.5f);
	D3DXMatrixRotationY(&matrixRotationY, D3DX_PI * 0.5f);
	D3DXMatrixTranslation(&matrixTranslation, 0.0f, -1.9f, -1.0f);

	D3DXMATRIXA16 matrixIdentity {};
	D3DXMatrixIdentity(&matrixIdentity);

	matrixLocalList_.resize(2);
	matrixLocalList_[0] = matrixScale * matrixRotationX * matrixRotationY;
	matrixLocalList_[1] = matrixScalePotion * matrixRotationX * matrixRotationY * matrixTranslation;

	frustumLocal_.vNear_00 = {-1.0f, -1.0f, -0.3f};
	frustumLocal_.vNear_01 = {-1.0f, 1.0f, -0.3f};
	frustumLocal_.vNear_10 = {-1.0f, -1.0f, +1.0f};
	frustumLocal_.vNear_11 = {-1.0f, 1.0f, +1.0f};
	frustumLocal_.vFar_00 = {1.0f, -1.0f, -0.3f};
	frustumLocal_.vFar_01 = {1.0f, 1.0f, -0.3f};
	frustumLocal_.vFar_10 = {1.0f, -1.0f, +1.0f};
	frustumLocal_.vFar_11 = {1.0f, 1.0f, +1.0f};

	frustumInteractionLocal_ = frustumLocal_;
	frustumInteractionLocal_.vNear_00.z = -2.0f;
	frustumInteractionLocal_.vNear_01.z = -2.0f;
	frustumInteractionLocal_.vFar_00.z = -2.0f;
	frustumInteractionLocal_.vFar_01.z = -2.0f;

	width_ = 1;
	height_ = 1;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::WALL)] = true;
	isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::PLAYER)] = true;

	pTypeComponentTriggerable_ = new TrapTypeComponentTriggerableHealingWell;
	static_cast<TrapTypeComponentTriggerableHealingWell *> (
		pTypeComponentTriggerable_)->timerCreateHealingPotionMax_ = 15.0f;

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
	if (timerCreateHealingPotion_ <= 0.0f && isPotionExist_ && player.GetPlayerCurHp() < player.GetPlayerMaxHp())
	{
		if (CheckOBBCollision(&(player.GetFrustumInWorld()), &(trap.getInteractionArea())))
		{
			player.PlayerHPHealed(HEALING_AMOUNT);

			timerCreateHealingPotion_ = static_cast<TrapTypeComponentTriggerableHealingWell *> (
				pParent_)->timerCreateHealingPotionMax_;

			isPotionExist_ = false;
			trap.setRenderModel(1, false);
		}
	}
}

void TrapComponentTriggerableHealingWell::update(Trap & trap, float fDelta)
{
	timerCreateHealingPotion_ -= fDelta;

	if (timerCreateHealingPotion_ < 0.0f)
		timerCreateHealingPotion_ = 0.0f;

	if (timerCreateHealingPotion_ == 0.0f)
	{
		isPotionExist_ = true;
		trap.setRenderModel(1, true);
	}
}
