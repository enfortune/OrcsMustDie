#include "stdafx.h"
#include "QuickSlot.h"

#include "cGameUIButton.h"
#include "cGameUISprite.h"

void QuickSlot::changeButton(IconType iconType, int indexButton)
{
	assert(indexButton >= 0 && indexButton < QUICKSLOT_COUNT);

	std::string imagePath {};

	switch (iconType)
	{
	case QuickSlot::IconType::NONE: SAFE_DELETE(pButtonArray_[indexButton]); return;
	case QuickSlot::IconType::ATTACK: imagePath = "Resource/Image/UI/IconBash.png"; break;
	case QuickSlot::IconType::BASH: imagePath = "Resource/Image/UI/IconBash.png"; break;
	case QuickSlot::IconType::WHIRLWIND: imagePath = "Resource/Image/UI/IconWhirlwind.png"; break;
	case QuickSlot::IconType::BARRICADE: imagePath = "Resource/Image/UI/IconBarricade.bmp"; break;
	case QuickSlot::IconType::SPIKE: imagePath = "Resource/Image/UI/IconSpike.bmp"; break;
	case QuickSlot::IconType::HEALING_WELL: imagePath = "Resource/Image/UI/IconHealingWell.bmp"; break;
	default: assert(false);
	}

	delete pButtonArray_[indexButton];
	
	pButtonArray_[indexButton] = new cGameUIButton;
	pButtonArray_[indexButton]->Setup("QuickSlotButton" + std::to_string(indexButton), nullptr, imagePath);

	pSpriteBar_->AddChild(pButtonArray_[indexButton]);
}

void QuickSlot::init()
{
	cGameNode::Setup();

	pSpriteBar_ = new cGameUISprite;
	pSpriteBar_->Setup("Resource/Image/UI/QuickSlot.bmp");
	AddChild(pSpriteBar_);

	changeButton(IconType::ATTACK, 0);
	changeButton(IconType::BARRICADE, 1);
	changeButton(IconType::SPIKE, 2);
	changeButton(IconType::HEALING_WELL, 3);
}

void QuickSlot::update()
{
	cGameNode::Update(g_pTimeManager->GetEllapsedTime());
}

void QuickSlot::render()
{
	cGameNode::Render();
}

void QuickSlot::resetButton()
{
	for (int i = 0; i < QUICKSLOT_COUNT; ++i)
	{
		pButtonArray_[i]->RemoveFromParent();
		SAFE_DELETE(pButtonArray_[i]);
	}
}

void QuickSlot::Delete() { resetButton(); SAFE_DELETE(pSpriteBar_); }
