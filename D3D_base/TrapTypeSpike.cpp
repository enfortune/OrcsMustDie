#include "stdafx.h"
#include "TrapTypeSpike.h"

#include "TrapTypeComponent.h"

TrapTypeSpike::TrapTypeSpike()
{
	//pModel_;

	frustumLocal_;
	isBlockable_ = false;

	width_ = 2;
	height_ = 2;

	frustumInteractionLocal_ = frustumLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;
	isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::ENEMY)] = true;

	timerType_ = TrapType::eTimerType::COOLDOWN;

	pTypeComponentAttackable_ = new TrapTypeComponentAttackable;

	pTypeComponentAttackable_->cooldownMax_ = 3.0f;
	pTypeComponentAttackable_->damage_ = 3;
	pTypeComponentAttackable_->duration_ = 1.5f;
	pTypeComponentAttackable_->frustumAttackLocal_ = frustumInteractionLocal_;
}
