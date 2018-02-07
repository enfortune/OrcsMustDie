#pragma once

#define g_pParticleManager cParticleManager::GetInstance()

class cGameParticle;
class cParticleManager
{
	SINGLETONE(cParticleManager);
private:
	std::set<cGameParticle*> m_setParticle;

public:

	void AddParticle(cGameParticle* pParticle);
	void DeleteParticle(cGameParticle* pParticle);

	void UpdateParticles(float fDelta);
	void RenderParticles();

	void ClearParticles();
};

