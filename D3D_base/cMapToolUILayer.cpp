#include "stdafx.h"
#include "cMapToolUILayer.h"
#include "cMapMaker.h"
#include "cMapData.h"
#include "cGameUIButton.h"

cMapToolUILayer::cMapToolUILayer()
	: m_pMaker(nullptr)
{
	ZeroMemory(&m_pBlockChangeButton[0], sizeof(LONG) * GRIDBOXKIND_END);
}

cMapToolUILayer::~cMapToolUILayer()
{
	this->Delete();
}

void cMapToolUILayer::Setup()
{
	cGameUILayer::Setup();

	RECT rcWinSize;
	GetClientRect(g_hWnd, &rcWinSize);

	for (int i = 0; i < static_cast<int>(GRIDBOXKIND_END); i++)
	{
		m_pBlockChangeButton[i] = new cGameUIButton;
		switch (static_cast<EN_GRIDBOXKIND>(i))
		{
			case GRIDBOXKIND_NONE:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKIND_NONE", this, "Resource/Image/MapTool/ui_none.jpg");
			break;
			case GRIDBOXKIND_DEFAULT:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKIND_DEFAULT", this, "Resource/Image/MapTool/ui_default.jpg");
			break;
			case GRIDBOXKINE_BLOCK1:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_BLOCK1", this, "Resource/Image/MapTool/ui_block1.jpg");
			break;
			case GRIDBOXKINE_BLOCK2:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_BLOCK2", this, "Resource/Image/MapTool/ui_block2.jpg");
			break;
			case GRIDBOXKINE_GRASS:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_GRASS", this, "Resource/Image/MapTool/ui_grass.jpg");
			break;
			case GRIDBOXKINE_MARBLE:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_MARBLE", this, "Resource/Image/MapTool/ui_marble.jpg");
			break;
			case GRIDBOXKINE_ROCK:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_ROCK", this, "Resource/Image/MapTool/ui_rock.jpg");
			break;
			case GRIDBOXKINE_SOIL:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_SOIL", this, "Resource/Image/MapTool/ui_soil.jpg");
			break;
			case GRIDBOXKINE_WOODEN:
				m_pBlockChangeButton[i]->Setup("GRIDBOXKINE_WOODEN", this, "Resource/Image/MapTool/ui_wooden.jpg");
			break;
		}
		m_pBlockChangeButton[i]->SetPosition(D3DXVECTOR2(rcWinSize.right - 150 + 75 * (i % 2), rcWinSize.top + 100 + 75 * (i / 2)));
		this->AddChild(m_pBlockChangeButton[i]);
	}
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
	for (int i = 0; i < static_cast<int>(GRIDBOXKIND_END); i++)
	{
		SAFE_RELEASE(m_pBlockChangeButton[i]);
	}
}

void cMapToolUILayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cGameUILayer::WndProc(hWnd, message, wParam, lParam); 
}

/* Delegate */

bool cMapToolUILayer::UIButtonAction(std::string sTag, BUTTON_STATE enState)
{
	if (enState == BUTTON_STATE::DOWN)
	{
		if (m_pMaker)
		{
			if (sTag == "GRIDBOXKIND_NONE")
			{
				m_pMaker->SetBoxKind(GRIDBOXKIND_NONE);
			}
			if (sTag == "GRIDBOXKIND_DEFAULT")
			{
				m_pMaker->SetBoxKind(GRIDBOXKIND_DEFAULT);
			}
			if (sTag == "GRIDBOXKINE_BLOCK1")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_BLOCK1);
			}
			if (sTag == "GRIDBOXKINE_BLOCK2")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_BLOCK2);
			}
			if (sTag == "GRIDBOXKINE_GRASS")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_GRASS);
			}
			if (sTag == "GRIDBOXKINE_MARBLE")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_MARBLE);
			}
			if (sTag == "GRIDBOXKINE_ROCK")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_ROCK);
			}
			if (sTag == "GRIDBOXKINE_SOIL")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_SOIL);
			}
			if (sTag == "GRIDBOXKINE_WOODEN")
			{
				m_pMaker->SetBoxKind(GRIDBOXKINE_WOODEN);
			}
		}
	}

	return true;
}