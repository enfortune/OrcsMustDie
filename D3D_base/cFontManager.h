#pragma once
#define g_pFontManager cFontManager::GetInstance()

class cFontManager
{

public:
	enum eFontType
	{
		FT_DEFAULT,
		FT_INGAMEUI_SCORE,
		FT_INGAMEUI_TIMER,
		FT_INGAMEUI_WAVE,
		FT_INGAMEUI_STAGE,
		FT_INGAMEUI_NOTICE,
		FT_INGAMEUI_SLOT,
		FT_INGAMEUI_COST
	};

private:
	SINGLETONE(cFontManager);
	std::map<eFontType, LPD3DXFONT>	m_mapFont;
	std::map<eFontType, D3DXFONT_DESCA>	m_mapFontDesc;
public:
	LPD3DXFONT GetFont(eFontType e, OUT LPD3DXFONT_DESCA lpDesc = nullptr);
	void Destroy();
};

