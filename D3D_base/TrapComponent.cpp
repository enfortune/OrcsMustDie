#include "stdafx.h"
#include "TrapComponent.h"

#include "TrapTypeComponent.h"
#include "Trap.h"

#include "cPlayer.h"
#include "cEnemyBase.h"

TrapComponentAttackable::TrapComponentAttackable(TrapTypeComponentAttackable * pParent) : pParent_(pParent)
{
	if (pParent)
	{
		cooldown_ = pParent->cooldownMax_;
		frustumAttackWorld_ = pParent->frustumAttackLocal_;
	}
}

void TrapComponentAttackable::attack(Trap & trap, cPlayer & player)
{
}

void TrapComponentAttackable::attack(Trap & trap, std::vector<cEnemyBase *> & enemyList)
{
	if (cooldown_ <= 0.0f)
	{
		for (int i = 0; i < static_cast<int> (enemyList.size()); ++i)
		{
			if (CheckOBBCollision(&(enemyList[i]->GetFrustumInWorld()), &frustumAttackWorld_))
			{
				enemyList[i]->getDamage(pParent_->damage_);
				cooldown_ = pParent_->cooldownMax_;
			}
		}
	}
}

void TrapComponentAttackable::update(Trap & trap, float fDelta)
{
	cooldown_ -= fDelta;

	if (cooldown_ < 0.0f)
		cooldown_ = 0.0f;
}

// === blockable =================================================================================================

void TrapComponentBlockable::onDestroy(Trap & trap)
{
	isBlockable_ = false;
	timerRemain_ = pParent_->timerRemainMax_;
}

TrapComponentBlockable::TrapComponentBlockable(TrapTypeComponentBlockable * pParent) : pParent_(pParent)
{
	if (pParent)
	{
		hp_ = pParent->hpMax_;
		timerRemain_ = pParent->timerRemainMax_;
	}
}

void TrapComponentBlockable::onHit(Trap & trap, int damage)
{
	hp_ -= damage;

	if (hp_ < 0)
		hp_ = 0;

	if (hp_ == 0 && isBlockable_)
		onDestroy(trap);
}

void TrapComponentBlockable::update(Trap & trap, float fDelta)
{
	if (isBlockable_)
		hp_ += pParent_->hpRegenerate_ * fDelta;
	else
	{
		timerRemain_ -= fDelta;

		if (timerRemain_ < 0.0f)
			timerRemain_ = 0.0f;

		if (timerRemain_ == 0.0f)
			isRemovable_ = true;
	}
}

// === triggerable ================================================================================================

