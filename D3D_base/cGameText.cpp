#include "stdafx.h"
#include "cGameText.h"


cGameText::cGameText()
	: m_pFont(nullptr)
	, m_enFontType(cFontManager::FT_DEFAULT)
	, m_sText(std::string(""))
	, m_dwFontFlags(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP)
	, m_stColor(1.0f, 1.0f, 1.0f, 1.0f)
	, m_vPos(0.f, 0.f)
	, m_rcOrigin({0, 0, 0, 0})
{
}

cGameText::~cGameText()
{
	this->Delete();
}

void cGameText::Setup(cFontManager::eFontType enFontType)
{
	m_pFont = g_pFontManager->GetFont(enFontType, &m_stDesc);
	m_pFont->AddRef();
}
void cGameText::SetText(std::string sText)
{
	m_sText = sText;
	int nWidth = m_stDesc.Width * sText.length();
	int nHeight = m_stDesc.Height;
	m_rcOrigin = { -nWidth / 2, -nHeight / 2, nWidth / 2, nHeight / 2 };
}
void cGameText::Render(D3DXMATRIXA16* matParent)
{
	D3DXVECTOR2 vCenter;
	D3DXVec2TransformCoord(&vCenter, &m_vPos, matParent);
	RECT rcDraw = {
		m_rcOrigin.left + vCenter.x,
		m_rcOrigin.top + vCenter.y,
		m_rcOrigin.right + vCenter.x,
		m_rcOrigin.bottom + vCenter.y };

	
	m_pFont->DrawTextA(NULL, m_sText.c_str(),
		m_sText.length(),
		&rcDraw,
		m_dwFontFlags,
		D3DCOLOR_COLORVALUE(m_stColor.r, m_stColor.g, m_stColor.b, m_stColor.a)
		);
}
void cGameText::Delete()
{
	SAFE_RELEASE(m_pFont);
}