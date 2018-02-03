#include "stdafx.h"
#include "cGameParticleShockwave.h"


cGameParticleShockwave::cGameParticleShockwave()
{
	m_fSize = 0.2f;
	m_dwVBSize = 4096;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;
}


cGameParticleShockwave::~cGameParticleShockwave()
{
}


void cGameParticleShockwave::ResetParticle(ST_PARTICLEATTRIBUTE* pAttr)
{
	pAttr->bIsAlive = true;

	
	D3DXVECTOR3 vTemp = D3DXVECTOR3(0, 0, 1);

	D3DXVECTOR3 vAngle = D3DXVECTOR3(
		-D3DXToRadian(rand() % 100 / 10.0f),
		D3DXToRadian(rand() % 3600 / 10.0f),
		0.f);

	D3DXMATRIX matRX, matRY, matRZ, matWorld;
	D3DXMatrixRotationX(&matRX, vAngle.x);
	D3DXMatrixRotationY(&matRY, vAngle.y);
	D3DXMatrixRotationZ(&matRZ, vAngle.z);
	matWorld = matRX * matRY * matRZ;

	D3DXVec3TransformCoord(&vTemp,
		&vTemp,
		&matWorld);

	float fVelocity = (float)((rand() % 2) + 1) * (1.f - powf(GetRandomFloat(0.f, 1.f), 3.f)) + 1.0f;
	
	pAttr->vPosition = m_vOrigin;
	pAttr->vVelocity = vTemp;
	pAttr->vVelocity *= fVelocity * 0.75f;

	pAttr->stColor = pAttr->stColorOrigin = D3DXCOLOR(
		GetRandomFloat(0.0f, 0.4f),
		GetRandomFloat(0.6f, 1.0f),
		GetRandomFloat(0.0f, 0.4f),
		GetRandomFloat(0.9f, 1.0f));
	pAttr->stColorFade = D3DXCOLOR(
		GetRandomFloat(0.0f, 0.01f),
		GetRandomFloat(0.05f, 0.1f),
		GetRandomFloat(0.0f, 0.01f),
		GetRandomFloat(0.7f, 0.9f));

	pAttr->fAge = 0.0f;
	pAttr->fLifeTime = GetRandomFloat(0.0f, 0.5f) + 0.5f;
}
void cGameParticleShockwave::Update(float fDelta)
{
	std::list<ST_PARTICLEATTRIBUTE>::iterator iter;

	for (iter = m_listParticles.begin(); iter != m_listParticles.end();)
	{
		if (!iter->bIsAlive)
			iter = m_listParticles.erase(iter);
		else
		{
			iter->vPosition += iter->vVelocity * fDelta;
			iter->fAge += fDelta;

			iter->stColor.r =
				(iter->stColorOrigin.r * (iter->fLifeTime - iter->fAge) / iter->fLifeTime) +
				(iter->stColorFade.r * iter->fAge / iter->fLifeTime);
			iter->stColor.g =
				(iter->stColorOrigin.g * (iter->fLifeTime - iter->fAge) / iter->fLifeTime) +
				(iter->stColorFade.g * iter->fAge / iter->fLifeTime);
			iter->stColor.b =
				(iter->stColorOrigin.b * (iter->fLifeTime - iter->fAge) / iter->fLifeTime) +
				(iter->stColorFade.b * iter->fAge / iter->fLifeTime);
			iter->stColor.a =
				(iter->stColorOrigin.a * (iter->fLifeTime - iter->fAge) / iter->fLifeTime) +
				(iter->stColorFade.a * iter->fAge / iter->fLifeTime);

			if (iter->fAge > iter->fLifeTime)
				iter->bIsAlive = false;

			iter++;
		}
	}
}
void cGameParticleShockwave::PreRender()
{
	cGameParticle::PreRender();

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}
void cGameParticleShockwave::PostRender()
{
	cGameParticle::PostRender();
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void cGameParticleShockwave::MakeShockWave(D3DXVECTOR3 vPos, int nMount)
{
	m_vOrigin = vPos;
	for (int i = 0; i < nMount; i++)
	{
		AddParticles();
	}
}