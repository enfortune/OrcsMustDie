#include "stdafx.h"
#include "cTestScene.h"
#include "cGameSprite.h"

#define SCREEN_WIDTH GetRectWidth(GetScreenRect())
#define SCREEN_HEIGHT GetRectHeight(GetScreenRect())

cTestScene::cTestScene()
	: m_spriteGirl(NULL)
{
}


cTestScene::~cTestScene()
{
	this->Destroy();
}

void cTestScene::Setup()
{
	cGameScene::Setup();

	/*GetRectWidth

	cGameScene::GetScreenRect()*/

	

	m_spriteGirl = new cGameSprite;
	m_spriteGirl->Setup("UITest/±èÅÂÈñ.jpg");
	m_spriteGirl->SetPosition(D3DXVECTOR2(
		SCREEN_WIDTH / 2.f,
		SCREEN_HEIGHT / 2.f));

}
void cTestScene::Update(float fDelta)
{
	
	cGameScene::Update(fDelta);
}
void cTestScene::Render()
{
	m_spriteGirl->Render();

	cGameScene::Render();
}
void cTestScene::Destroy()
{
	SAFE_DELETE(m_spriteGirl);

	cGameScene::Destroy();
}

void cTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}