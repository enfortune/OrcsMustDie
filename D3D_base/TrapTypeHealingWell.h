#pragma once

#include "TrapType.h"
#include "TrapTypeComponent.h"
#include "TrapComponent.h"

struct TrapTypeHealingWell : public TrapType
{
public:
	TrapTypeHealingWell();
	virtual ~TrapTypeHealingWell() = default;
};

struct TrapTypeComponentTriggerableHealingWell : public TrapTypeComponentTriggerable
{
public:
	float timerCreateHealingPotionMax_ {};

public:
	TrapTypeComponentTriggerableHealingWell() = default;
	virtual ~TrapTypeComponentTriggerableHealingWell() = default;

public:
	virtual std::unique_ptr<TrapComponentTriggerable> newComponentObject() const override;
};

struct TrapComponentTriggerableHealingWell : public TrapComponentTriggerable
{
public:
	float timerCreateHealingPotion_ {};

public:
	TrapComponentTriggerableHealingWell() = default;
	virtual ~TrapComponentTriggerableHealingWell() = default;

public:
	virtual void interaction(Trap & trap, cPlayer & player);

public:
	virtual void update(Trap & trap, float fDelta = g_pTimeManager->GetEllapsedTime()) override;
};