#include "stdafx.h"
#include "TrapTestScene.h"

#include "cSkinnedMesh.h"

TrapTestScene::TrapTestScene()
{
}

TrapTestScene::~TrapTestScene()
{
}

void TrapTestScene::Setup()
{
	cGameScene::Setup();

	trapTypeManager_.init();

	camera_.Setup();
	grid_.Setup();

	D3DXMATRIXA16 matrixIdentity {};
	D3DXMatrixIdentity(&matrixIdentity);

	trapList_.resize(16);
	trapList_[0].init(*(trapTypeManager_.find("Barricade")), matrixIdentity);

	int i = 0;
}

void TrapTestScene::Update(float fDelta)
{
	camera_.Update();

	trapList_[0].render();

	/*for (int i = 0; i < static_cast<int> (trapList_.size()); ++i)
		trapList_[i].update();*/

	cGameScene::Update(fDelta);
}

void TrapTestScene::Render()
{
	grid_.Render();

	trapList_[0].render();

	/*for (int i = 0; i < static_cast<int> (trapList_.size()); ++i)
		trapList_[i].render();*/

	cGameScene::Render();
}

void TrapTestScene::Delete()
{
	cGameScene::Delete();
}

void TrapTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	camera_.WndProc(hWnd, message, wParam, lParam);

	cGameScene::WndProc(hWnd, message, wParam, lParam);
}