#include "stdafx.h"
#include "cParticleManager.h"
#include "cGameParticle.h"


cParticleManager::cParticleManager()
{
}


cParticleManager::~cParticleManager()
{
}

void cParticleManager::AddParticle(cGameParticle * pParticle)
{
	m_setParticle.insert(pParticle);
}

void cParticleManager::DeleteParticle(cGameParticle * pParticle)
{
	std::set<cGameParticle*>::iterator iter;
	iter = m_setParticle.find(pParticle);
	if (iter != m_setParticle.end())
	{
		iter = m_setParticle.erase(iter);
	}
}

void cParticleManager::Update(float fDelta)
{
	std::set<cGameParticle*>::iterator iter;
	for (iter = m_setParticle.begin(); iter != m_setParticle.end(); iter++)
	{
		(*iter)->Update(fDelta);
	}
}

void cParticleManager::Render()
{
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	std::set<cGameParticle*>::iterator iter;
	for (iter = m_setParticle.begin(); iter != m_setParticle.end(); iter++)
	{
		(*iter)->Render();
	}
}

void cParticleManager::Destroy()
{
	this->Clear();
}

void cParticleManager::Clear()
{
	std::set<cGameParticle*>::iterator iter;

	for (iter = m_setParticle.begin(); iter != m_setParticle.end();)
	{
		iter = m_setParticle.erase(iter);
	}
}
