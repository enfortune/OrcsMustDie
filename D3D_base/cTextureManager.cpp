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
LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(std::string sFullPath, OUT D3DXIMAGE_INFO* pImageInfo)
{
	if (m_mapTexture.find(sFullPath) == m_mapTexture.end())
	{
		ZeroMemory(pImageInfo, sizeof(D3DXIMAGE_INFO));

		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			sFullPath.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&m_mapImageInfo[sFullPath],
			NULL,
			&m_mapTexture[sFullPath]);
	}

	memcpy_s(pImageInfo, sizeof(D3DXIMAGE_INFO), &m_mapImageInfo[sFullPath], sizeof(D3DXIMAGE_INFO));

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
