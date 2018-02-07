#include "stdafx.h"
#include "Trap.h"

#include "TrapType.h"
#include "TrapTypeComponent.h"
#include "TrapComponent.h"

#include "cPlayer.h"
#include "cEnemyBase.h"

void Trap::interaction(cPlayer & player)
{
	if (pType_->isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::PLAYER)])
	{
		if (pComponentAttackable_)
			pComponentAttackable_->attack(*this, player);
	}
}

void Trap::interaction(std::vector<cEnemyBase *> & enemyList)
{
	if (pType_->isInteractionToArray_[static_cast<size_t> (TrapType::eInteractionTo::ENEMY)])
	{
		if (pComponentAttackable_)
			pComponentAttackable_->attack(*this, enemyList);

		if (pComponentBlockable_)
			pComponentBlockable_->hit(*this, enemyList);
	}
}

void Trap::interaction(std::vector<ItemDummy>& itemList)
{
}

void Trap::init(TrapType & type, D3DXMATRIXA16 & matrixWorld)
{
	pType_ = &type;

	matrixWorldList_.resize(type.matrixLocalList_.size());

	for(int i = 0; i < static_cast<int> (matrixWorldList_.size()); ++i)
		matrixWorldList_[i] = type.matrixLocalList_[i] * matrixWorld;

	frustumWorld_ = type.frustumLocal_.TransformCoord(&matrixWorld);
	frustumInteractionWorld_ = type.frustumInteractionLocal_.TransformCoord(&matrixWorld);

	if (type.pTypeComponentAttackable_)
	{
		pComponentAttackable_ = std::move(type.pTypeComponentAttackable_->newComponentObject());
		pComponentAttackable_->frustumAttackWorld_ = pComponentAttackable_->frustumAttackWorld_.TransformCoord(&matrixWorld);
	}

	if (type.pTypeComponentBlockable_)
		pComponentBlockable_ = std::move(type.pTypeComponentBlockable_->newComponentObject());

	if (type.pTypeComponentTriggerable_)
		pComponentTriggerable_ = std::move(type.pTypeComponentTriggerable_->newComponentObject());
}

void Trap::update(float fDelta)
{
	if (pComponentAttackable_)
		pComponentAttackable_->update(*this, fDelta);

	if (pComponentBlockable_)
		pComponentBlockable_->update(*this, fDelta);

	if (pComponentTriggerable_)
		pComponentTriggerable_->update(*this, fDelta);
}

void Trap::render()
{
	pType_->render(matrixWorldList_[renderModelIndex_], renderModelIndex_);
}
