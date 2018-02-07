#pragma once

#include "cGameNode.h"

class cGameUIButton;
class cGameUISprite;

class QuickSlot : public cGameNode
{
private:
	enum { QUICKSLOT_COUNT = 10 };
	enum class IconType { NONE, ATTACK, BASH, WHIRLWIND, BARRICADE, SPIKE, HEALING_WELL };

	cGameUISprite * pSpriteBar_ {};
	cGameUISprite * pSpriteSelect_ {};

	D3DXMATRIXA16 matrixWorld_ {};

private:
	cGameUIButton * pButtonArray_[QUICKSLOT_COUNT] {};

private:
	void selectButton(int indexButton);
	void changeButton(IconType iconType, int indexButton);
	void removeButton(int indexButton);

public:
	QuickSlot() = default;
	virtual ~QuickSlot();

public:
	void init();
	void update();
	void render();

	void resetButton();

	virtual void Delete();
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
