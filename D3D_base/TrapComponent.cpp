#include "stdafx.h"
#include "TrapComponent.h"

TrapComponentAttackable::TrapComponentAttackable(TrapTypeComponentAttackable * pParent) : pParent_(pParent) {}

void TrapComponentAttackable::attack(std::vector<PlayerDummy> & playerList)
{
}

void TrapComponentAttackable::attack(std::vector<EnemyDummy> & enemyList)
{
	for (int i = 0; i < static_cast<int> (enemyList.size()); ++i)
	{
		//if (enemyList[i].isAttack() && isCollision(frustumAttackWorld_, enemyList[i].frustum))
		//enemyList[i].hit(damage_);
	}
}

void TrapComponentAttackable::update(float fDelta)
{
	cooldown_ -= fDelta;

	if (cooldown_ < 0.0f)
		cooldown_ = 0.0f;
}

TrapComponentBlockable::TrapComponentBlockable(TrapTypeComponentBlockable * pParent) : pParent_(pParent) {}

void TrapComponentBlockable::hit(std::vector<EnemyDummy> & enemyList)
{
	//hp_ -= damage - defense_;

	if (hp_ < 0)
		hp_ = 0;
}

void TrapComponentBlockable::update(float fDelta)
{
}

TrapComponentTriggerable::TrapComponentTriggerable(TrapTypeComponentTriggerable * pParent) : pParent_(pParent) {}

void TrapComponentTriggerable::update(float fDelta)
{
}
