#pragma once

#include "TrapType.h"
#include "TrapTypeComponent.h"
#include "TrapComponent.h"

struct TrapTypeBarricade : public TrapType
{
public:
	TrapTypeBarricade();
	virtual ~TrapTypeBarricade() = default;
};

struct TrapTypeComponentBlockableBarricade : public TrapTypeComponentBlockable
{
public:
	TrapTypeComponentBlockableBarricade() = default;
	virtual ~TrapTypeComponentBlockableBarricade() = default;

public:
	virtual std::unique_ptr<TrapComponentBlockable> newComponentObject() const override;
};

struct TrapComponentBlockableBarricade : public TrapComponentBlockable
{
public:
	TrapComponentBlockableBarricade(TrapTypeComponentBlockable * pParent = nullptr);
	virtual ~TrapComponentBlockableBarricade() = default;

public:
	virtual void onHit(Trap & trap, int damage) override;
};