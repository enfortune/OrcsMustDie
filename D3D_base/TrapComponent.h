#pragma once

#include "ClassDummy.h"

class Trap;

struct TrapType;
struct TrapTypeComponentAttackable;

class cPlayer;
class cEnemyBase;

struct TrapComponentAttackable
{
public:
	TrapTypeComponentAttackable * pParent_ {};

	FRUSTUM frustumAttackWorld_ {};

	float cooldown_ {};

public:
	TrapComponentAttackable(TrapTypeComponentAttackable * pParent = nullptr);
	virtual ~TrapComponentAttackable() = default;

	//UNCOPYABLE(TrapComponentAttackable);

public:
	virtual void attack(Trap & trap, cPlayer & player);
	virtual void attack(Trap & trap, std::vector<cEnemyBase *> & enemyList);

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapTypeComponentBlockable;

struct TrapComponentBlockable
{
public:
	TrapTypeComponentBlockable * pParent_ {};

	int hp_ {};

	bool isBlockable_ {};
	bool isRemovable_ {};

	float timerRemain_ {};

protected:
	virtual void onDestroy(Trap & trap);

public:
	TrapComponentBlockable(TrapTypeComponentBlockable * pParent = nullptr);
	virtual ~TrapComponentBlockable() = default;

	//UNCOPYABLE(TrapComponentBlockable);

public:
	inline bool isBlockable() const { return isBlockable_; }
	inline bool isRemovable() const { return isRemovable_; }

public:
	virtual void onHit(Trap & trap, int damage);

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapTypeComponentTriggerable;

struct TrapComponentTriggerable
{
public:
	TrapTypeComponentTriggerable * pParent_ {};

public:
	TrapComponentTriggerable(TrapTypeComponentTriggerable * pParent = nullptr);
	virtual ~TrapComponentTriggerable() = default;

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime()) = 0;
};