#pragma once

class cGameSprite
{
	RECT						m_pCropRect;

	LPD3DXSPRITE				m_pSprite;
	LPDIRECT3DTEXTURE9			m_pTexture;
	D3DXIMAGE_INFO				m_stImageInfo;
	SYNTHESIZE_REF(DWORD, m_dwSpriteFlags, SpriteFlag);
	SYNTHESIZE_REF(D3DXCOLOR, m_stColor, Color); // 0.f~ 1.f
	SYNTHESIZE_REF(float, m_fZorder, Zorder);
	

	/* basic data for matrix */
protected:
	std::vector<D3DXVECTOR2> m_vecContentBox;

	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle); // Z angle
	SYNTHESIZE_REF(D3DXVECTOR2, m_vScale, Scale);
public:
	cGameSprite();
	~cGameSprite();

	void Setup(std::string sFileName);
	void Render(D3DXMATRIXA16* matParent = nullptr);
	void Delete();


	void SetCropRect(RECT* lpRect);
	void SetCropRect(float fRatioL, float fRatioT, float fRatioR, float fRatioB);


	std::vector<D3DXVECTOR2> GetBoundingBox(D3DXMATRIXA16* matParent = nullptr);
	D3DXMATRIXA16 GetTransformMatrix(D3DXMATRIXA16* matParent = nullptr);

	D3DXIMAGE_INFO GetImageInfo() { return m_stImageInfo; }
};

