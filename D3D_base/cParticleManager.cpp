#include "stdafx.h"
#include "cParticleManager.h"
#include "cGameParticle.h"


cParticleManager::cParticleManager()
{
}


cParticleManager::~cParticleManager()
{
	this->ClearParticles;
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

void cParticleManager::UpdateParticles(float fDelta)
{
	std::set<cGameParticle*>::iterator iter;
	for (iter = m_setParticle.begin(); iter != m_setParticle.end(); iter++)
	{
		(*iter)->Update(fDelta);
	}
}

void cParticleManager::RenderParticles()
{
	std::set<cGameParticle*>::iterator iter;
	for (iter = m_setParticle.begin(); iter != m_setParticle.end(); iter++)
	{
		(*iter)->Render();
	}
}

void cParticleManager::ClearParticles()
{
	std::set<cGameParticle*>::iterator iter;

	for (iter = m_setParticle.begin(); iter != m_setParticle.end();)
	{
		iter = m_setParticle.erase(iter);
	}
}
