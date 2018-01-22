#include "stdafx.h"
#include "cMapToolUILayer.h"



cMapToolUILayer::cMapToolUILayer()
{
}


cMapToolUILayer::~cMapToolUILayer()
{
	this->Delete();
}

void cMapToolUILayer::Setup()
{
	cGameUILayer::Setup();
}
void cMapToolUILayer::Update(float fDelta)
{
	cGameUILayer::Update(fDelta);
}
void cMapToolUILayer::Render()
{
	cGameUILayer::Render();
}
void cMapToolUILayer::Delete()
{
	
}

void cMapToolUILayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameUILayer::WndProc(hWnd, message, wParam, lParam); 
}

/* Delegate */

bool cMapToolUILayer::UIButtonAction(std::string sTag, BUTTON_STATE enState)
{
		

	return true;
}