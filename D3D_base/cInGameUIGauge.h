#pragma once
#include "cGameNode.h"

class cGameSprite;
class cPlayer;

class cInGameUIGauge : public cGameNode
{
	cPlayer * m_pPlayer;

	float m_fRatioHP; // 0.f~1.f
	float m_fRatioMP; // 0.f~1.f

	cGameSprite* m_pSpriteGaugeBack;
	cGameSprite* m_pSpriteHPFront;
	cGameSprite* m_pSpriteMPFront;

	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle); // Z angle
	SYNTHESIZE_REF(D3DXVECTOR2, m_vScale, Scale);
private:
	void UpdateRatio();
public:
	cInGameUIGauge();
	virtual ~cInGameUIGauge();

	virtual void Setup(cPlayer * pPlayer = nullptr);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render()override;
	virtual void Delete()override;

	virtual D3DXMATRIXA16 GetMatirixToParent() override;
};

