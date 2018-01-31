#include "stdafx.h"
#include "TrapTypeBarricade.h"

#include "TrapTypeComponent.h"

TrapTypeBarricade::TrapTypeBarricade()
{
	//pModel_ = ;

	frustumLocal_.vNear_00 = {-0.5f, 0.0f, -0.5f};
	frustumLocal_.vNear_01 = {-0.5f, 0.3f, -0.5f};
	frustumLocal_.vNear_10 = {-0.5f, 0.0f, +0.5f};
	frustumLocal_.vNear_11 = {-0.5f, 0.3f, +0.5f};
	frustumLocal_.vFar_00 = {0.5f, 0.0f, -0.5f};
	frustumLocal_.vFar_01 = {0.5f, 0.3f, -0.5f};
	frustumLocal_.vFar_10 = {0.5f, 0.0f, +0.5f};
	frustumLocal_.vFar_11 = {0.5f, 0.3f, +0.5f};

	isBlockable_ = true;

	frustumInteractionLocal_ = frustumLocal_;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;

	pComponentBlockable_ = new TrapTypeComponentBlockable;
	pComponentBlockable_->hpMax_ = 80;
}

TrapTypeBarricade::~TrapTypeBarricade() { delete pComponentBlockable_; }
