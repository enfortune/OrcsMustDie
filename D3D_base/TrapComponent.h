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

public:
	TrapComponentBlockable(TrapTypeComponentBlockable * pParent = nullptr);
	virtual ~TrapComponentBlockable() = default;

	//UNCOPYABLE(TrapComponentBlockable);

public:
	virtual void hit(Trap & trap, std::vector<cEnemyBase *> & enemyList);

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