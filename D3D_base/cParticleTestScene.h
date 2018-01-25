#pragma once
#include "cGameScene.h"

class cCamera;
class cGrid;
class cGameParticleSnow;
class cGameParticleFirework;

class cParticleTestScene : public cGameScene
{
	cCamera*	m_pCamera;
	cGrid*		m_pGrid;

	ST_FRUSTUM	m_stFrustum;

	std::vector<ST_PC_VERTEX> m_vecParticle;

	cGameParticleSnow* m_pSnow;
	cGameParticleFirework* m_pFirework;

	bool m_bIsCulling;

public:
	cParticleTestScene();
	~cParticleTestScene();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void SetupParticle();
	void UpdateParticle(float fDelta = g_pTimeManager->GetEllapsedTime());
	void RenderParticle();
};

