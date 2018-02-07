#pragma once
#include "cGameParticle.h"
class cGameParticleShockwave : public cGameParticle
{
	float m_fRange;
public:
	cGameParticleShockwave();
	virtual ~cGameParticleShockwave();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeShockWave(D3DXVECTOR3 vPos, float fRange, int nMount);
};

