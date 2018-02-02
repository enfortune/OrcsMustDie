#pragma once
#include "cGameNode.h"

enum PLAYERSTATE
{
	PLAYERSTATE_STAND,
	PLAYERSTATE_MOVE,
	PLAYERSTATE_BACKMOVE,
	PLAYERSTATE_ATTACK,
	PLAYERSTATE_HIT,
	PLAYERSTATE_JUMPSTART,
	PLAYERSTATE_JUMPING,
	PLAYERSTATE_JUMPEND,
	PLAYERSTATE_SKILL_SHILEDBASH,
	PLAYERSTATE_SKILL_WHIRLWIND
};

class cSkinnedMeshEX;
class cEnemy;
class cPlayer : public cGameNode
{
protected:
	cSkinnedMeshEX *			m_pPlayerMesh;
	std::vector<cEnemy*>	*m_vEnemy;
	PLAYERSTATE					m_pPlayerState;

	SYNTHESIZE_REF(float, m_pRotationY, RotationY);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPlayerPos, PlayerPos);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPlayerDir, PlayerDir);

	//KeyCheck
	bool m_bIs_W;
	bool m_bIs_S;
	bool m_bIs_A;
	bool m_bIs_D;

	float m_fPlayerCurrRot;
	float m_fPlayerTargetRot;

	bool isJump;
	//HP,MP นื ป๓ลย
	//int		nPlayerMaxHp, nPlayerCurHp, nPlayerMaxMp, nPlayerCurMp;
	BOOL	bPlayerStun, bPlayerMove;

	SYNTHESIZE_REF(int, nPlayerMaxHp, PlayerMaxHp);
	SYNTHESIZE_REF(int, nPlayerCurHp, PlayerCurHp);
	SYNTHESIZE_REF(int, nPlayerMaxMp, PlayerMaxMp);
	SYNTHESIZE_REF(int, nPlayerCurMp, PlayerCurMp);

	SYNTHESIZE_REF(int, m_nPlayerAtkDamage, PlayerAtkDamage);
	SYNTHESIZE_REF(int, m_nPlayerExp, PlayerExp);
	SYNTHESIZE_REF(int, m_nPlayerGold, PlayerGold);
public:
	cPlayer();
	~cPlayer();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	void PlayerDamaged(int damage);
	void PlayerHPHealed(int HPheal);
	void playerMPHealed(int MPheal);
	void PlayerAttacked();
	void PlayerWhirlWind();
	void PlayerShiledBash();
	void PlayerJumpBlend();
	void setEnemy(std::vector<cEnemy*>* Enemy);
	void IsPlayerState();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

