#pragma once

#include "TrapType.h"

struct TrapTypeBarricade : public TrapType
{
public:
	TrapTypeBarricade();
	virtual ~TrapTypeBarricade() = default;

	//UNCOPYABLE(TrapTypeBarricade);
};