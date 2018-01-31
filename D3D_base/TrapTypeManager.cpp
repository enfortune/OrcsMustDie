#include "stdafx.h"
#include "TrapTypeManager.h"

#include "TrapTypeBarricade.h"
#include "TrapTypeSpike.h"
#include "TrapTypeHealingWell.h"

TrapTypeManager::~TrapTypeManager()
{
	for (auto iter = pTrapTypeMap_.begin(); iter != pTrapTypeMap_.end(); ++iter)
		delete iter->second;
}

TrapType * TrapTypeManager::find(std::string key) const
{
	auto check = pTrapTypeMap_.find(key);
	
	if (check != pTrapTypeMap_.end())
		return check->second;

	return nullptr;
}

TrapType * TrapTypeManager::add(std::string key, TrapType * pTrapType)
{
	auto check = pTrapTypeMap_.emplace(std::make_pair(key, pTrapType));

	if (check.second)
		return check.first->second;

	return nullptr;
}

void TrapTypeManager::init()
{
	add("Barricade", new TrapTypeBarricade);
	add("Spike", new TrapTypeSpike);
	add("HealingWell", new TrapTypeHealingWell);
}
