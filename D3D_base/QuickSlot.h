#pragma once

#include "cGameNode.h"

class cGameUIButton;
class TrapType;

class QuickSlot : public cGameNode
{
public:
	enum class eCategory { NONE, WEAPON, TRAP, TRINKET };

private:
	enum { QUICKSLOT_COUNT = 10 };

private:
	union uQuickSlotData;
	struct QuickSlotData;

private:
	cGameUIButton * pButtonArray_[QUICKSLOT_COUNT] {};

	

public:
	

public:
	void init();
	void update();
	void render();
};

union QuickSlot::uQuickSlotData
{

};

struct QuickSlotData
{

};