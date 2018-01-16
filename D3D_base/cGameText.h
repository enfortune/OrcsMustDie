#pragma once
class cGameText
{
	LPD3DXFONT m_pFont;
	D3DXFONT_DESCA m_stDesc;
	cFontManager::eFontType m_enFontType;

	std::string	m_sText;
	SYNTHESIZE_REF(DWORD, m_dwFontFlags, FontFlag);
	SYNTHESIZE_REF(D3DXCOLOR, m_stColor, Color); // 0.f~ 1.f
	
protected:
	RECT m_rcOrigin;
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);

public:
	cGameText();
	~cGameText();

	void Setup(cFontManager::eFontType enFontType);
	void SetText(std::string sText);
	void Render(D3DXMATRIXA16* matParent = nullptr);
	void Delete();
};

