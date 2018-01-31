#include "stdafx.h"
#include "Trap.h"

#include "TrapType.h"
#include "TrapTypeComponent.h"

void Trap::init(TrapType & type, D3DXMATRIXA16 & matrixWorld)
{
	pType_ = &type;
	matrixWorld_ = matrixWorld;

	frustumWorld_ = type.frustumLocal_.TransformCoord(&matrixWorld);
	frustumInteractionWorld_ = type.frustumInteractionLocal_.TransformCoord(&matrixWorld);

	if (type.pComponentAttackable_)
	{
		pComponentAttackable_ = type.pComponentAttackable_->newComponentObject();
		frustumInteractionWorld_ = pComponentAttackable_->frustumAttackWorld_.TransformCoord(&matrixWorld);
	}

	if (type.pComponentBlockable_)
		pComponentBlockable_ = type.pComponentBlockable_->newComponentObject();

	if (type.pComponentTriggerable_)
		pComponentTriggerable_ = type.pComponentTriggerable_->newComponentObject();
}

void Trap::update(float fDelta)
{
	if (pComponentAttackable_)
		pComponentAttackable_->update(fDelta);

	if (pComponentBlockable_)
		pComponentBlockable_->update(fDelta);

	if (pComponentTriggerable_)
		pComponentTriggerable_->update(fDelta);
}

void Trap::render() { pType_->render(matrixWorld_); }
