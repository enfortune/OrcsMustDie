#pragma once

#include "TrapType.h"
#include "TrapTypeComponent.h"

struct TrapTypeSpike : public TrapType
{
public:
	TrapTypeSpike();
	virtual ~TrapTypeSpike() = default;

public:
	//virtual void render(D3DXMATRIXA16 & matrixWorld) override;
};

struct TrapTypeAttackableSpike : public TrapTypeComponentAttackable
{
public:
	TrapTypeAttackableSpike() = default;
	virtual ~TrapTypeAttackableSpike() = default;

public:
	virtual TrapComponentAttackable * newComponentObject() const;
};