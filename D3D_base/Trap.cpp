#include "stdafx.h"
#include "Trap.h"

#include "TrapType.h"
#include "TrapTypeComponent.h"

Trap::~Trap()
{
	delete pComponentAttackable_;
	delete pComponentBlockable_;
	delete pComponentTriggerable_;
}

void Trap::interaction(std::vector<PlayerDummy>& playerList)
{
	if (pType_->isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::PLAYER)])
	{
		if (pComponentAttackable_)
			pComponentAttackable_->attack(playerList);

		if (pComponentTriggerable_)
			pComponentTriggerable_->trigger();
	}
}

void Trap::interaction(std::vector<EnemyDummy>& enemyList)
{
	if (pType_->isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::ENEMY)])
	{
		if (pComponentAttackable_)
			pComponentAttackable_->attack(enemyList);

		if (pComponentBlockable_)
			pComponentBlockable_->hit(enemyList);

		if (pComponentTriggerable_)
			pComponentTriggerable_->trigger();
	}
}

void Trap::interaction(std::vector<ItemDummy>& itemList)
{
}

void Trap::init(TrapType & type, D3DXMATRIXA16 & matrixWorld)
{
	pType_ = &type;
	matrixWorld_ = matrixWorld * type.matrixLocal_;

	frustumWorld_ = type.frustumLocal_.TransformCoord(&matrixWorld);
	frustumInteractionWorld_ = type.frustumInteractionLocal_.TransformCoord(&matrixWorld);

	if (type.pTypeComponentAttackable_)
	{
		pComponentAttackable_ = type.pTypeComponentAttackable_->newComponentObject();
		frustumInteractionWorld_ = pComponentAttackable_->frustumAttackWorld_.TransformCoord(&matrixWorld);
	}

	if (type.pTypeComponentBlockable_)
		pComponentBlockable_ = type.pTypeComponentBlockable_->newComponentObject();

	if (type.pTypeComponentTriggerable_)
		pComponentTriggerable_ = type.pTypeComponentTriggerable_->newComponentObject();
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

void Trap::render() 
{
	if (pComponentRender_)
		pComponentRender_->render(*pType_, matrixWorld_);
	else
		pType_->render(matrixWorld_);
}
