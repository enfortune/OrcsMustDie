#pragma once
#include "cGameParticle.h"
class cGameParticleSpark : public cGameParticle
{

public:
	cGameParticleSpark();
	virtual ~cGameParticleSpark();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;
	
	void MakeSpark(D3DXVECTOR3 vPos, int nMount);
};

