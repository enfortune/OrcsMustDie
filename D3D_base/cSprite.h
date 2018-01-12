#pragma once
class cSprite
{
	LPD3DXSPRITE				m_pSprite;
	LPDIRECT3DTEXTURE9			m_pTexture;
	D3DXIMAGE_INFO				m_stImageInfo;

	/* transform Data */
	D3DXVECTOR3					m_vPos;
	D3DXVECTOR3					m_vScale;
	float						m_fRotZ;

public:
	cSprite();
	~cSprite();

	void Setup(std::string sFileName);
	void Render(DWORD dwFlags = D3DXSPRITE_ALPHABLEND);

	D3DXMATRIXA16 GetTransformMatrix();
};

