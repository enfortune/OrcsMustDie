#include "stdafx.h"
#include "TrapTypeComponent.h"

#include "TrapComponent.h"

TrapComponentAttackable * TrapTypeComponentAttackable::newComponentObject() const
{
	TrapComponentAttackable * temp = new TrapComponentAttackable;

	temp->frustumAttackWorld_ = frustumAttackLocal_;
	temp->cooldown_ = cooldownMax_;

	return temp;
}

TrapComponentBlockable * TrapTypeComponentBlockable::newComponentObject() const
{
	TrapComponentBlockable * temp = new TrapComponentBlockable;
	temp->hp_ = hpMax_;

	return temp;
}
