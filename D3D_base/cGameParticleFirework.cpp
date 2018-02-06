#include "stdafx.h"
#include "cGameParticleFirework.h"


cGameParticleFirework::cGameParticleFirework(D3DXVECTOR3 vOrigin, int nNumParticles)
{
	m_vOrigin = vOrigin;
	m_fSize = 0.3f;
	m_dwVBSize = 2048;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;

	for (int i = 0; i < nNumParticles; i++)
		AddParticles();
}


cGameParticleFirework::~cGameParticleFirework()
{
}

void cGameParticleFirework::ResetParticle(ST_PARTICLEATTRIBUTE* pAttr)
{
	pAttr->bIsAlive = true;
	pAttr->vPosition = m_vOrigin;

	float fRadius = GetRandomFloat(0.1f, 0.1f);
	D3DXVECTOR3 vTemp = D3DXVECTOR3(0, 0, fRadius);

	D3DXVECTOR3 vAngle = D3DXVECTOR3(
		D3DXToRadian(rand() % 3600 / 10.0f),
		D3DXToRadian(rand() % 3600 / 10.0f),
		D3DXToRadian(rand() % 3600 / 10.0f));

	D3DXMATRIX matRX, matRY, matRZ, matWorld;
	D3DXMatrixRotationX(&matRX, vAngle.x);
	D3DXMatrixRotationY(&matRY, vAngle.y);
	D3DXMatrixRotationZ(&matRZ, vAngle.z);
	matWorld = matRX * matRY * matRZ;

	D3DXVec3TransformCoord(&vTemp,
		&vTemp,
		&matWorld);

	pAttr->vVelocity = vTemp + D3DXVECTOR3(0.f, 0.2f, 0.f);
	pAttr->vVelocity *= 50.0f;
	pAttr->vAcceleration = D3DXVECTOR3(0.f, -10.f, 0.f);

	pAttr->stColor = D3DXCOLOR(
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f), 
		1.0f);

	pAttr->fAge = 0.0f;
	pAttr->fLifeTime = GetRandomFloat(0.0f, 1.0f) + 2.0f;
}
void cGameParticleFirework::Update(float fDelta)
{
	std::list<ST_PARTICLEATTRIBUTE>::iterator iter;

	for (iter = m_listParticles.begin(); iter != m_listParticles.end(); iter++)
	{
		if (iter->bIsAlive)
		{
			iter->vVelocity += iter->vAcceleration * fDelta;
			iter->vPosition += iter->vVelocity * fDelta;
			iter->fAge += fDelta;

			if (iter->fAge > iter->fLifeTime)
				iter->bIsAlive = false;
		}
	}
}
void cGameParticleFirework::PreRender()
{
	cGameParticle::PreRender();

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}
void cGameParticleFirework::PostRender()
{
	cGameParticle::PostRender();
	//g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}