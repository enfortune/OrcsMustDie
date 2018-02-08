#include "stdafx.h"
#include "TrapTypeSpike.h"

#include "TrapTypeComponent.h"
#include "Trap.h"

#include "cEnemyBase.h"

TrapTypeSpike::TrapTypeSpike()
{
	modelList_.resize(2);
	modelList_[0].Setup("Resource/Trap/Spike", "Resource/Trap/Spike/SpikesFloor.X");
	modelList_[1].Setup("Resource/Trap/Spike", "Resource/Trap/Spike/StationarySpikes.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotation {}, matrixRotation2 {}, matrixRotation3 {};
	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);

	D3DXMatrixRotationX(&matrixRotation, D3DX_PI * 0.5f);
	D3DXMatrixRotationY(&matrixRotation2, D3DX_PI * 0.5f);
	D3DXMatrixRotationX(&matrixRotation3, D3DX_PI * 1.0f);

	matrixLocalList_.resize(2);
	matrixLocalList_[0] = matrixScale * matrixRotation;
	matrixLocalList_[1] = matrixScale * matrixRotation3 * matrixRotation2;

	frustumLocal_.vNear_00 = {-1.0f, 0.0f, -1.0f};
	frustumLocal_.vNear_01 = {-1.0f, 0.6f, -1.0f};
	frustumLocal_.vNear_10 = {-1.0f, 0.0f, +1.0f};
	frustumLocal_.vNear_11 = {-1.0f, 0.6f, +1.0f};
	frustumLocal_.vFar_00 = {1.0f, 0.0f, -1.0f};
	frustumLocal_.vFar_01 = {1.0f, 0.6f, -1.0f};
	frustumLocal_.vFar_10 = {1.0f, 0.0f, +1.0f};
	frustumLocal_.vFar_11 = {1.0f, 0.6f, +1.0f};

	/*D3DXMATRIXA16 matrixInverse {};
	D3DXMatrixInverse(&matrixInverse, nullptr, &(matrixLocalList_[0]));

	frustumLocal_ = frustumLocal_.TransformCoord(&matrixInverse);*/
	isBlockable_ = false;

	width_ = 1;
	height_ = 1;

	frustumInteractionLocal_ = frustumLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;
	isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::ENEMY)] = true;

	timerType_ = TrapType::eTimerType::COOLDOWN;

	pTypeComponentAttackable_ = new TrapTypeComponentAttackableSpike;

	pTypeComponentAttackable_->cooldownMax_ = 3.0f;
	pTypeComponentAttackable_->damage_ = 3;
	pTypeComponentAttackable_->duration_ = 1.5f;
	pTypeComponentAttackable_->frustumAttackLocal_ = frustumInteractionLocal_;

	moneyCost_ = 300;
}

std::unique_ptr<TrapComponentAttackable> TrapTypeComponentAttackableSpike::newComponentObject() const
{ return std::make_unique<TrapComponentAttackableSpike>(const_cast<TrapTypeComponentAttackableSpike *> (this)); }

TrapComponentAttackableSpike::TrapComponentAttackableSpike(TrapTypeComponentAttackable * pParent) { pParent_ = pParent; }

void TrapComponentAttackableSpike::attack(Trap & trap, std::vector<cEnemyBase*> & enemyList)
{
	if (cooldown_ <= 0.0f)
	{
		for (int i = 0; i < static_cast<int> (enemyList.size()); ++i)
		{
			if (CheckOBBCollision(&(enemyList[i]->GetFrustumInWorld()), &frustumAttackWorld_))
			{
				enemyList[i]->getDamage(pParent_->damage_);
				cooldown_ = pParent_->cooldownMax_;

				trap.setRenderModel(0);
			}
		}
	}
}

void TrapComponentAttackableSpike::update(Trap & trap, float fDelta)
{
	TrapComponentAttackable::update(trap, fDelta);

	if (cooldown_ <= pParent_->cooldownMax_ - pParent_->duration_)
		trap.setRenderModel(1);
}
