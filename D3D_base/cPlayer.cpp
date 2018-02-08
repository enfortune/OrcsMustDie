#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPhysicsBody.h"
#include "cEnemyBase.h"
#include "cGameParticleSpark.h"
#include "cGameParticleEruption.h"

cPlayer::cPlayer()
	: m_pRotationY(0.0f)
	, m_vPlayerPos(0, 0, 0)
	, m_vPlayerDir(0, 0, 0)
	, m_bIs_W(false)
	, m_bIs_S(false)
	, m_bIs_A(false)
	, m_bIs_D(false)
	, m_fPlayerCurrRot(0.0f)
	, m_fPlayerTargetRot(0.0f)
	, nPlayerMaxHp(0)
	, nPlayerCurHp(0)
	, nPlayerMaxMp(0)
	, nPlayerCurMp(0)
	, bPlayerStun(false)
	, bPlayerMove(false)
	, m_nPlayerAtkDamage(0)
	, m_nPlayerExp(0)
	, m_nPlayerGold(0)
	, m_pPlayerState(PLAYERSTATE_STAND)
	, m_ePlayerTrapType(PLAYERTRAPTYPE_BARRICADE)
	, m_vEnemy(NULL)
	, m_bIsBattle(true)
	, m_pPlayerParticle(NULL)
	, speedX(0.f)
	, speedZ(0.f)
	, m_fPlayerRestore(0.0f)
	, isShiledP(false)
	, m_vSwordPos(0, 0, 0)
{
	D3DXMatrixIdentity(&m_mMatSword);
	D3DXMatrixIdentity(&m_mMatShield);
}

cPlayer::~cPlayer()
{
	Delete();
}

void cPlayer::Setup()
{
	cGameNode::Setup();

	m_pPlayerMesh = new cSkinnedMeshEX;
	m_pPlayerMesh->Setup("Resource/XFile/Player","Resource/XFile/Player/True_King.X");
	m_pPlayerState = PLAYERSTATE_STAND;
	IsPlayerState();
	//m_pPlayerMesh->SetAnimationSet(0, 11, true);
	m_vPlayerPos = GetTransformData()->GetPosition();
	m_vPlayerDir = D3DXVECTOR3(0, 0, 1);

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(-PI / 4.f);
	m_pPhysicsBody->MakeBodyCuboid(0.4f, 1.f, 0.2f, D3DXVECTOR3(0.f, 0.5f, 0.f));
	m_pPhysicsBody->GetPhysicsData().vPos = m_vPlayerPos;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(20.f, 0.f, 20.f);
	m_pPhysicsBody->SetBodyType(PHYSICSBODYTYPE_DINAMIC);

	m_pPlayerParticle = new cGameParticleSpark;
	m_pPlayerParticle->Setup("");
	g_pParticleManager->AddParticle(m_pPlayerParticle);

	nPlayerMaxHp = 500;
	nPlayerCurHp = 500;
	nPlayerMaxMp = 500;
	nPlayerCurMp = 500;
	m_nPlayerGold = 3000;
	m_nPlayerAtkDamage = 10;

	m_vAtkParticleStart = D3DXVECTOR3(0.5f, 1.f, 1.f);
	m_vAtkParticleEnd = D3DXVECTOR3(-0.5f, 0.2f, 1.f);

	m_pPlayerSword = new cSkinnedMeshEX;
	m_pPlayerSword->Setup("Resource/XFile/Player", "Resource/XFile/Player/Sword.X");
	//m_pPlayerSword->SetAnimationSet(0, 0);

	m_pPlayerShield = new cSkinnedMeshEX;
	m_pPlayerShield->Setup("Resource/XFile/Player", "Resource/XFile/Player/Shiled.X");
	//m_pPlayerShield->SetAnimationSet(0, 0);

	m_pPlaterParticleEruption = new cGameParticleEruption;
	m_pPlaterParticleEruption->Setup("");
	g_pParticleManager->AddParticle(m_pPlaterParticleEruption);

	g_pSoundManager->AddSound("PlayerAttackHit", "Resource/Sound/Player/2HSwordHitFlesh03.ogg", false, false);
	g_pSoundManager->AddSound("PlayerWhirlwind", "Resource/Sound/Player/PlayerWhirlwind.ogg", false, true);
	g_pSoundManager->AddSound("PlayerAttack", "Resource/Sound/Player/HumanMaleAttackA.ogg", false, false);
}

void cPlayer::Update(float fDelta)
{
	m_pPlayerMesh->Update(fDelta);
	m_pPlayerSword->Update(fDelta);
	m_pPlayerShield->Update(fDelta);

	int ManaCount = 0;
	
	speedX = 0;
	speedZ = 0;

	if (nPlayerCurMp <= 0) nPlayerCurMp = 0;

	if (nPlayerMaxHp < nPlayerCurHp) nPlayerMaxHp = nPlayerCurHp;
	if (nPlayerMaxMp < nPlayerCurMp) nPlayerMaxMp = nPlayerCurMp;

	if (nPlayerCurHp <= 0 && m_pPlayerState != PLAYERSTATE_DEATH)
	{
		nPlayerCurHp = 0;
		m_pPlayerState = PLAYERSTATE_DEATH;
		IsPlayerState();
	}

	if (m_pPhysicsBody->GetPhysicsData().vPos.y < -5)
	{
		m_pPhysicsBody->GetPhysicsData().vPos = D3DXVECTOR3(47, 0, 29);
		nPlayerCurHp  = nPlayerCurHp - 100;
	}

	D3DXVECTOR3 vLeft;
	D3DXVec3Cross(&vLeft, &m_vPlayerDir, &D3DXVECTOR3(0, 1, 0));
	if (nPlayerCurHp < 50)
	{
		int a = nPlayerCurHp;
	}
	m_bIs_W = false;
	m_bIs_S = false;
	m_bIs_A = false;
	m_bIs_D = false;
	m_fPlayerTargetRot = 0.0f;

	if (m_pPlayerState != PLAYERSTATE_DEATH)
	{
		if (g_pKeyManager->IsStayKeyDown('W') && m_pPlayerState != PLAYERSTATE_ATTACK
			&& m_pPlayerState != PLAYERSTATE_JUMPEND && m_pPlayerState != PLAYERSTATE_SKILL_SHILEDBASH)
		{
			speedX += 5.f * m_vPlayerDir.x;
			speedZ += 5.f * m_vPlayerDir.z;
			if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
				m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
				&& m_pPlayerState != PLAYERSTATE_JUMPING)
			{
				m_pPlayerState = PLAYERSTATE_MOVE;
				IsPlayerState();
			}
			m_bIs_W = true;
		}
		else if (g_pKeyManager->IsOnceKeyUp('W'))
		{
			if (m_pPlayerState == PLAYERSTATE_MOVE)
			{
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}
		}

		if (g_pKeyManager->IsStayKeyDown('S') && m_pPlayerState != PLAYERSTATE_ATTACK
			&& m_pPlayerState != PLAYERSTATE_JUMPEND && m_pPlayerState != PLAYERSTATE_SKILL_SHILEDBASH)
		{
			speedX -= 5.f *  m_vPlayerDir.x;
			speedZ -= 5.f * m_vPlayerDir.z;
			if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
				m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
				&& m_pPlayerState != PLAYERSTATE_JUMPING)
			{
				m_pPlayerState = PLAYERSTATE_MOVE;
				IsPlayerState();
			}
			m_bIs_S = true;
		}
		else if (g_pKeyManager->IsOnceKeyUp('S'))
		{
			if (m_pPlayerState == PLAYERSTATE_MOVE)
			{
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}
		}
		if (g_pKeyManager->IsStayKeyDown('A') && m_pPlayerState != PLAYERSTATE_ATTACK
			&& m_pPlayerState != PLAYERSTATE_JUMPEND && m_pPlayerState != PLAYERSTATE_SKILL_SHILEDBASH)
		{
			speedX += 5.f *  vLeft.x;
			speedZ += 5.f * vLeft.z;
			if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
				m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
				&& m_pPlayerState != PLAYERSTATE_JUMPING)
			{
				m_pPlayerState = PLAYERSTATE_MOVE;
				IsPlayerState();
			}
			m_bIs_A = true;
		}
		else if (g_pKeyManager->IsOnceKeyUp('A'))
		{
			if (m_pPlayerState == PLAYERSTATE_MOVE)
			{
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}
		}

		if (g_pKeyManager->IsStayKeyDown('D') && m_pPlayerState != PLAYERSTATE_ATTACK
			&& m_pPlayerState != PLAYERSTATE_JUMPEND && m_pPlayerState != PLAYERSTATE_SKILL_SHILEDBASH)
		{
			speedX -= 5.f *  vLeft.x;
			speedZ -= 5.f * vLeft.z;
			if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
				m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
				&& m_pPlayerState != PLAYERSTATE_JUMPING)
			{
				m_pPlayerState = PLAYERSTATE_MOVE;
				IsPlayerState();
			}
			m_bIs_D = true;
		}
		else if (g_pKeyManager->IsOnceKeyUp('D'))
		{
			if (m_pPlayerState == PLAYERSTATE_MOVE)
			{
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}
		}

		if (m_bIsBattle)
		{
			if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON) && m_pPlayerState != PLAYERSTATE_ATTACK)
			{
				m_pPlayerState = PLAYERSTATE_ATTACK;
				IsPlayerState();

				PlayerAttacked();
			}
			if (m_pPlayerState == PLAYERSTATE_ATTACK && m_pPlayerMesh->GetAniEnd() == true)
			{
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}
			isShiledP = false;

			if (g_pKeyManager->IsOnceKeyDown('2') && nPlayerCurMp > 100
				&& m_pPlayerState != PLAYERSTATE_SKILL_SHILEDBASH)
			{
				m_pPlayerState = PLAYERSTATE_SKILL_SHILEDBASH;
				IsPlayerState();

				ManaCount = 100;
				nPlayerCurMp = nPlayerCurMp - ManaCount;
			}

			if (m_pPlayerState == PLAYERSTATE_SKILL_SHILEDBASH && m_pPlayerMesh->getCurPosition() >= 0.4f)
			{
				PlayerShiledBash();
			}

			if (m_pPlayerState == PLAYERSTATE_SKILL_SHILEDBASH && m_pPlayerMesh->GetAniEnd() == true)
			{
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}

			if (g_pKeyManager->IsOnceKeyDown('3') && nPlayerCurMp > 50)
			{
				if (m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND)
				{
					m_pPlayerState = PLAYERSTATE_SKILL_WHIRLWIND;
					IsPlayerState();
				}
				PlayerWhirlWind();
			}
			if (m_pPlayerState == PLAYERSTATE_SKILL_WHIRLWIND)
			{

				nPlayerCurMp - 50;
				ManaCount += 2;
				nPlayerCurMp = nPlayerCurMp - ManaCount;

				if (nPlayerCurMp <= 2)
				{
					m_pPlayerState = PLAYERSTATE_STAND;
					IsPlayerState();
				}
			}

			if (g_pKeyManager->IsOnceKeyUp('3'))
			{
				ManaCount = 0;
				m_pPlayerState = PLAYERSTATE_STAND;
				IsPlayerState();
			}

			if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
			{
				if (m_pPhysicsBody->GetPhysicsData().bOnGround == true)
				{
					m_pPhysicsBody->GetPhysicsData().vVelocity.y = 5.f;
					if (m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND)
					{
						m_pPlayerState = PLAYERSTATE_JUMPSTART;
						IsPlayerState();
					}
				}
			}
		}
	}

	PlayerRotationBlend(fDelta);
	PlayerJumpBlend();
	PlayerParticleUpdate();

	m_fPlayerRestore += fDelta;
	if (m_fPlayerRestore >= 1.0f)
	{
		if(m_pPlayerState == PLAYERSTATE_SKILL_WHIRLWIND) PlayerWhirlWind();
		if (nPlayerMaxHp != nPlayerCurHp) nPlayerCurHp += 5;
		if (nPlayerMaxMp != nPlayerCurMp) nPlayerCurMp += 5;
		m_fPlayerRestore = 0;
	}

	m_pPhysicsBody->GetPhysicsData().vVelocity.x = speedX;
	m_pPhysicsBody->GetPhysicsData().vVelocity.z = speedZ;

	m_pPhysicsBody->GetPhysicsData().fRotAngle = m_pRotationY;


	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, m_pRotationY);
	m_vPlayerDir = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vPlayerDir, &m_vPlayerDir, &matR);

	m_mMatSword = m_pPlayerMesh->FindBone("kingvarianwrynn_Bone116");
	m_mMatShield = m_pPlayerMesh->FindBone("kingvarianwrynn_Bone115");

	cGameNode::Update(fDelta);
}

void cPlayer::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIXA16 matMeshWorld;

	D3DXMatrixRotationY(&matMeshWorld, D3DX_PI / 1.f);
	matMeshWorld *= this->GetMatrixToWorld();
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matMeshWorld);

	/* 플레이어 메시 렌더하는 부분임 */
	D3DXMATRIXA16 matPlayerMeshWorld, matPlayeMeshRot;
	D3DXMatrixRotationY(&matPlayeMeshRot, m_fPlayerCurrRot);
	matPlayerMeshWorld = matPlayeMeshRot * matMeshWorld;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matPlayerMeshWorld);
	m_pPlayerMesh->Render();

	D3DXMATRIXA16 matR1, matR2, matS, matT;
	D3DXMatrixIdentity(&matR1);
	D3DXMatrixIdentity(&matR2);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixIdentity(&matS);

	D3DXMatrixRotationY(&matR1, -D3DX_PI / 2);
	D3DXMatrixRotationY(&matR2, -D3DX_PI / 2);
	D3DXMatrixTranslation(&matT, 0, 0.2f, 0);
	D3DXMatrixScaling(&matS, 2, 2, 2);

	D3DXMATRIXA16 matShield, matSword;
	matSword = matS * m_mMatSword * matPlayerMeshWorld;
	matShield = matS * matT * m_mMatShield * matPlayerMeshWorld;

	if (m_pPlayerSword)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matSword);
		m_pPlayerSword->Render();
	}

	if (m_pPlayerShield)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matShield);
		m_pPlayerShield->Render();
	}
	
	//this->m_pPhysicsBody->GetShapeData().stCuboid.Render(g_pD3DDevice);

	cGameNode::Render();
}

void cPlayer::Delete()
{
	SAFE_DELETE(m_pPlayerMesh);
	g_pParticleManager->DeleteParticle(m_pPlayerParticle);
	SAFE_DELETE(m_pPlayerParticle);
	SAFE_DELETE(m_pPlayerSword);
	SAFE_DELETE(m_pPlayerShield);
	g_pParticleManager->DeleteParticle(m_pPlaterParticleEruption);
	SAFE_DELETE(m_pPlaterParticleEruption);
}			   

void cPlayer::PlayerDamaged(int damage)
{
	if (damage)
	{
		nPlayerCurHp = nPlayerCurHp - damage;

		if (0 > nPlayerCurHp) nPlayerCurHp = 0;
	}
}

void cPlayer::PlayerHPHealed(int HPheal)
{
	if (HPheal)
	{
		nPlayerCurHp = nPlayerCurHp + HPheal;
		
		if (nPlayerMaxHp <= nPlayerCurHp) nPlayerCurHp = nPlayerMaxHp;
	}
}

void cPlayer::playerMPHealed(int MPheal)
{
	if (MPheal)
	{
		nPlayerCurMp = nPlayerCurMp + MPheal;
		
		if (nPlayerMaxMp <= nPlayerCurMp) nPlayerCurMp = nPlayerMaxMp;
	}
}

void cPlayer::PlayerAttacked()
{
	for (int i = 0; i < (*m_vEnemy).size(); i++)
	{
		float PlayerLength;
		D3DXVECTOR3 vDist = (*m_vEnemy)[i]->GetTransformData()->GetPosition() - m_pTransformData->GetPosition();
		D3DXVECTOR3 vDir = m_vPlayerDir;
		D3DXVec3Normalize(&vDist, &vDist);
		D3DXVec3Normalize(&vDir, &vDir);
		float fCos = D3DXVec3Dot(&vDist, &vDir);

		PlayerLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vEnemy)[i]->GetTransformData()->GetPosition()));

		if (PlayerLength < 0.8 && fCos > cosf(D3DX_PI/4.f))
		{
			(*m_vEnemy)[i]->getDamage(m_nPlayerAtkDamage * 2);
			g_pSoundManager->Play("PlayerAttackHit");
		}
	}
}

void cPlayer::PlayerWhirlWind()
{
	for (int i = 0; i < (*m_vEnemy).size(); i++)
	{
		float PlayerLength;
		PlayerLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vEnemy)[i]->GetTransformData()->GetPosition()));
		
		if (PlayerLength < 1.5f)
		{
			(*m_vEnemy)[i]->getDamage(m_nPlayerAtkDamage * 2);
		}
	}
}

void cPlayer::PlayerShiledBash()
{
	for (int i = 0; i < (*m_vEnemy).size(); i++)
	{
		float PlayerLength;
		D3DXVECTOR3 vDist = (*m_vEnemy)[i]->GetTransformData()->GetPosition() - m_pTransformData->GetPosition();
		D3DXVECTOR3 vDir = m_vPlayerDir;
		D3DXVec3Normalize(&vDist, &vDist);
		D3DXVec3Normalize(&vDir, &vDir);
		float fCos = D3DXVec3Dot(&vDist, &vDir);

		PlayerLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vEnemy)[i]->GetTransformData()->GetPosition()));

		if (PlayerLength < 1.5f && fCos > cosf(D3DX_PI / 4.f) && (*m_vEnemy)[i]->GetBoss() == false)
		{
			(*m_vEnemy)[i]->getDamage(m_nPlayerAtkDamage * 2);
			(*m_vEnemy)[i]->GetPhysicsBody()->GetPhysicsData().vVelocity = (vDist * 15.f) + D3DXVECTOR3(0.f, 5.f, 0.f);
		}
	}
}

void cPlayer::PlayerJumpBlend()
{
	
	if (m_pPlayerState == PLAYERSTATE_JUMPSTART && m_pPlayerMesh->GetAniEnd() == true)
	{
		m_pPlayerState = PLAYERSTATE_JUMPING;
		IsPlayerState();
	}
	else if (m_pPlayerState == PLAYERSTATE_JUMPING && m_pPhysicsBody->GetPhysicsData().bOnGround == true)
	{
		m_pPlayerState = PLAYERSTATE_JUMPEND;
		IsPlayerState();
	}
	else if (m_pPlayerState == PLAYERSTATE_JUMPEND && m_pPlayerMesh->getCurPosition() >= 0.4)
	{
		m_pPlayerState = PLAYERSTATE_STAND;
		IsPlayerState();
	}

}

void cPlayer::PlayerRotationBlend(float fDelta)
{
	if (!m_bIs_A && !m_bIs_S && !m_bIs_D) m_fPlayerTargetRot = 0.f;
	else if (m_bIs_A)
	{
		if (m_bIs_W) m_fPlayerTargetRot = -D3DX_PI / 4.f;
		else if (m_bIs_S) m_fPlayerTargetRot = -D3DX_PI * 3.f / 4.f;
		else m_fPlayerTargetRot = -D3DX_PI / 2.f;
	}
	else if (m_bIs_D)
	{
		if (m_bIs_W) m_fPlayerTargetRot = +D3DX_PI / 4.f;
		else if (m_bIs_S) m_fPlayerTargetRot = +D3DX_PI * 3.f / 4.f;
		else m_fPlayerTargetRot = +D3DX_PI / 2.f;
	}
	else if (m_bIs_S)
	{
		m_fPlayerTargetRot = D3DX_PI;
	}

	while (m_fPlayerCurrRot <= -D3DX_PI || m_fPlayerCurrRot > D3DX_PI)
	{
		if (m_fPlayerCurrRot <= -D3DX_PI) m_fPlayerCurrRot += D3DX_PI * 2.f;
		if (m_fPlayerCurrRot > D3DX_PI) m_fPlayerCurrRot -= D3DX_PI * 2.f;
	}

	float fRotSpeed = 3.0f;
	float fRotEpsilon = D3DX_PI * fRotSpeed * fDelta * 1.99f;

	if (m_fPlayerCurrRot >= 0.f)
	{
		if (m_fPlayerCurrRot > m_fPlayerTargetRot && m_fPlayerCurrRot - D3DX_PI < m_fPlayerTargetRot)
		{
			m_fPlayerCurrRot -= D3DX_PI * fRotSpeed * fDelta;
			//if (m_fPlayerCurrRot < m_fPlayerTargetRot) m_fPlayerCurrRot = m_fPlayerTargetRot;
		}
		else
		{
			m_fPlayerCurrRot += D3DX_PI * fRotSpeed * fDelta;
			//if (m_fPlayerCurrRot > m_fPlayerTargetRot) m_fPlayerCurrRot = m_fPlayerTargetRot;
		}
	}
	else
	{
		if (m_fPlayerCurrRot < m_fPlayerTargetRot && m_fPlayerCurrRot + D3DX_PI > m_fPlayerTargetRot)
		{
			m_fPlayerCurrRot += D3DX_PI * fRotSpeed * fDelta;
			//if (m_fPlayerCurrRot > m_fPlayerTargetRot) m_fPlayerCurrRot = m_fPlayerTargetRot;
		}
		else
		{
			m_fPlayerCurrRot -= D3DX_PI * fRotSpeed * fDelta;
			//if (m_fPlayerCurrRot < m_fPlayerTargetRot) m_fPlayerCurrRot = m_fPlayerTargetRot;
		}
	}
	if (fabs(m_fPlayerCurrRot - m_fPlayerTargetRot) <= fRotEpsilon ||
		fabs(m_fPlayerCurrRot - m_fPlayerTargetRot) >= (D3DX_PI * 2.f - fRotEpsilon))
		m_fPlayerCurrRot = m_fPlayerTargetRot;

	while (m_fPlayerCurrRot <= -D3DX_PI || m_fPlayerCurrRot > D3DX_PI)
	{
		if (m_fPlayerCurrRot <= -D3DX_PI) m_fPlayerCurrRot += D3DX_PI * 2.f;
		if (m_fPlayerCurrRot > D3DX_PI) m_fPlayerCurrRot -= D3DX_PI * 2.f;
	}
}

void cPlayer::setEnemy(std::vector<cEnemyBase*>* Enemy)
{
	m_vEnemy	= Enemy;
}

void cPlayer::IsPlayerState()
{
	switch (m_pPlayerState)
	{
	case PLAYERSTATE_STAND:
		m_pPlayerMesh->SetAnimationSetBlend(0, 11, true);
		g_pSoundManager->Stop("PlayerWhirlwind");
		break;
	case PLAYERSTATE_MOVE:
		m_pPlayerMesh->SetAnimationSet(0, 10, true);
		g_pSoundManager->Stop("PlayerWhirlwind");
		break;
	case PLAYERSTATE_ATTACK:
		m_pPlayerMesh->SetAnimationSetBlend(0, 4, false);
		g_pSoundManager->Play("PlayerAttack");
		break;
	case PLAYERSTATE_HIT:
		m_pPlayerMesh->SetAnimationSetBlend(0, 3, false);
		break;
	case PLAYERSTATE_JUMPSTART:
		m_pPlayerMesh->SetAnimationSetBlend(0, 7, false);
		break;					
	case PLAYERSTATE_JUMPING:		  
		m_pPlayerMesh->SetAnimationSet(0, 5, false);
		break;						  
	case PLAYERSTATE_JUMPEND :		  
		m_pPlayerMesh->SetAnimationSetBlend(0, 6, false);
		break;
	case PLAYERSTATE_SKILL_WHIRLWIND :
		m_pPlayerMesh->SetAnimationSetBlend(0, 0, true);
		g_pSoundManager->Play("PlayerWhirlwind");
		break;
	case PLAYERSTATE_SKILL_SHILEDBASH :
		m_pPlayerMesh->SetAnimationSetBlend(0, 2, false);
		break;
	case PLAYERSTATE_DEATH :
		m_pPlayerMesh->SetAnimationSetBlend(0, 9, false);
		break;
	default:
		break;
	}
}

void cPlayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cPlayer::PlayerParticleUpdate()
{
	bool isparticle = false;

	switch (m_pPlayerState)
	{
		case PLAYERSTATE_STAND:
		break;
		case PLAYERSTATE_MOVE:
		break;
		case PLAYERSTATE_BACKMOVE:
		break;
		case PLAYERSTATE_ATTACK:
			// 시작 : 0.4    끝 : 0.6  사잇값 0.2
			if (m_pPlayerMesh->getCurPosition() >= 0.4f && m_pPlayerMesh->getCurPosition() <= 0.6f)
			{
				D3DXVECTOR3 vPos =
					m_vAtkParticleStart * (-0.4f + 1.0f - m_pPlayerMesh->getCurPosition()) / 0.2f +
					m_vAtkParticleEnd * (-0.4f + m_pPlayerMesh->getCurPosition()) / 0.2f;

				D3DXVec3TransformCoord(&vPos, &vPos, &this->GetMatrixToWorld());
				m_pPlayerParticle->MakeSpark(vPos, 20);
			}
		break;
		case PLAYERSTATE_HIT:
		break;
		case PLAYERSTATE_JUMPSTART:
		break;
		case PLAYERSTATE_JUMPING:
		break;
		case PLAYERSTATE_JUMPEND:
		break;
		case PLAYERSTATE_DEATH:
		break;
		case PLAYERSTATE_SKILL_SHILEDBASH:
			if (m_pPlayerMesh->getCurPosition() >= 0.4f && m_pPlayerMesh->getCurPosition() <= 0.5f)
			{
				D3DXVECTOR3 vMakePos(0, 0.5, 0);
				 m_pPlaterParticleEruption->MakeEruption(vMakePos, this->GetMatrixToWorld(),200);
			}
		break;
		case PLAYERSTATE_SKILL_WHIRLWIND:
			{
				D3DXVECTOR3 vPos = D3DXVECTOR3(1, 0, 0);
				D3DXVECTOR3 vPosTemp;
				
				D3DXMATRIXA16 matR, matWorld;
				D3DXMatrixRotationY(&matR, D3DX_PI/36.f);

				matWorld = m_mMatSword;
				D3DXVec3TransformCoord(&vPosTemp, &vPos, &(matWorld * this->GetMatrixToWorld()));
				m_pPlayerParticle->MakeSpark(vPosTemp, 30);
				
				for (int i = 0; i < 3; i++)
				{
					matWorld *= matR;
					D3DXVec3TransformCoord(&vPosTemp, &vPos, &(matWorld * this->GetMatrixToWorld()));
					m_pPlayerParticle->MakeSpark(vPos, 30);
				}
			}
		break;
		default:
		break;
	}
}
