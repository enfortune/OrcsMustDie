#include "stdafx.h"
#include "cMainGame.h"

#include "cTestScene.h"
#include "cInGameScene.h"
#include "cMapToolScene.h"
#include "cParticleTestScene.h"
#include "cPhysicsTestScene.h"

#include "TrapTestScene.h"
#include "SceneMainMenu.h"

cMainGame::cMainGame()
{
}

     
cMainGame::~cMainGame()
{
	g_pTextureManager->Destroy();
	g_pFontManager->Destroy();
	g_pSceneManager->Destroy();
	g_pSoundManager->Destroy();
	g_pParticleManager->Destroy();
	g_pXFileManager->Destroy();

	g_pDeviceManager->Destroy(); // 마지막에 디스트로이할 것
}

void cMainGame::Setup()
{
	g_pSoundManager->Setup();
	g_pKeyManager->Setup();

	g_pSceneManager->Setup(new SceneMainMenu);
}

void cMainGame::Update()
{
	g_pSoundManager->Update();
	g_pKeyManager->Update();
	float fTime = g_pTimeManager->GetEllapsedTime();

	g_pSceneManager->PreUpdate();
	g_pSceneManager->Update(fTime);

	g_pParticleManager->Update(fTime);
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);

	g_pD3DDevice->BeginScene();
	/////////////////////////////////

	g_pSceneManager->Render();
	g_pParticleManager->Render();

	/////////////////////////////////

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_pSceneManager->WndProc(hWnd, message, wParam, lParam);
}


