#pragma once

#include "ClassDummy.h"

struct TrapComponentAttackable;

struct TrapTypeComponentAttackable
{
public:
	FRUSTUM frustumAttackLocal_ {};

	int damage_ {};
	float duration_ {};
	float cooldownMax_ {};

public:
	TrapTypeComponentAttackable() = default;
	virtual ~TrapTypeComponentAttackable() = default;

public:
	virtual std::unique_ptr<TrapComponentAttackable> newComponentObject() const;
};

// === blockable ========================================================================

struct TrapComponentBlockable;

struct TrapTypeComponentBlockable
{
public:
	int hpMax_ {};
	int defense_ {};
	int hpRegenerate_ {};

	float timerRemainMax_ {};

public:
	TrapTypeComponentBlockable() = default;
	virtual ~TrapTypeComponentBlockable() = default;

public:
	virtual std::unique_ptr<TrapComponentBlockable> newComponentObject() const;
};

// === triggerable =======================================================================

struct TrapComponentTriggerable;

struct TrapTypeComponentTriggerable
{
public:
	TrapTypeComponentTriggerable() = default;
	virtual ~TrapTypeComponentTriggerable() = default;

public:
	virtual std::unique_ptr<TrapComponentTriggerable> newComponentObject() const = 0;
};