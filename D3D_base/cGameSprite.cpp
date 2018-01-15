#include "stdafx.h"
#include "cGameSprite.h"


cGameSprite::cGameSprite()
	: m_pSprite(NULL)
	, m_pTexture(NULL)
	, m_stColor(1.0f, 1.0f, 1.0f, 1.0f)
	, m_dwSpriteFlags(D3DXSPRITE_ALPHABLEND)
	, m_vPos(0.0f, 0.0f)
	, m_fRotAngle(0.0f)
	, m_vScale(1.0f, 1.0f)
{
}

cGameSprite::~cGameSprite()
{
	this->Destroy();
}

void cGameSprite::Setup(std::string sFileName)
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	/*ZeroMemory(&m_stImageInfo, sizeof(D3DXIMAGE_INFO));

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
		&m_pTexture);*/

	m_pTexture = g_pTextureManager->GetTexture(sFileName, &m_stImageInfo);
	m_pTexture->AddRef();

	m_vecContentBox.resize(4);

	m_vecContentBox[0] = D3DXVECTOR2(-static_cast<float>(m_stImageInfo.Width) / 2.f, -static_cast<float>(m_stImageInfo.Height) / 2.f); // LT
	m_vecContentBox[1] = D3DXVECTOR2(static_cast<float>(m_stImageInfo.Width) / 2.f, -static_cast<float>(m_stImageInfo.Height) / 2.f); // RT
	m_vecContentBox[2] = D3DXVECTOR2(static_cast<float>(m_stImageInfo.Width) / 2.f, static_cast<float>(m_stImageInfo.Height) / 2.f); // RB
	m_vecContentBox[3] = D3DXVECTOR2(-static_cast<float>(m_stImageInfo.Width) / 2.f, static_cast<float>(m_stImageInfo.Height) / 2.f); // LB
}

void cGameSprite::Render()
{
	m_pSprite->SetTransform(&this->GetTransformMatrix());

	m_pSprite->Begin(m_dwSpriteFlags);

	m_pSprite->Draw(m_pTexture, 
		NULL,
		&D3DXVECTOR3(m_stImageInfo.Width / 2.0f, m_stImageInfo.Height / 2.0f, 0),
		&D3DXVECTOR3(0, 0, 0.0f),
		D3DCOLOR_COLORVALUE(m_stColor.r, m_stColor.g, m_stColor.b, m_stColor.a)
		//m_stColor
		);

	m_pSprite->End();
}

void cGameSprite::Destroy()
{
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pTexture);
}

std::vector<D3DXVECTOR2> cGameSprite::GetBoundingBox()
{
	std::vector<D3DXVECTOR2> vecRet(4);
	D3DXMATRIXA16 matTrans = this->GetTransformMatrix();

	for (int i = 0; i < 4; i++)
	{
		D3DXVec2TransformCoord(&vecRet[i], &m_vecContentBox[i], &matTrans);
	}
	return vecRet;
}

D3DXMATRIXA16 cGameSprite::GetTransformMatrix()
{
	D3DXMATRIXA16 matTransform, matS, matR, matT;
	D3DXMatrixIdentity(&matTransform);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, 0.0f);
	//D3DXMatrixRotationAxis(&matR, &m_vAxis, m_fRotAngle); // use Axis Z
	D3DXMatrixRotationZ(&matR, m_fRotAngle);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, 0.0f);

	matTransform = matS * matR * matT;
	return matTransform;
}