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
	Trap * pTrap_ {};
	TrapTypeComponentAttackable * pParent_ {};

	FRUSTUM frustumAttackWorld_ {};

	float cooldown_ {};

public:
	TrapComponentAttackable(TrapTypeComponentAttackable * pParent = nullptr);
	virtual ~TrapComponentAttackable() = default;

	//UNCOPYABLE(TrapComponentAttackable);

public:
	virtual void attack(cPlayer & player);
	virtual void attack(std::vector<cEnemyBase *> & enemyList);

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapTypeComponentBlockable;

struct TrapComponentBlockable
{
public:
	Trap * pTrap_ {};
	TrapTypeComponentBlockable * pParent_ {};

	int hp_ {};

public:
	TrapComponentBlockable(TrapTypeComponentBlockable * pParent = nullptr);
	virtual ~TrapComponentBlockable() = default;

	//UNCOPYABLE(TrapComponentBlockable);

public:
	virtual void hit(std::vector<cEnemyBase *> & enemyList);

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapTypeComponentTriggerable;

struct TrapComponentTriggerable
{
public:
	Trap * pTrap_ {};
	TrapTypeComponentTriggerable * pParent_ {};

public:
	TrapComponentTriggerable(TrapTypeComponentTriggerable * pParent = nullptr);
	virtual ~TrapComponentTriggerable() = default;

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime()) = 0;
};