#include "stdafx.h"
#include "TrapTypeComponent.h"

#include "TrapComponent.h"

std::unique_ptr<TrapComponentAttackable> TrapTypeComponentAttackable::newComponentObject() const
{
	std::unique_ptr<TrapComponentAttackable> temp = std::make_unique<TrapComponentAttackable> (const_cast<TrapTypeComponentAttackable *> (this));

	temp->frustumAttackWorld_ = frustumAttackLocal_;
	temp->cooldown_ = cooldownMax_;

	return temp;
}

std::unique_ptr<TrapComponentBlockable> TrapTypeComponentBlockable::newComponentObject() const
{
	std::unique_ptr<TrapComponentBlockable> temp
		= std::make_unique<TrapComponentBlockable> (const_cast<TrapTypeComponentBlockable *>(this));

	temp->hp_ = hpMax_;

	return temp;
}
