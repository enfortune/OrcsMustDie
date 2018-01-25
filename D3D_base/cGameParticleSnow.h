#pragma once
#include "cGameParticle.h"
class cGameParticleSnow : public cGameParticle
{
public:
	cGameParticleSnow(D3DXVECTOR3 vBound_000, D3DXVECTOR3 vBound_111, int nNumParticles);
	virtual ~cGameParticleSnow();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
};

