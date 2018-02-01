#include "stdafx.h"
#include "TrapComponent.h"


TrapComponentAttackable::TrapComponentAttackable(TrapTypeComponentAttackable & parent) : parent_(parent) {}

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
}


TrapComponentBlockable::TrapComponentBlockable(TrapTypeComponentBlockable & parent) : parent_(parent) {}

void TrapComponentBlockable::hit(std::vector<EnemyDummy> & enemyList)
{
	//hp_ -= damage - defense_;

	if (hp_ < 0)
		hp_ = 0;
}

void TrapComponentBlockable::update(float fDelta)
{
}

TrapComponentTriggerable::TrapComponentTriggerable(TrapTypeComponentTriggerable & parent) : parent_(parent) {}

void TrapComponentTriggerable::update(float fDelta)
{
}
