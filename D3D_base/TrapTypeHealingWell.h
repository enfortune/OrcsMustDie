#pragma once

#include "TrapType.h"

struct TrapTypeHealingWell : public TrapType
{
public:
	TrapTypeHealingWell();
	virtual ~TrapTypeHealingWell() = default;

	//UNCOPYABLE(TrapTypeHealingWell);
};