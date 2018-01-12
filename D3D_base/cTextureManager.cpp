#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
	this->Destroy();
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(std::string sFullPath)
{
	if (m_mapTexture.find(sFullPath) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFile(g_pD3DDevice, sFullPath.c_str(),
			&m_mapTexture[sFullPath]);
	}

	return m_mapTexture[sFullPath];
}

void cTextureManager::Destroy()
{
	for each(auto p in m_mapTexture)
	{
		SAFE_RELEASE(p.second);
	}
	m_mapTexture.clear();
}
