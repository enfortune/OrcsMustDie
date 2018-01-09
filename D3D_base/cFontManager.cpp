#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

LPD3DXFONT cFontManager::GetFont(eFontType e)
{
	if (m_mapFont.find(e) != m_mapFont.end())
	{
		return m_mapFont[e];
	}

	D3DXFONT_DESC stFD;
	ZeroMemory(&stFD, sizeof(D3DXFONT_DESC));

	if (e == FT_DEFAULT)
	{
		stFD.Height = 50;
		stFD.Width = 25;
		stFD.Weight = FW_MEDIUM;
		stFD.Italic = false;
		stFD.CharSet = DEFAULT_CHARSET;
		stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
		stFD.PitchAndFamily = FF_DONTCARE;

		//strcpy_s(stFD.FaceName, "±¼¸²Ã¼");

		AddFontResource("font/umberto.ttf");
		strcpy_s(stFD.FaceName, "umberto");
	}
	else if (e == FT_UI)
	{
		stFD.Height = 50;
		stFD.Width = 25;
		stFD.Weight = FW_MEDIUM;
		stFD.Italic = false;
		stFD.CharSet = DEFAULT_CHARSET;
		stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
		stFD.PitchAndFamily = FF_DONTCARE;

		//strcpy_s(stFD.FaceName, "±¼¸²Ã¼");

		AddFontResource("font/umberto.ttf");
		strcpy_s(stFD.FaceName, "umberto");
	}
	else if (e == FT_QUEST)
	{
		stFD.Height = 50;
		stFD.Width = 25;
		stFD.Weight = FW_MEDIUM;
		stFD.Italic = false;
		stFD.CharSet = DEFAULT_CHARSET;
		stFD.OutputPrecision = OUT_DEFAULT_PRECIS;
		stFD.PitchAndFamily = FF_DONTCARE;

		strcpy_s(stFD.FaceName, "±¼¸²Ã¼");
	}

	D3DXCreateFontIndirect(g_pD3DDevice, &stFD, &m_mapFont[e]);

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
