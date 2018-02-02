#pragma once

// created in-game object

#include "TrapType.h"
#include "TrapComponent.h"

#include "ClassDummy.h"

class Trap
{
private:
	TrapType * pType_ {};
	D3DXMATRIXA16 matrixWorld_ {};

	// cache
	FRUSTUM frustumWorld_ {};
	FRUSTUM frustumInteractionWorld_ {};

	TrapComponentAttackable * pComponentAttackable_ {};
	TrapComponentBlockable * pComponentBlockable_ {};
	TrapComponentTriggerable * pComponentTriggerable_ {};

public:
	Trap() = default;

public:
	inline bool isConstructible(TrapType::eInstallPosition installPosition) const
	{ return pType_->isInstallPositionArray_[static_cast<int> (installPosition)]; }
	
	inline bool isBlockable() const { return pType_->isBlockable_; }
	inline FRUSTUM getFrustum() const { return frustumWorld_; }
	inline FRUSTUM getInteractionArea() const { return frustumInteractionWorld_; }

public:
	void interaction(std::vector<PlayerDummy> & playerList);
	void interaction(std::vector<EnemyDummy> & enemyList);
	void interaction(std::vector<ItemDummy> & itemList);

	void onTrigger();

public:
	void init(TrapType & type, D3DXMATRIXA16 & matrixWorld);
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
	void render();
};