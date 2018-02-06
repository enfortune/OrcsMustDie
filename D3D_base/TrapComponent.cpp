#include "stdafx.h"
#include "TrapComponent.h"

#include "TrapTypeComponent.h"
#include "Trap.h"

#include "cPlayer.h"
#include "cEnemyBase.h"

TrapComponentAttackable::TrapComponentAttackable(TrapTypeComponentAttackable * pParent) : pParent_(pParent) {}

void TrapComponentAttackable::attack(cPlayer & player)
{
}

void TrapComponentAttackable::attack(std::vector<cEnemyBase *> & enemyList)
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

TrapComponentBlockable::TrapComponentBlockable(TrapTypeComponentBlockable * pParent) : pParent_(pParent) {}

void TrapComponentBlockable::hit(std::vector<cEnemyBase *> & enemyList)
{
	//hp_ -= damage_ - defense_;

	if (hp_ < 0)
		hp_ = 0;
}

void TrapComponentBlockable::update(Trap & trap, float fDelta)
{
}

TrapComponentTriggerable::TrapComponentTriggerable(TrapTypeComponentTriggerable * pParent) : pParent_(pParent) {}
