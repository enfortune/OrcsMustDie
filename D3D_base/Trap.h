#pragma once

// created in-game object

#include "TrapType.h"
#include "TrapComponent.h"

class cPlayer;
class cEnemyBase;

class Trap
{
private:
	TrapType * pType_ {};
	std::vector<D3DXMATRIXA16> matrixWorldList_ {};

	// cache
	FRUSTUM frustumWorld_ {};
	FRUSTUM frustumInteractionWorld_ {};

	int renderModelIndex_ {};

	std::unique_ptr<TrapComponentAttackable> pComponentAttackable_ {};
	std::unique_ptr<TrapComponentBlockable> pComponentBlockable_ {};
	std::unique_ptr<TrapComponentTriggerable> pComponentTriggerable_ {};

public:
	Trap() = default;

public:
	inline bool isConstructible(TrapType::eInstallPosition installPosition) const
	{ return pType_->isInstallPositionArray_[static_cast<int> (installPosition)]; }

	inline bool isBlockable() const { return pType_->isBlockable_; }

	inline int getWidth() const { return pType_->width_; }
	inline int getHeight() const { return pType_->height_; }

	inline FRUSTUM getFrustum() const { return frustumWorld_; }
	inline FRUSTUM getInteractionArea() const { return frustumInteractionWorld_; }

	inline float getFrustumMaxLength() const { return pType_->getFrustumMaxLength(); }
	D3DXVECTOR3 getFrustumCenter() const;

public:		// for component
	inline void setRenderModelIndex(int index) { renderModelIndex_ = index; }

public:
	void interaction(cPlayer & player);
	void interaction(std::vector<cEnemyBase *> & enemyList);
	void interaction(std::vector<ItemDummy> & itemList);

	void onHit(int damage);

public:
	void init(TrapType & type, D3DXMATRIXA16 & matrixWorld);
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
	void render();
};
