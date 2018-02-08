#pragma once
#include "cChaNode.h"
#include "cEnemyBase.h"

class cSkinnedMeshEX;
class cPlayer;
class cGameParticleShockwave;
class GraphFindPath;

class cBoss : public cEnemyBase
{
private:
	D3DXVECTOR3 vPlayerPos;
	cPlayer* m_pPlayer;

	GraphFindPath* m_pFindPath;
	std::vector<D3DXVECTOR3> m_vPath;

	bool bPhase1, bPhase2, bPhase3;
	bool bskillEffect;
	/* particle */
	cGameParticleShockwave* m_pParticle;

public:
	cBoss();
	virtual ~cBoss();

	virtual void Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void Idle();
	void Move(D3DXVECTOR3 vGoal, float fDelta);
	void Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum);
	void Attack(float fDelta);
	void Dead();
	void Jump();
	void find(int tpNum, float fDelta);
	void setPlayer(cPlayer* pSetPlayer);
	void getDamage(int nDamage);
	void HpManager();
	void Hit();
	void sendGold();
	void setPath(GraphFindPath* pFindPath);

	void shockwave();
};

