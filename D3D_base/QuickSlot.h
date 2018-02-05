#pragma once

#include "cGameNode.h"

class cGameUIButton;

class QuickSlot : public cGameNode
{
private:
	enum { QUICKSLOT_COUNT = 10 };

private:
	cGameUIButton * pButtonArray_[QUICKSLOT_COUNT] {};

public:
	QuickSlot();

public:

};

//#include "cGameNode.h"
//
//#include "cGameUIButton.h"
//#include "TrapType.h"
//
//class cGameUIButton;
//struct TrapType;
//
//class WeaponDummy;
//
//enum class eCategory { NONE, WEAPON, TRAP, TRINKET };
//
//class QuickSlot : public cGameNode
//{
//private:
//	enum { QUICKSLOT_COUNT = 10 };
//
//private:
//	union uQuickSlotData
//	{
//		WeaponDummy * pWeapon {};
//		TrapType * pTrap;
//	};
//
//	struct QuickSlotData
//	{
//		eCategory category_ {eCategory::NONE};
//		uQuickSlotData data_ {};
//		cGameUIButton * pButton_ {};
//	};
//
//private:
//	QuickSlotData data[QUICKSLOT_COUNT];
//
//public:
//	
//
//public:
//	void init();
//	void update();
//	void render();
//};
