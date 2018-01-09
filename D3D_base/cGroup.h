#pragma once
#include "cGameObject.h"
#include "cMtlTex.h"

class cGroup : public cGameObject
{
private:
	SYNTHESIZE_REF(std::vector<ST_PTN_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup();
	~cGroup();

	void Render();
};

