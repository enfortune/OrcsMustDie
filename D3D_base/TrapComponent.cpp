#include "stdafx.h"
#include "TrapComponent.h"

void TrapComponentBlockable::onHit(int damage, EnemyDummy * pEnemy)
{
}

void TrapComponentBlockable::update(float fDelta)
{
}

void TrapComponentTriggerable::update(float fDelta)
{
}

void TrapComponentAttackable::attack(std::vector<PlayerDummy>& playerList)
{
}

void TrapComponentAttackable::attack(std::vector<EnemyDummy>& playerList)
{
}

void TrapComponentAttackable::update(float fDelta)
{
}
