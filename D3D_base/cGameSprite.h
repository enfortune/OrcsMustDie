#pragma once

class cGameSprite /* RC 방식으로 관리하려면 cGameObject 상속받아서 사용*/
{
	LPD3DXSPRITE				m_pSprite;
	LPDIRECT3DTEXTURE9			m_pTexture;
	D3DXIMAGE_INFO				m_stImageInfo;
	SYNTHESIZE_REF(DWORD, m_dwSpriteFlags, SpriteFlag);
	SYNTHESIZE_REF(D3DXCOLOR, m_stColor, Color); // 0.f~ 1.f

	/* basic data for matrix */
protected:
	std::vector<D3DXVECTOR2> m_vecContentBox;

	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
	//SYNTHESIZE_REF(D3DXVECTOR3, m_vAxis, Axis); // use Axis(0,0,1)
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle); // Z angle
	SYNTHESIZE_REF(D3DXVECTOR2, m_vScale, Scale);

public:
	cGameSprite();
	~cGameSprite();

	void Setup(std::string sFileName);
	void Render();
	void Destroy();

	std::vector<D3DXVECTOR2> GetBoundingBox();
	D3DXMATRIXA16 GetTransformMatrix();

	D3DXIMAGE_INFO GetImageInfo() { return m_stImageInfo; }
};

