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
	~TrapTypeComponentAttackable() = default;

public:
	virtual TrapComponentAttackable * newComponentObject() const;
};

struct TrapComponentBlockable;

struct TrapTypeComponentBlockable
{
public:
	int hpMax_ {};
	int defense_ {};
	int hpRegenerate_ {};

	float hitCooldownMax_ {};

public:
	TrapTypeComponentBlockable() = default;
	~TrapTypeComponentBlockable() = default;

public:
	virtual TrapComponentBlockable * newComponentObject() const;
};

struct TrapComponentTriggerable;

struct TrapTypeComponentTriggerable
{
public:
	TrapTypeComponentTriggerable() = default;
	virtual ~TrapTypeComponentTriggerable() = default;

public:
	virtual TrapComponentTriggerable * newComponentObject() const = 0;
};