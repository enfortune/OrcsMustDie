#include "stdafx.h"
#include "cGameUILabel.h"
#include "cGameText.h"

cGameUILabel::cGameUILabel()
	: m_pText(NULL)
	, m_vPos(0.f, 0.f)
{
}


cGameUILabel::~cGameUILabel()
{
	this->Delete();
}

void cGameUILabel::SetText(std::string sText)
{
	m_pText->SetText(sText);
}

void cGameUILabel::Setup(cFontManager::eFontType enFontType, std::string sText)
{
	cGameNode::Setup(false);

	m_pText = new cGameText;
	m_pText->Setup(enFontType);
	m_pText->SetText(sText);
}
void cGameUILabel::Render() 
{
	m_pText->Render(&this->GetMatrixToWorld());

	cGameNode::Render();
}
void cGameUILabel::Delete() 
{
	SAFE_DELETE(m_pText);
}

D3DXMATRIXA16 cGameUILabel::GetMatirixToParent()
{
	D3DXMATRIXA16 matRet, matT;
	D3DXMatrixIdentity(&matRet);

	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, 0.0f);

	matRet = matT;

	return matRet;
}