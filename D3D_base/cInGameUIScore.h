#pragma once
#include "cGameNode.h"

class cGameUISprite;
class cGameUILabel;

class cPlayer;

class cInGameUIScore : public cGameNode
{
	cPlayer *	m_pPlayer;

	int		m_nScore;

	cGameUISprite*	m_pSpriteBack;
	cGameUILabel*	m_pLabel;

	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);

public:
	cInGameUIScore();
	~cInGameUIScore();

	virtual void Setup(cPlayer * pPlayer = nullptr);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render()override;
	virtual void Delete()override;
	virtual D3DXMATRIXA16 GetMatirixToParent() override;
};

