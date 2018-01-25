#include "stdafx.h"
#include "cGameParticleSnow.h"


cGameParticleSnow::cGameParticleSnow(D3DXVECTOR3 vBound_000, D3DXVECTOR3 vBound_111, int nNumParticles)
{
	m_vBound_000 = vBound_000;
	m_vBound_111 = vBound_111;

	m_fSize = 0.1f;
	m_dwVBSize = 2048;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;

	for (int i = 0; i < nNumParticles; i++)
		AddParticles();
}

cGameParticleSnow::~cGameParticleSnow()
{
}

void cGameParticleSnow::ResetParticle(ST_PARTICLEATTRIBUTE* pAttr)
{
	pAttr->bIsAlive = true;

	GetRandomVec3(
		&pAttr->vPosition, 
		&m_vBound_000,
		&m_vBound_111);

	pAttr->vPosition.y = m_vBound_111.y;
	
	pAttr->vVelocity.x = GetRandomFloat(0.0f, 1.0f) * (-2.0f);
	pAttr->vVelocity.y = GetRandomFloat(0.3f, 1.0f) * (-10.0f);
	pAttr->vVelocity.z = GetRandomFloat(0.0f, 1.0f) * (-2.0f);

	pAttr->stColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}
void cGameParticleSnow::Update(float fDelta)
{
	std::list<ST_PARTICLEATTRIBUTE>::iterator iter;
	for (iter = m_listParticles.begin(); iter != m_listParticles.end(); iter++)
	{
		iter->vPosition += iter->vVelocity * fDelta;

		if (iter->vPosition.x < m_vBound_000.x || iter->vPosition.x > m_vBound_111.x ||
			iter->vPosition.y < m_vBound_000.y || iter->vPosition.y > m_vBound_111.y ||
			iter->vPosition.z < m_vBound_000.z || iter->vPosition.z > m_vBound_111.z)
		{
			ResetParticle(&(*iter));
		}
	}
}
