#include "stdafx.h"
#include "cMapMaker.h"

#include "cMapData.h"
#include "cMapToolGrid.h"
#include "cRay.h"

cMapMaker::cMapMaker()
	: m_pMapData(nullptr)
	, m_pGrid(nullptr)
	, m_bIsMakingEnable(true)
	, m_bIsOnMapLayer(false)
	, m_rcActiveArea({0, 0, 0, 0})
	, m_enCurrBoxKind(GRIDBOXKIND_NONE)
	, m_nCurrX(0)
	, m_nCurrY(0)
	, m_nCurrZ(0)
{
}

cMapMaker::~cMapMaker()
{
	this->Delete();
}

void cMapMaker::Setup(cMapData* pMapData)
{
	cGameNode::Setup(false);

	m_pMapData = pMapData;
	m_enCurrBoxKind = GRIDBOXKIND_DEFAULT;

	m_pGrid = new cMapToolGrid;
	if (pMapData) m_pGrid->Setup(pMapData->GetSizeZ(), static_cast<float>(pMapData->GetCubeSize()));
	else m_pGrid->Setup();

	RECT rcWinSize;
	GetClientRect(g_hWnd, &rcWinSize);
	m_rcActiveArea = { rcWinSize.left + 100, rcWinSize.top + 100, rcWinSize.right - 100, rcWinSize.bottom - 100 };

	if (pMapData) m_nCurrY = pMapData->GetZeroStdY();
}
void cMapMaker::Update(float fDelta)
{ 
	if (g_pKeyManager->IsOnceKeyDown('R'))
	{
		this->LayerUp();
	}
	else if (g_pKeyManager->IsOnceKeyDown('F'))
	{
		this->LayerDown();
	}
	else if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		this->SaveData("SampleStageMap.map");
	}
	else if (g_pKeyManager->IsOnceKeyDown('2'))
	{
		this->LoadData("SampleStageMap.map");
	}


	cGameNode::Update(fDelta);
}
void cMapMaker::Render()
{
	if (m_pGrid) m_pGrid->Render();
	if (m_pMapData && m_bIsMakingEnable && m_bIsOnMapLayer && !g_pKeyManager->IsStayKeyDown(VK_CONTROL))
		m_pMapData->DrawVirtualBox(m_nCurrX, m_nCurrY, m_nCurrZ);
	
	cGameNode::Render();
}
void cMapMaker::Delete()
{
	SAFE_DELETE(m_pGrid);
}

void cMapMaker::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_MOUSEMOVE:
			if (CheckRectContainPoint(&m_rcActiveArea, &g_ptMouse))
			{
				if (m_pMapData)
				{
					m_bIsOnMapLayer = m_pMapData->GetPickingIndex(m_nCurrX, m_nCurrY, m_nCurrZ, cRay::RayAtWorldSpace(g_ptMouse.x, g_ptMouse.y));
				}
			}
		break;
		case WM_LBUTTONDOWN:
			if (m_bIsOnMapLayer && !(GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				if (m_pMapData) m_pMapData->MakeBox(m_nCurrX, m_nCurrY, m_nCurrZ, m_enCurrBoxKind);
			}
		break;
		case WM_RBUTTONDOWN:
			if (m_bIsOnMapLayer && !(GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				if (m_pMapData) m_pMapData->MakeBox(m_nCurrX, m_nCurrY, m_nCurrZ, GRIDBOXKIND_NONE);
			}
		break;
		default:
		break;
	}

	cGameNode::WndProc(hWnd, message, wParam, lParam);
}

void cMapMaker::SetMapData(cMapData* pMapData)
{
	m_pMapData = pMapData;
	if (pMapData) m_nCurrY = pMapData->GetZeroStdY();
}

void cMapMaker::LayerUp()
{
	if (m_pMapData)
		if (m_nCurrY < m_pMapData->GetSizeY() - 1)
		{
			m_nCurrY++;
			if (m_pGrid) m_pGrid->SetHeight((m_nCurrY - m_pMapData->GetZeroStdY()) * m_pMapData->GetCubeSize());
		}
}
void cMapMaker::LayerDown()
{
	if (m_pMapData)
		if (m_nCurrY > 0)
		{
			m_nCurrY--;
			if (m_pGrid) m_pGrid->SetHeight((m_nCurrY - m_pMapData->GetZeroStdY()) * m_pMapData->GetCubeSize());
		}	
}

void cMapMaker::SaveData(std::string sFileName)
{
	if (m_pMapData) m_pMapData->SaveData(sFileName);
}
void cMapMaker::LoadData(std::string sFileName)
{
	if (m_pMapData) m_pMapData->LoadData(sFileName);
}