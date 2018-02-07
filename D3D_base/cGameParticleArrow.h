#pragma once
#include "cGameParticle.h"
class cGameParticleArrow : public cGameParticle
{
	D3DXVECTOR3 m_vDir;
	float m_fRange;

public:
	cGameParticleArrow();
	virtual ~cGameParticleArrow();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeArrow(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fRange, int nMount);
	void ClearArrow();
};

