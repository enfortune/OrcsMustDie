#include "stdafx.h"
#include "QuickSlot.h"

#include "cGameUIButton.h"
#include "cGameUISprite.h"
#include "cGameSprite.h"

void QuickSlot::selectButton(int indexButton)
{
	assert(indexButton >= 0 && indexButton < QUICKSLOT_COUNT);

	if (pButtonArray_[indexButton])
	{
		if (pSpriteSelect_->GetParentNode() == nullptr)
			pSpriteBar_->AddChild(pSpriteSelect_);

		pSpriteSelect_->SetPosition({indexButton * 59.1f - 290 + 25.0f, 0.0f});
	}
	else if (pSpriteSelect_->GetParentNode())
		pSpriteSelect_->RemoveFromParent();
}

void QuickSlot::changeButton(IconType iconType, int indexButton)
{
	assert(indexButton >= 0 && indexButton < QUICKSLOT_COUNT);

	std::string imagePath {};

	switch (iconType)
	{
	case QuickSlot::IconType::NONE: removeButton(indexButton); break;
	case QuickSlot::IconType::ATTACK: imagePath = "Resource/Image/UI/IconAttack.png"; break;
	case QuickSlot::IconType::BASH: imagePath = "Resource/Image/UI/IconBash.png"; break;
	case QuickSlot::IconType::WHIRLWIND: imagePath = "Resource/Image/UI/IconWhirlwind.png"; break;
	case QuickSlot::IconType::BARRICADE: imagePath = "Resource/Image/UI/IconBarricade.png"; break;
	case QuickSlot::IconType::SPIKE: imagePath = "Resource/Image/UI/IconSpike.png"; break;
	case QuickSlot::IconType::HEALING_WELL: imagePath = "Resource/Image/UI/IconHealingWell.png"; break;
	default: assert(false);
	}

	removeButton(indexButton);
	
	pButtonArray_[indexButton] = new cGameUIButton;
	pButtonArray_[indexButton]->Setup("QuickSlotButton" + std::to_string(indexButton), nullptr, imagePath);
	pButtonArray_[indexButton]->SetPosition({indexButton * 59.1f - 290 + 25.0f, 0.0f});

	pSpriteBar_->AddChild(pButtonArray_[indexButton]);

	selectButton(0);
}

void QuickSlot::removeButton(int indexButton)
{
	if (pButtonArray_[indexButton])
	{
		pButtonArray_[indexButton]->RemoveFromParent();
		SAFE_RELEASE(pButtonArray_[indexButton]);
		pButtonArray_[indexButton] = nullptr;
	}
}

QuickSlot::~QuickSlot() { Delete(); }

void QuickSlot::init()
{
	cGameNode::Setup();

	RECT rectWindow {};
	GetWindowRect(g_hWnd, &rectWindow);

	pSpriteBar_ = new cGameUISprite;
	pSpriteBar_->Setup("Resource/Image/UI/QuickSlot.png");
	pSpriteBar_->SetPosition({(rectWindow.right - rectWindow.left) * 0.5f, (rectWindow.bottom - rectWindow.top) * 0.83f});
	pSpriteBar_->GetSprite()->SetSpriteFlag(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE | D3DXSPRITE_ALPHABLEND);

	AddChild(pSpriteBar_);

	pSpriteSelect_ = new cGameUISprite;
	pSpriteSelect_->Setup("Resource/Image/UI/IconEffectMouseOver.png");
	pSpriteSelect_->GetSprite()->SetSpriteFlag(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE | D3DXSPRITE_ALPHABLEND);

	changeButton(IconType::ATTACK, 0);
	changeButton(IconType::BASH, 1);
	changeButton(IconType::WHIRLWIND, 2);
	changeButton(IconType::BARRICADE, 3);
	changeButton(IconType::SPIKE, 4);
	changeButton(IconType::HEALING_WELL, 5);
}

void QuickSlot::update()
{
	for (int i = 0; i < 9; ++i)
	{
		if (g_pKeyManager->IsOnceKeyDown(static_cast<char> ('1' + i)))
			selectButton(i);
	}

	if (g_pKeyManager->IsOnceKeyDown('0'))
		selectButton(9);

	cGameNode::Update(g_pTimeManager->GetEllapsedTime());
}

void QuickSlot::render()
{
	cGameNode::Render();
}

void QuickSlot::resetButton()
{
	for (int i = 0; i < QUICKSLOT_COUNT; ++i)
		removeButton(i);
}

void QuickSlot::Delete() 
{
	resetButton();

	if (pSpriteSelect_->GetParentNode())
		pSpriteSelect_->RemoveFromParent();

	SAFE_DELETE(pSpriteSelect_);
	SAFE_DELETE(pSpriteBar_);
}
