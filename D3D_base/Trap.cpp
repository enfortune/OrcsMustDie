#include "stdafx.h"
#include "Trap.h"

#include "TrapType.h"
#include "TrapTypeComponent.h"
#include "TrapComponent.h"

Trap::~Trap()
{
	delete pComponentAttackable_; pComponentAttackable_ = nullptr;
	delete pComponentBlockable_; pComponentBlockable_ = nullptr;
	delete pComponentTriggerable_; pComponentTriggerable_ = nullptr;
}

void Trap::interaction(std::vector<PlayerDummy>& playerList)
{
	if (pType_->isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::PLAYER)])
	{
		if (pComponentAttackable_)
			pComponentAttackable_->attack(playerList);
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
	}
}

void Trap::interaction(std::vector<ItemDummy>& itemList)
{
}

void Trap::init(TrapType & type, D3DXMATRIXA16 & matrixWorld)
{
	pType_ = &type;
	matrixWorld_ = type.matrixLocal_ * matrixWorld;

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
	pType_->render(matrixWorld_, renderModelIndex_);
}
