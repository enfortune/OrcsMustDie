#include "stdafx.h"
#include "SceneMainMenu.h"

#include "cGameSprite.h"

#include "cInGameScene.h"

void SceneMainMenu::onButtonClick(int buttonIndex)
{
	switch (buttonIndex)
	{
	case 0: g_pSceneManager->PushScene(new cInGameScene);  break;
	case 1:
	case 2: PostQuitMessage(0); break;
	}
}

SceneMainMenu::~SceneMainMenu() { Delete(); }

void SceneMainMenu::Setup()
{
	std::string fileName[SPRITE_COUNT];
	
	for (int i = 0; i < SPRITE_COUNT; ++i)
		fileName[i] = "Resource/Image/Scene/";

	fileName[0] += "MainMenu.png";
	fileName[1] += "MainMenuSelectPlay.png";
	fileName[2] += "MainMenuSelectOption.png";
	fileName[3] += "MainMenuSelectExit.png";

	rectButtonArea_[0] = {400, 344, 710, 404};
	rectButtonArea_[1] = {464, 476, 640, 518};
	rectButtonArea_[2] = {464, 534, 640, 578};

	RECT rectWindow {};
	GetWindowRect(g_hWnd, &rectWindow);

	for(int i = 0; i < SPRITE_COUNT; ++i)
	{
		pSpriteArray_[i] = new cGameSprite;
		pSpriteArray_[i]->Setup(fileName[i]);
		pSpriteArray_[i]->SetPosition({
			(rectWindow.right - rectWindow.left) * 0.5f, (rectWindow.bottom - rectWindow.top) * 0.5f});
	}
}

void SceneMainMenu::Update(float fDelta)
{
	int i = 0;
	for (; i < BUTTON_COUNT; ++i)
	{
		if (PtInRect(&(rectButtonArea_[i]), g_ptMouse))
		{
			renderIndex_ = i + 1;

			if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
				onButtonClick(i);

			break;
		}
	}

	if (i == BUTTON_COUNT)
		renderIndex_ = 0;
}

void SceneMainMenu::Render()
{
	pSpriteArray_[renderIndex_]->Render();

	std::string strTemp = std::to_string(g_ptMouse.x) + ", " + std::to_string(g_ptMouse.y);

	HDC hdc = GetDC(g_hWnd);
	TextOut(hdc, 20, 20, strTemp.c_str(), strTemp.size());
}

void SceneMainMenu::Delete()
{
	for (int i = 0; i < SPRITE_COUNT; ++i)
		SAFE_DELETE(pSpriteArray_[i]);
}
