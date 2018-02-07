#pragma once
#include "cGameParticle.h"
class cGameParticleEruption :
	public cGameParticle
{
	D3DXVECTOR3 m_vAxis;
	float m_fRotAngle;
public:
	cGameParticleEruption();
	virtual ~cGameParticleEruption();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeEruption(D3DXVECTOR3 vPos, D3DXVECTOR3 vAxis, float fRotAngle, int nMount);
	void ClearEruption();
};

