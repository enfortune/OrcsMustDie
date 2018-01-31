#include "stdafx.h"
#include "TrapTypeSpike.h"

#include "TrapTypeComponent.h"

TrapTypeSpike::TrapTypeSpike()
{
	//pModel_;

	frustumLocal_;
	isBlockable_ = false;

	frustumInteractionLocal_ = frustumLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;
	isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::ENEMY)] = true;

	timerType_ = TrapType::eTimerType::COOLDOWN;

	pComponentAttackable_ = new TrapTypeComponentAttackable;

	pComponentAttackable_->cooldownMax_ = 3.0f;
	pComponentAttackable_->damage_ = 3;
	pComponentAttackable_->duration_ = 1.5f;
	pComponentAttackable_->frustumAttackLocal_ = frustumInteractionLocal_;
}
