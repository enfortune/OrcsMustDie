#include "stdafx.h"
#include "TrapTypeHealingWell.h"

TrapTypeHealingWell::TrapTypeHealingWell()
{
	//pModel_ = ;

	frustumLocal_;
	isBlockable_ = false;

	width_ = 2;
	height_ = 3;

	frustumInteractionLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::WALL)] = true;
	isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::TRIGGER)] = true;

	timerType_ = TrapType::eTimerType::REPAIR;
}
