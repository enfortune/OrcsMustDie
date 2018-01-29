#pragma once
class cMapToolGrid
{
private:
	std::vector<ST_PC_VERTEX>		m_vecVertex;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	
	SYNTHESIZE(float, m_fHeight, Height);
public:
	cMapToolGrid();
	~cMapToolGrid();

	void Setup(int nNumTile = 15, float fInterval = 1.0f);
	void Render();
};

