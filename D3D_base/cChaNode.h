#pragma once
#include "cGameNode.h"


class cSkinnedMeshEX;

class cChaNode : public cGameNode
{
protected:

	enum state
	{
		IDLE,
		MOVE,
		ATTACK_MOVE,
		ATTACK,
		SKILL,
		HIT,
		JUMP_START,
		JUMP,
		JUMP_END,
		DEAD
	};
	state EnemyState;
	bool bAttack, bAttackAction, bIdle, bDead, bDeadbody;

	int nIdleAni, nAttackAni, nDeadAni, nMoveAni, nAttackMoveAni,
		nJumpStartAni, nJumpAni, nJumpEndAni, nHitAni, nSkillAni;

	cSkinnedMeshEX * m_pSkinnedMesh;
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

	float fDeadCount;

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
	void Dijkstra(int tpnum, float fDelta);
	void find(int tpNum, float fDelta);
	void getDamage(int nDamage);
	void HpManager();

};
