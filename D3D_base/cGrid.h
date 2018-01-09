#pragma once
class cGrid
{
private:
	std::vector<ST_PC_VERTEX>		m_vecVertex;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;

public:
	cGrid();
	~cGrid();

	void Setup(int nNUmHalfTile = 15, float fInterval = 1.0f);
	void Render();
};

