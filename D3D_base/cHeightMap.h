#pragma once
#include "iMap.h"

class cHeightMap : public iMap
{
private:
	std::vector<D3DXVECTOR3>	m_vecVertex;
	int							m_nTileCount;
	LPD3DXMESH					m_pMesh;

public:
	cHeightMap();
	virtual ~cHeightMap();

	void Setup(char* szFolder, char* szRaw, char* szTex);
	void Render();

	virtual bool GetHeight(IN float x, OUT float& y, IN float z);
};
