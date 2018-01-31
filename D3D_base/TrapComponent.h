#pragma once

#include "ClassDummy.h"

class Trap;

struct TrapComponentAttackable
{
public:
	FRUSTUM frustumAttackWorld_ {};

	float cooldown_ {};

public:
	TrapComponentAttackable() = default;
	virtual ~TrapComponentAttackable() = default;

	//UNCOPYABLE(TrapComponentAttackable);

public:
	virtual void attack(std::vector<PlayerDummy> & playerList);
	virtual void attack(std::vector<EnemyDummy> & enemyList);

public:
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapComponentBlockable
{
public:
	int hp_ {};

public:
	TrapComponentBlockable() = default;
	virtual ~TrapComponentBlockable() = default;

	//UNCOPYABLE(TrapComponentBlockable);

public:
	virtual void onHit(int damage, EnemyDummy * pEnemy = nullptr);

public:
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapComponentTriggerable
{
public:

public:
	TrapComponentTriggerable() = default;
	virtual ~TrapComponentTriggerable() = default;

public:
	virtual void trigger() = 0;

public:
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
};