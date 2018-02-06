#pragma once
#include "cChaNode.h"
#include "cEnemyBase.h"

class cSkinnedMeshEX;
class cPlayer;

class cBoss : public cEnemyBase
{
private:
	D3DXVECTOR3 vPlayerPos;
	cPlayer* m_pPlayer;
	struct tempPoint
	{
		D3DXVECTOR3 p;
		bool start;
		bool goal;
		bool check;
		int prev;
		int next[3];
		int cost;
		int curCost;
		bool bmove;
	};

	tempPoint tp[5];
	bool bPhase1, bPhase2, bPhase3;


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
	void Dijkstra(int tpnum, float fDelta);
	void find(int tpNum, float fDelta);
	void setPlayer(cPlayer* pSetPlayer);
	void getDamage(int nDamage);
	void HpManager();
	void Hit();

};

