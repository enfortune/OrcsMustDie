#pragma once
#include "cGameNode.h"


class cSkinnedMesh;

class cChaNode : public cGameNode
{
protected:

	cSkinnedMesh* m_pSkinnedMesh;
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vDir;
	float fRotY;
	int		nMaxHp, nCurHp, nMaxMp, nCurMp;
	BOOL	bStun, bPoison, bMove;

	SYNTHESIZE_REF(int, m_nAtkDamage, AtkDamage);
	SYNTHESIZE_REF(int, m_nExp, Exp);
	SYNTHESIZE_REF(int, m_nGold, Gold);

public:
	cChaNode();
	virtual ~cChaNode();


	virtual void Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime()) override;

};
