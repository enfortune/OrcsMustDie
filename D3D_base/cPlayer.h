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
	PLAYERSTATE_DEATH,
	PLAYERSTATE_SKILL_SHILEDBASH,
	PLAYERSTATE_SKILL_WHIRLWIND,
};

enum PLAYERTRAPTYPE
{
	PLAYERTRAPTYPE_BARRICADE,
	PLAYERTRAPTYPE_SPIKE,
	PLAYERTRAPTYPE_HEALINGWELL
};

class cSkinnedMeshEX;
class cGameParticleSpark;
class cGameParticleEruption;
class cEnemyBase;
class cPlayer : public cGameNode
{
protected:
	cSkinnedMeshEX*				m_pPlayerMesh;
	cSkinnedMeshEX*				m_pPlayerSword;
	cSkinnedMeshEX*				m_pPlayerShield;

	std::vector<cEnemyBase *>	*m_vEnemy;
	PLAYERSTATE					m_pPlayerState;
	cGameParticleSpark*			m_pPlayerParticle;
	cGameParticleEruption*		m_pPlaterParticleEruption;

	D3DXVECTOR3					m_vAtkParticleStart;
	D3DXVECTOR3					m_vAtkParticleEnd;

	D3DXMATRIXA16				m_mMatSword;
	D3DXMATRIXA16				m_mMatShield;

	SYNTHESIZE_REF(bool, m_bIsBattle, IsBattle)
	SYNTHESIZE_REF(float, m_pRotationY, RotationY);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPlayerPos, PlayerPos);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPlayerDir, PlayerDir);
	SYNTHESIZE_REF(PLAYERTRAPTYPE, m_ePlayerTrapType, PlayerTrapType);
	//KeyCheck
	bool m_bIs_W;
	bool m_bIs_S;
	bool m_bIs_A;
	bool m_bIs_D;

	float m_fPlayerCurrRot;
	float m_fPlayerTargetRot;

	float m_fPlayerRestore;
	float speedX, speedZ;

	bool isShiledP;

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
	virtual void Update(float fDelta) override;
	virtual void Render() override;
	virtual void Delete() override;

	void PlayerDamaged(int damage);
	void PlayerHPHealed(int HPheal);
	void playerMPHealed(int MPheal);
	void PlayerAttacked();
	void PlayerWhirlWind();
	void PlayerShiledBash();
	void PlayerJumpBlend();
	void PlayerRotationBlend(float fDelta);
	void setEnemy(std::vector<cEnemyBase *>* Enemy);
	void IsPlayerState();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void PlayerParticleUpdate();
};