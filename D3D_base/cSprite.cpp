#include "stdafx.h"
#include "cSprite.h"


cSprite::cSprite()
	: m_pSprite(NULL)
	, m_pTexture(NULL)
{
}


cSprite::~cSprite()
{
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTexture);
}

void cSprite::Setup(std::string sFileName)
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	/*
	m_pTexture = g_pTextureManager->GetTexture("UI/±èÅÂÈñ.jpg");
	*/
	ZeroMemory(&m_stImageInfo, sizeof(D3DXIMAGE_INFO));

	D3DXCreateTextureFromFileEx(
		g_pD3DDevice,
		sFileName.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_stImageInfo,
		NULL,
		&m_pTexture);
}
void cSprite::Render(DWORD dwFlags)
{
	m_pSprite->SetTransform(&this->GetTransformMatrix());

	m_pSprite->Begin(dwFlags);

	m_pSprite->Draw(m_pTexture, 
		NULL,
		&D3DXVECTOR3(m_stImageInfo.Width / 2.0f, m_stImageInfo.Height / 2.0f, 0),
		&D3DXVECTOR3(0, 0, 0.0f),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

D3DXMATRIXA16 cSprite::GetTransformMatrix()
{
	D3DXMATRIXA16 matTransform, matS, matR, matT;
	D3DXMatrixIdentity(&matTransform);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationZ(&matR, m_fRotZ);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	matTransform = matS * matR * matT;
	return matTransform;
}