#include "stdafx.h"
#include "TrapTypeComponent.h"

#include "TrapComponent.h"

TrapComponentAttackable * TrapTypeComponentAttackable::newComponentObject() const
{
	TrapComponentAttackable * temp = new TrapComponentAttackable(const_cast<TrapTypeComponentAttackable *>(this));

	//temp->matrixAttackWorld_
	temp->frustumAttackWorld_ = frustumAttackLocal_;
	temp->cooldown_ = cooldownMax_;

	return temp;
}

TrapComponentBlockable * TrapTypeComponentBlockable::newComponentObject() const
{
	TrapComponentBlockable * temp = new TrapComponentBlockable(const_cast<TrapTypeComponentBlockable *>(this));
	temp->hp_ = hpMax_;

	return temp;
}
