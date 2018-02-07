#pragma once
#include "cGameParticle.h"
class cGameParticleEruption :
	public cGameParticle
{
	D3DXMATRIX m_matTrans;
public:
	cGameParticleEruption();
	virtual ~cGameParticleEruption();

	virtual void ResetParticle(ST_PARTICLEATTRIBUTE* pAttr) override;
	virtual void Update(float fDelta) override;
	virtual void PreRender() override;
	virtual void PostRender() override;

	void MakeEruption(D3DXVECTOR3 vOriginPos, D3DXMATRIX matTrans, int nMount);
	void ClearEruption();
};

