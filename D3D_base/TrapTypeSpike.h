#pragma once

#include "TrapType.h"
#include "TrapTypeComponent.h"
#include "TrapComponent.h"

class cEnemyBase;

struct TrapTypeSpike : public TrapType
{
public:
	TrapTypeSpike();
	virtual ~TrapTypeSpike() = default;

public:
	//virtual void render(D3DXMATRIXA16 & matrixWorld) override;
};

struct TrapTypeComponentAttackableSpike : public TrapTypeComponentAttackable
{
public:
	TrapTypeComponentAttackableSpike() = default;
	virtual ~TrapTypeComponentAttackableSpike() = default;

public:
	virtual std::unique_ptr<TrapComponentAttackable> newComponentObject() const override;
};

class TrapComponentAttackableSpike : public TrapComponentAttackable
{
public:
	TrapComponentAttackableSpike(TrapTypeComponentAttackable * pParent = nullptr);
	virtual ~TrapComponentAttackableSpike() = default;

public:
	virtual void attack(Trap & trap, std::vector<cEnemyBase *> & enemyList) override;

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime()) override;
};