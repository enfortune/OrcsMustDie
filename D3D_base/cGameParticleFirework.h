#pragma once
#include "cGameParticle.h"
class cGameParticleFirework :
	public cGameParticle
{
public:
	cGameParticleFirework(D3DXVECTOR3 vOrigin, int nNumParticles);
	virtual ~cGameParticleFirework();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;
};

