#include "stdafx.h"
#include "cMainGame.h"

#include "cTestScene.h"

cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	g_pSceneManager->Setup(new cTestScene);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	g_pSceneManager->Update();
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

	/////////////////////////////////

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_pSceneManager->WndProc(hWnd, message, wParam, lParam);
}


