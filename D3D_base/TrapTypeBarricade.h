#pragma once

#include "TrapType.h"
#include "TrapComponent.h"

struct TrapTypeBarricade : public TrapType
{
public:
	TrapTypeBarricade();
	virtual ~TrapTypeBarricade() = default;
};

struct TrapComponentBarricade : public TrapComponentBlockable
{
public:
	TrapComponentBarricade() = default;
	virtual ~TrapComponentBarricade() = default;

protected:
	virtual void onDestroy(Trap & trap) override;
};