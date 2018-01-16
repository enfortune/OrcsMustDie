#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e, OUT LPD3DXFONT_DESCA lpDesc)
{
	if (m_mapFont.find(e) != m_mapFont.end())
	{
		if (lpDesc) (*lpDesc) = m_mapFontDesc[e]; // 원본을 수정할 수 없도록 복사본을 넘긴다.
		return m_mapFont[e];
	}

	D3DXFONT_DESC stFD;
	ZeroMemory(&stFD, sizeof(D3DXFONT_DESC));
	RECT	rcWnd;
	GetClientRect(g_hWnd, &rcWnd);

	float fScaleX = static_cast<float>(GetRectWidth(rcWnd)) / 1920.f;
	float fScaleY = static_cast<float>(GetRectHeight(rcWnd)) / 1080.f;

	switch (e)
	{
		case FT_DEFAULT:
		break;
		case FT_INGAMEUI_SCORE:
			stFD.Height =	static_cast<INT>(static_cast<float>(30) * fScaleX);
			stFD.Width =	static_cast<INT>(static_cast<float>(18) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			AddFontResource("font/umberto.ttf");
			strcpy_s(stFD.FaceName, "umberto");
		break;
		case FT_INGAMEUI_TIMER:
			stFD.Height = static_cast<INT>(static_cast<float>(50) * fScaleX);
			stFD.Width = static_cast<INT>(static_cast<float>(25) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			strcpy_s(stFD.FaceName, "굴림체");
		break;
		case FT_INGAMEUI_WAVE:
			stFD.Height = static_cast<INT>(static_cast<float>(50) * fScaleX);
			stFD.Width = static_cast<INT>(static_cast<float>(25) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			strcpy_s(stFD.FaceName, "굴림체");
		break;
		case FT_INGAMEUI_STAGE:
			stFD.Height = static_cast<INT>(static_cast<float>(50) * fScaleX);
			stFD.Width = static_cast<INT>(static_cast<float>(25) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			strcpy_s(stFD.FaceName, "굴림체");
		break;
		case FT_INGAMEUI_NOTICE:
			stFD.Height = static_cast<INT>(static_cast<float>(50) * fScaleX);
			stFD.Width = static_cast<INT>(static_cast<float>(25) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			strcpy_s(stFD.FaceName, "굴림체");
		break;
		case FT_INGAMEUI_SLOT:
			stFD.Height = static_cast<INT>(static_cast<float>(50) * fScaleX);
			stFD.Width = static_cast<INT>(static_cast<float>(25) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			strcpy_s(stFD.FaceName, "굴림체");
		break;
		case FT_INGAMEUI_COST:
			stFD.Height = static_cast<INT>(static_cast<float>(50) * fScaleX);
			stFD.Width = static_cast<INT>(static_cast<float>(25) * fScaleY);
			stFD.Weight = FW_MEDIUM;
			stFD.Italic = false;
			stFD.CharSet = DEFAULT_CHARSET;
			stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			stFD.PitchAndFamily = FF_DONTCARE;
			strcpy_s(stFD.FaceName, "굴림체");
		break;
		default:
		break;
	}

	D3DXCreateFontIndirect(g_pD3DDevice, &stFD, &m_mapFont[e]);
	m_mapFontDesc[e] = stFD;
	if (lpDesc) (*lpDesc) = m_mapFontDesc[e];
	return m_mapFont[e];
}

void cFontManager::Destroy()
{
	for each(auto p in m_mapFont)
	{
		SAFE_RELEASE(p.second);
	}
	m_mapFont.clear();
}
