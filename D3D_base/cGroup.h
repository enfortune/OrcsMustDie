#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cGroup : public cObject
{
private:
	SYNTHESIZE_REF(std::vector<ST_PTN_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup();
	~cGroup();

	void Render();
};

