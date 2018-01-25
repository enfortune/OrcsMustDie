#include "stdafx.h"
#include "cParticleTestScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cGameParticleSnow.h"
#include "cGameParticleFirework.h"


cParticleTestScene::cParticleTestScene()
	: m_pSnow(nullptr)
	, m_pFirework(nullptr)
	, m_bIsCulling(false)
{
}


cParticleTestScene::~cParticleTestScene()
{
	this->Delete();
}

void cParticleTestScene::Setup()
{
	cGameScene::Setup();
	this->SetupParticle();

	/*m_pSnow = new cGameParticleSnow(
		D3DXVECTOR3(-10.f, 0.f, -10.f),
		D3DXVECTOR3(10.f, 10.f, 10.f),
		1024);
	m_pSnow->Setup("");

	m_pFirework = new cGameParticleFirework(
		D3DXVECTOR3(0.f, 5.f, 50.f),
		1024);
	m_pFirework->Setup("");*/

	m_stFrustum.vNear_00 = D3DXVECTOR3(-1.f, -1.f, -1.f);
	m_stFrustum.vNear_01 = D3DXVECTOR3(-1.f, 1.f, -1.f);
	m_stFrustum.vNear_10 = D3DXVECTOR3(1.f, -1.f, -1.f);
	m_stFrustum.vNear_11 = D3DXVECTOR3(1.f, 1.f, -1.f);
	m_stFrustum.vFar_00 = D3DXVECTOR3(-10.f, -10.f, 10.f);
	m_stFrustum.vFar_01 = D3DXVECTOR3(-10.f, 10.f, 10.f);
	m_stFrustum.vFar_10 = D3DXVECTOR3(10.f, -10.f, 10.f);
	m_stFrustum.vFar_11 = D3DXVECTOR3(10.f, 10.f, 10.f);

	m_pCamera = new cCamera;
	m_pCamera->Setup(nullptr);
	m_pGrid = new cGrid;
	m_pGrid->Setup();
}
void cParticleTestScene::Update(float fDelta)
{
	m_pCamera->Update();
	if (m_pSnow) m_pSnow->Update(fDelta);
	if (m_pFirework) m_pFirework->Update(fDelta);

	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_bIsCulling = !m_bIsCulling;
	}

	this->UpdateParticle(fDelta);
	cGameScene::Update(fDelta);
}
void cParticleTestScene::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGrid->Render();
	if (m_pSnow)m_pSnow->Render();
	if (m_pFirework) m_pFirework->Render();

	this->RenderParticle();
	cGameScene::Render();
}
void cParticleTestScene::Delete()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pSnow);
	SAFE_DELETE(m_pFirework);
}

void cParticleTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pCamera->WndProc(hWnd, message, wParam, lParam);
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}

void cParticleTestScene::SetupParticle()
{
	m_vecParticle.resize(5000);
	for (size_t i = 0; i < m_vecParticle.size(); i++)
	{
		float fRadius = rand() % 100 / 10.0f;
		m_vecParticle[i].p = D3DXVECTOR3(0, 0, fRadius);

		D3DXVECTOR3 vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX matRX, matRY, matRZ, matWorld;
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ;

		D3DXVec3TransformCoord(&m_vecParticle[i].p,
			&m_vecParticle[i].p,
			&matWorld);
		m_vecParticle[i].c = D3DCOLOR_ARGB(255, 180, 50, 20);
	}

	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true); 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f)); //용책 322 체크
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.2f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(10.0f));

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}
void cParticleTestScene::UpdateParticle(float fDelta)
{
}
void cParticleTestScene::RenderParticle()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	//g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture("batman.png"));
	
	ST_SPHERE stSphere;
	stSphere.fRadius = 0.1f;
	if (m_bIsCulling)
	{
		for (size_t i = 0; i < m_vecParticle.size(); i++)
		{
			stSphere.vCenter = m_vecParticle[i].p;
			if (CheckFrustumIntersectSphere(&m_stFrustum, &stSphere))
			{
				g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
					1,
					&m_vecParticle[i],
					sizeof(ST_PC_VERTEX));
			}
		}
	}
	else
	{
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
			m_vecParticle.size(),
			&m_vecParticle[0],
			sizeof(ST_PC_VERTEX));
	}
	

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}