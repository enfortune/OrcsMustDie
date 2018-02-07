#pragma once

#include "cGameScene.h"

class cGameSprite;

class SceneMainMenu : public cGameScene
{
private:
	enum { SPRITE_COUNT = 4 };
	enum { BUTTON_COUNT = 3 };

private:
	cGameSprite * pSpriteArray_[SPRITE_COUNT] {};
	RECT rectButtonArea_[BUTTON_COUNT] {};

	int renderIndex_ {};

private:
	void onButtonClick(int buttonIndex);

public:
	SceneMainMenu() = default;
	virtual ~SceneMainMenu();

public:
	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;
};