#pragma once
#include "cGameParticle.h"
class cGameParticleEruption :
	public cGameParticle
{
public:
	cGameParticleEruption();
	virtual ~cGameParticleEruption();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeEruption(D3DXVECTOR3 vPos, int nMount);
	void ClearEruption();
};

