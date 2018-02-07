#include "stdafx.h"
#include "cGameParticleArrow.h"




cGameParticleArrow::cGameParticleArrow()
	: m_vDir(0.f, 0.f, 0.f)
	, m_fRange(1.f)
{
	m_fSize = 0.2f;
	m_dwVBSize = 4096;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;
}

cGameParticleArrow::~cGameParticleArrow()
{
}

void cGameParticleArrow::ResetParticle(ST_PARTICLEATTRIBUTE* pAttr)
{
	pAttr->bIsAlive = true;

	float fMainRadius = 1.f;
	float fSubRadius = GetRandomFloat(0.0f, 0.1f);

	D3DXVECTOR3 vTemp = D3DXVECTOR3(0, 0, fMainRadius);

	D3DXVECTOR3 vAngle = D3DXVECTOR3(
		m_vDir.x,
		m_vDir.y,
		m_vDir.z);

	D3DXMATRIX matRX, matRY, matRZ, matWorld;
	D3DXMatrixRotationX(&matRX, vAngle.x);
	D3DXMatrixRotationY(&matRY, vAngle.y);
	D3DXMatrixRotationZ(&matRZ, vAngle.z);
	matWorld = matRX * matRY * matRZ;

	D3DXVec3TransformCoord(&vTemp,
		&vTemp,
		&matWorld);

	pAttr->vPosition = m_vOrigin + vTemp * GetRandomFloat(0.0f, 0.1f);
	pAttr->vVelocity = vTemp;
	pAttr->vVelocity *= 5.0f;

	pAttr->stColor = pAttr->stColorOrigin = D3DXCOLOR(
		GetRandomFloat(0.0f, 0.3f),
		GetRandomFloat(0.0f, 0.3f),
		GetRandomFloat(0.8f, 1.0f),
		GetRandomFloat(1.0f, 1.0f));
	pAttr->stColorFade = D3DXCOLOR(
		GetRandomFloat(0.0f, 0.01f),
		GetRandomFloat(0.0f, 0.01f),
		GetRandomFloat(0.0f, 0.05f),
		GetRandomFloat(1.0f, 1.0f));

	pAttr->fAge = 0.0f;
	pAttr->fLifeTime = GetRandomFloat(0.0f, 0.2f) + 0.3f;
}
void cGameParticleArrow::Update(float fDelta)
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
void cGameParticleArrow::PreRender()
{
	cGameParticle::PreRender();

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

}
void cGameParticleArrow::PostRender()
{
	cGameParticle::PostRender();
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void cGameParticleArrow::MakeArrow(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fRange, int nMount)
{
	m_vOrigin = vPos;
	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_fRange = fRange;
	for (int i = 0; i < nMount; i++)
	{
		AddParticles();
	}
}

void cGameParticleArrow::ClearArrow()
{
	for (std::list<ST_PARTICLEATTRIBUTE>::iterator iter = m_listParticles.begin(); iter != m_listParticles.end(); )
	{
		iter = m_listParticles.erase(iter);
	}
}