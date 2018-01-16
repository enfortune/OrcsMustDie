#include "stdafx.h"
#include "cGameUIButton.h"
#include "iGameUIDelegate.h"
#include "cGameSprite.h"

#define GAMEUIBUTTONSCALE_POPUP		1.01f
#define GAMEUIBUTTONSCALE_PUSH		0.99f
#define GAMEUIBUTTONSCALE_DELTA		0.4f // per sec
#define GAMEUIBUTTONSCALE_EPSILON	0.001f

cGameUIButton::cGameUIButton()
	: m_pSprite(nullptr)
	, m_pSpriteOnMouse(nullptr)
	, m_pSpritePush(nullptr)
	, m_pDelegate(nullptr)
	, m_bEnable(true)
	, m_enState(BUTTON_STATE::IDLE)
	, m_vPos(D3DXVECTOR2(0.0f, 0.0f))
	, m_fRotAngle(0.0f)
	, m_fButtonScale(1.0f)
	, m_enSpriteState(BUTTON_SPRITESTATE::IDLE)
	, m_vScale(D3DXVECTOR2(1.0f, 1.0f))
{
}


cGameUIButton::~cGameUIButton()
{
	this->Delete();
}

void cGameUIButton::Setup(std::string sTag, iGameUIDelegate* pDelegate, std::string sSpriteFileName, std::string sSpriteOnMouseFileName, std::string sSpritePushFileName)
{
	cGameNode::Setup(false);
	m_pSprite = new cGameSprite; 
	m_pSprite->Setup(sSpriteFileName);
	if (sSpriteOnMouseFileName != "")
	{
		m_pSpriteOnMouse = new cGameSprite;
		m_pSpriteOnMouse->Setup(sSpriteOnMouseFileName);
	}
	if (sSpritePushFileName != "")
	{
		m_pSpritePush = new cGameSprite;
		m_pSpritePush->Setup(sSpritePushFileName);
	}
	m_pDelegate = pDelegate;
	m_sTag = sTag;
}
void cGameUIButton::Update(float fDelta)
{
	/* sprite scale update */
	switch (m_enSpriteState)
	{
		case BUTTON_SPRITESTATE::IDLE:
			if (m_fButtonScale > 1.0f) 
				m_fButtonScale -= GAMEUIBUTTONSCALE_DELTA * fDelta;
			else if (m_fButtonScale <1.0f) 
				m_fButtonScale += GAMEUIBUTTONSCALE_DELTA * fDelta;
			if (fabs(m_fButtonScale - 1.0f) < GAMEUIBUTTONSCALE_EPSILON)
				m_fButtonScale = 1.0f;
		break;
		case BUTTON_SPRITESTATE::ONMOUSE:
			if (m_fButtonScale > GAMEUIBUTTONSCALE_POPUP)
				m_fButtonScale -= GAMEUIBUTTONSCALE_DELTA * fDelta;
			else if (m_fButtonScale <GAMEUIBUTTONSCALE_POPUP)
				m_fButtonScale += GAMEUIBUTTONSCALE_DELTA * fDelta;
			if (fabs(m_fButtonScale - GAMEUIBUTTONSCALE_POPUP) < GAMEUIBUTTONSCALE_EPSILON)
				m_fButtonScale = GAMEUIBUTTONSCALE_POPUP;
		break;
		case BUTTON_SPRITESTATE::PUSH:
			if (m_fButtonScale > GAMEUIBUTTONSCALE_PUSH)
				m_fButtonScale -= GAMEUIBUTTONSCALE_DELTA * fDelta;
			else if (m_fButtonScale <GAMEUIBUTTONSCALE_PUSH)
				m_fButtonScale += GAMEUIBUTTONSCALE_DELTA * fDelta;
			if (fabs(m_fButtonScale - GAMEUIBUTTONSCALE_PUSH) < GAMEUIBUTTONSCALE_EPSILON)
				m_fButtonScale = GAMEUIBUTTONSCALE_PUSH;
		break;

		case BUTTON_SPRITESTATE::END:
		break;
		default:
		break;
	}

	if (!m_bEnable) return; //사용중이 아니라면 리턴

	m_pDelegate->UIButtonAction(m_sTag, m_enState); // 현재 상태에 대한 버튼액션 실행 후
	

	switch (m_enState) //다운과 업 함수가 한번만 실행될 수 있도록 상태를 정리한다
	{
		case BUTTON_STATE::DOWN:
			m_enState = BUTTON_STATE::HOLD;
		break;
		case BUTTON_STATE::UP:
			m_enState = BUTTON_STATE::IDLE;
		break;
		default:
		break;
	}

	cGameNode::Update(fDelta);
}
void cGameUIButton::Render()
{
	if (!m_bEnable)
	{
		m_pSprite->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
		m_pSprite->Render(&this->GetMatrixToWorld());
		return;
	}


	switch (m_enSpriteState)
	{
		case BUTTON_SPRITESTATE::IDLE:
			if (!m_pSpriteOnMouse) m_pSprite->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
			else m_pSprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pSprite->Render(&this->GetMatrixToWorld());
		break;
		case BUTTON_SPRITESTATE::ONMOUSE:
			if (m_pSpriteOnMouse) m_pSpriteOnMouse->Render(&this->GetMatrixToWorld());
			else
			{
				m_pSprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pSprite->Render(&this->GetMatrixToWorld());
			}
		break;
		case BUTTON_SPRITESTATE::PUSH:
			if (m_pSpritePush) m_pSpritePush->Render(&this->GetMatrixToWorld());
			else
			{
				if (m_pSpriteOnMouse) m_pSpriteOnMouse->Render(&this->GetMatrixToWorld());
				else
				{
					m_pSprite->SetColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
					m_pSprite->Render(&this->GetMatrixToWorld());
				}
			}
		break;
		default:
			m_pSprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pSprite->Render(&this->GetMatrixToWorld());
		break;
	}

	cGameNode::Render();
}
void cGameUIButton::Delete()
{
	SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pSpriteOnMouse);
	SAFE_DELETE(m_pSpritePush);
}
void cGameUIButton::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_pSprite) return;
	if (!m_bEnable) return;
	
	switch (message)
	{
		case WM_MOUSEMOVE:
			if (CheckPolygonContainVec2(m_pSprite->GetBoundingBox(&this->GetMatrixToWorld()),
				D3DXVECTOR2(g_ptMouse.x, g_ptMouse.y)))
			{
				switch (m_enState)
				{
					case BUTTON_STATE::DOWN: case BUTTON_STATE::HOLD: 
						//m_enSpriteState = BUTTON_SPRITESTATE::PUSH;
					break;
					default:
						m_enSpriteState = BUTTON_SPRITESTATE::ONMOUSE;
					break;
				}

			}
			else
			{
				m_enState = BUTTON_STATE::IDLE;
				m_enSpriteState = BUTTON_SPRITESTATE::IDLE;

			}
		break;
		case WM_LBUTTONDOWN:
			if (CheckPolygonContainVec2(m_pSprite->GetBoundingBox(&this->GetMatrixToWorld()),
				D3DXVECTOR2(g_ptMouse.x, g_ptMouse.y)))
			{
				m_enSpriteState = BUTTON_SPRITESTATE::PUSH;
				m_enState = BUTTON_STATE::DOWN;
			}
		break;
		case WM_LBUTTONUP:
			if (CheckPolygonContainVec2(m_pSprite->GetBoundingBox(&this->GetMatrixToWorld()),
				D3DXVECTOR2(g_ptMouse.x, g_ptMouse.y)))
			{
				m_enSpriteState = BUTTON_SPRITESTATE::ONMOUSE;
				switch (m_enState)
				{
					case BUTTON_STATE::DOWN: case BUTTON_STATE::HOLD:
						m_enState = BUTTON_STATE::UP;
					break;
					default:
						m_enSpriteState = BUTTON_SPRITESTATE::ONMOUSE;
					break;
				}
			}
		break;
		default:
		break;
	}
	cGameNode::WndProc(hWnd, message, wParam, lParam);
}

D3DXMATRIXA16 cGameUIButton::GetMatirixToParent()
{
	D3DXMATRIXA16 matRet, matS, matR, matT;
	D3DXMatrixIdentity(&matRet);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, 0.0f);
	//D3DXMatrixRotationAxis(&matR, &m_vAxis, m_fRotAngle); // use Axis Z
	D3DXMatrixRotationZ(&matR, m_fRotAngle);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, 0.0f);

	matRet = matS * matR * matT;

	return matRet;
}