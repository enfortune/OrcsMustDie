#pragma once

#include "ClassDummy.h"

class Trap;

struct TrapType;
struct TrapTypeComponentAttackable;

struct TrapComponentAttackable
{
public:
	TrapTypeComponentAttackable & parent_;

	FRUSTUM frustumAttackWorld_ {};

	float cooldown_ {};

public:
	TrapComponentAttackable(TrapTypeComponentAttackable & parent);
	virtual ~TrapComponentAttackable() = default;

	//UNCOPYABLE(TrapComponentAttackable);

public:
	virtual void attack(std::vector<PlayerDummy> & playerList);
	virtual void attack(std::vector<EnemyDummy> & enemyList);

public:
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapTypeComponentBlockable;

struct TrapComponentBlockable
{
public:
	TrapTypeComponentBlockable & parent_;

	int hp_ {};

public:
	TrapComponentBlockable(TrapTypeComponentBlockable & parent);
	virtual ~TrapComponentBlockable() = default;

	//UNCOPYABLE(TrapComponentBlockable);

public:
	virtual void hit(std::vector<EnemyDummy> & enemyList);

public:
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapTypeComponentTriggerable;

struct TrapComponentTriggerable
{
public:
	TrapTypeComponentTriggerable & parent_;

public:
	TrapComponentTriggerable(TrapTypeComponentTriggerable & parent);
	virtual ~TrapComponentTriggerable() = default;

public:
	virtual void trigger() = 0;

public:
	void update(float fDelta = g_pTimeManager->GetEllapsedTime());
};

struct TrapComponentRender
{
public:
	TrapComponentRender() = default;
	virtual ~TrapComponentRender() = default;

public:
	virtual void render(TrapType & trapType, D3DXMATRIXA16 & matrixWorld) = 0;
};