#pragma once
#include "cGameParticle.h"
class cGameParticleEruption :
	public cGameParticle
{
	D3DXMATRIXA16 m_matTrans;
public:
	cGameParticleEruption();
	virtual ~cGameParticleEruption();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeEruption(D3DXVECTOR3 vOriginPos, D3DXMATRIXA16 matTrans, int nMount);
	void ClearEruption();
};

