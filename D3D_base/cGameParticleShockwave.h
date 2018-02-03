#pragma once
#include "cGameParticle.h"
class cGameParticleShockwave : public cGameParticle
{
public:
	cGameParticleShockwave();
	virtual ~cGameParticleShockwave();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeShockWave(D3DXVECTOR3 vPos, int nMount);
};

