#pragma once
#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	SINGLETONE(cTextureManager);

	std::map<std::string, LPDIRECT3DTEXTURE9>	m_mapTexture;
	std::map<std::string, D3DXIMAGE_INFO>	m_mapImageInfo;
public:
	LPDIRECT3DTEXTURE9 GetTexture(std::string sFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string sFullPath, OUT D3DXIMAGE_INFO* pImageInfo);

	void Destroy();
};

