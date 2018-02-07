#include "stdafx.h"
#include "cBoss.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPlayer.h"
#include "cPhysicsBody.h"
#include "cGameParticleShockwave.h"

cBoss::cBoss()
	: m_pParticle(nullptr)
{
}


cBoss::~cBoss()
{
	Delete();
}

void cBoss::Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup)
{
	cGameNode::Setup(true);

	m_bBoss = true;

	nIdleAni = 7;
	nAttackAni = 2;
	nDeadAni = 5;
	nMoveAni = 3;
	nAttackMoveAni = 6;
	nJumpStartAni = 7;
	nJumpAni = 5;
	nJumpEndAni = 6;
	nHitAni = 4;
	nSkillAni = 9;
	m_nGold = 300;

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(0.0f);
	m_pPhysicsBody->MakeBodyCuboid(3.f, 3.f, 1.5f,D3DXVECTOR3(0, 1.5, 0));
	m_pPhysicsBody->GetPhysicsData().fElasticity = 0.f;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(20.f, 0.f, 20.f);
	m_pPhysicsBody->GetBodyType() = PHYSICSBODYTYPE_STATIC;

	m_nAtkDamage = 50;
	m_nSkillDamage = 100;
	nMaxHp = 400;
	nCurHp = 400;
	nMaxMp = 150;
	nCurMp = 150;
	bAttackAction = false;
	bAttack = false;
	bMove = FALSE;
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pSkinnedMesh = new cSkinnedMeshEX;
	m_pSkinnedMesh->Setup("Resource/XFile/Enemy", "Resource/XFile/Enemy/mount.X");
	m_pSkinnedMesh->SetAnimationSetBlend(0, nIdleAni, true);
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pPhysicsBody->GetPhysicsData().vPos = vPosSetup;
	m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
	vPos = vPosSetup;
	bDead = false;
	bIdle = true;
	bDeadbody = false;
	fDeadCount = 0.f;
	//for (int i = 0; i < 5; i++)
	//{
	//	tp[i].start = false;
	//	tp[i].goal= false;
	//	tp[i].check = false;
	//	tp[i].prev = 0;
	//	tp[i].curCost = 100000000;
	//	tp[i].bmove = false;
	//	if (i == 0)
	//	{
	//		tp[0].start = true;
	//		tp[i].next[0] = 1;
	//		tp[i].next[1] = 2;
	//		tp[i].next[2] = 3;
	//		tp[0].curCost = 0;
	//		tp[0].cost = 3;
	//	}
	//	else if (i >= 1 && i < 4)
	//	{
	//		tp[i].next[0] = 4;
	//		tp[i].next[1] = 4;
	//		tp[i].next[2] = 4;
	//	}
	//	else if (i == 4)
	//	{
	//		tp[4].goal = true;
	//	}
	//}
	//
	//tp[0].p = vPosSetup;
	//tp[1].p = D3DXVECTOR3(-2, 0, 2);
	//tp[2].p = D3DXVECTOR3(2, 0, 2);
	//tp[3].p = D3DXVECTOR3(-1, 0, 0);
	//tp[4].p = D3DXVECTOR3(0, 0, 5);
	//tp[1].cost = 3;
	//tp[2].cost = 8;
	//tp[3].cost = 6;
	//tp[4].cost = 5;
	bPhase1 = true;
	bPhase2 = false;
	bPhase3 = false;
	bskillEffect = false;
	EnemyState = IDLE;

	m_pParticle = new cGameParticleShockwave;
	m_pParticle->Setup("");
	g_pParticleManager->AddParticle(m_pParticle);
}

void cBoss::Update(float fDelta)
{
	if (m_pTransformData->GetPosition() != nullptr)
	{
		switch (EnemyState)
		{
		case IDLE:
		{
			Idle();
		}
		break;
		case MOVE:
		{
			//for (int i = 0; i < 5; i++)
			//{
			//	if (tp[i].start)
			//	{
			//		Dijkstra(i, fDelta);
			//	}
			//	if (tp[i].goal)
			//	{
			//		find(i, fDelta);
			//	}
			//}
		}
		break;
		case ATTACK:
		{
			Attack(fDelta);
		}
		break;
		case DEAD:
		{
			Dead();
		}
		case SKILL:
		{

		}
		break;
		case HIT:
		{
			Hit();
		}
		break;
		case JUMP_START:
		{

		}
		break;
		case JUMP:
		{
			Jump();
		}
		break;
		case JUMP_END:
		{
			Jump();
		}
		break;
		}
		if (EnemyState != DEAD)
		{
			float length;
			vPlayerPos = D3DXVECTOR3(m_pPlayer->GetTransformData()->GetPosition());
			vPos = m_pTransformData->GetPosition();
			length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - vPlayerPos));
			if (bAttack == true)
			{

			}
			else
			{
				if (length > 6.f)
				{
					//EnemyState = MOVE;
					EnemyState = IDLE;
				}
				else if (length <= 6.f)
				{
					EnemyState = ATTACK;
				}
				if (nCurHp < 30)
				{
					int a = nCurHp;
				}
			}
			HpManager();
		}

		if (m_pTransformData->GetPosition().y <= -1)
		{
			EnemyState = DEAD;
		}
		if (!bDeadbody)
		{
			m_pSkinnedMesh->Update(fDelta);
		}
		else if (bDeadbody)
		{
			fDeadCount += fDelta;
		}
		cGameNode::Update(fDelta);
	}
}

void cBoss::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());

	m_pSkinnedMesh->Render();
	

	cGameNode::Render();
}

void cBoss::Delete()
{
	SAFE_DELETE(m_pSkinnedMesh);

	g_pParticleManager->DeleteParticle(m_pParticle);
	SAFE_DELETE(m_pParticle);
}

void cBoss::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void cBoss::Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum)
{
	D3DXVECTOR3 vTempDir = vGoal - vPos;
	float tempLength = D3DXVec3Length(&vTempDir);
	if (!bMove)
	{
		if (tempLength > 0.1)
		{
			bMove = TRUE;
			m_pSkinnedMesh->SetAnimationSetBlend(0, nMoveAni, true);
		}
	}
	else
	{
		if (tempLength > 0.1)
		{
			D3DXVec3Normalize(&vDir, &vTempDir);

			float tempRot;
			tempRot = atan((-1 * vTempDir.z) / vTempDir.x);
			if (vTempDir.x < 0) tempRot -= D3DX_PI;

			tempRot = tempRot - (D3DX_PI / 2);
			m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
			m_pPhysicsBody->GetPhysicsData().fRotAngle = tempRot;

			if (m_pPhysicsBody->GetPhysicsData().bOnGround == true)
			{
				float dot = D3DXVec2Dot(&D3DXVECTOR2(
					m_pPhysicsBody->GetPhysicsData().vVelocity.x,
					m_pPhysicsBody->GetPhysicsData().vVelocity.z), &D3DXVECTOR2(vDir.x, vDir.z));

				if (dot < 2)
					m_pPhysicsBody->GetPhysicsData().vAccel
					= vDir * (30.f + D3DXVec3Length(&m_pPhysicsBody->GetPhysicsData().vDamping));
				else
					m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			}
			
		}
		else
		{
			vPos = vGoal;
			bMove = FALSE;
			m_pSkinnedMesh->SetAnimationSetBlend(0, nIdleAni, true);
			tp[dijkNum].bmove = false;
			tp[dijkNum].check = true;
		}
	}
}

void cBoss::Idle()
{
	if (!bIdle)
	{
		m_pSkinnedMesh->SetAnimationSetBlend(0, nIdleAni, true);

		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);

		bIdle = true;
		bMove = false;
	}
}

void cBoss::Move(D3DXVECTOR3 vGoal, float fDelta)
{
	D3DXVECTOR3 vTempDir = vGoal - vPos;
	vTempDir = D3DXVECTOR3(vTempDir.x, 0, vTempDir.z);
	float tempLength = D3DXVec3Length(&vTempDir);
	if (!bMove)
	{
		if (tempLength > 0.1)
		{
			bMove = true;
			if (!bPhase3)
			{
				m_pSkinnedMesh->SetAnimationSetBlend(0, nMoveAni, true);
			}
			else if (bPhase3)
			{
				m_pSkinnedMesh->SetAnimationSetBlend(0, nAttackMoveAni, true);
			}
			bIdle = false;
		}
	}
	else
	{
		if (tempLength > 0.1)
		{
			D3DXVec3Normalize(&vDir, &vTempDir);

			float tempRot;
			tempRot = atan((-1 * vTempDir.z) / vTempDir.x);
			if (vTempDir.x < 0) tempRot -= D3DX_PI;

			tempRot = tempRot - (D3DX_PI / 2);
			m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
			m_pPhysicsBody->GetPhysicsData().fRotAngle = tempRot;

			if (m_pPhysicsBody->GetPhysicsData().bOnGround == true)
			{
				float dot = D3DXVec2Dot(&D3DXVECTOR2(
					m_pPhysicsBody->GetPhysicsData().vVelocity.x,
					m_pPhysicsBody->GetPhysicsData().vVelocity.z), &D3DXVECTOR2(vDir.x, vDir.z));
				if (!bPhase3)
				{
					if (dot < 1.7)
						m_pPhysicsBody->GetPhysicsData().vAccel
						= vDir * (30.f + D3DXVec3Length(&m_pPhysicsBody->GetPhysicsData().vDamping));

					else
						m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
				}
				else if (bPhase3)
				{
					if (dot < 2.7)
						m_pPhysicsBody->GetPhysicsData().vAccel
						= vDir * (30.f + D3DXVec3Length(&m_pPhysicsBody->GetPhysicsData().vDamping));

					else
						m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
				}
			}
			else
			{
				m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			}

		}
	}
}

void cBoss::Attack(float fDelta)
{
	float length;
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - vPlayerPos));

	D3DXVECTOR3 vDist = m_pPlayer->GetTransformData()->GetPosition() - m_pTransformData->GetPosition();

	D3DXVec3Normalize(&vDist, &vDist);
	D3DXVec3Normalize(&vDir, &vDir);
	float fCos = D3DXVec3Dot(&vDist, &vDir);

	if (!bAttack)
	{
		if (length > 2.f)
		{
			Move(vPlayerPos, fDelta);
			bAttack = false;
		}

		else if (length <= 2.f && m_pPhysicsBody->GetPhysicsData().bOnGround == true)
		{
			D3DXVECTOR3 vTempDir;
			vTempDir = vPlayerPos - vPos;
			vTempDir = D3DXVECTOR3(vTempDir.x, 0, vTempDir.z);
			D3DXVec3Normalize(&vDir, &vTempDir);

			float tempRot;
			tempRot = atan((-1 * vTempDir.z) / vTempDir.x);
			if (vTempDir.x < 0) tempRot -= D3DX_PI;

			tempRot = tempRot - (D3DX_PI / 2);
			GetTransformData()->SetAxis(D3DXVECTOR3(0, 1, 0));
			GetTransformData()->SetRotAngle(tempRot);
			bMove = false;

			m_pPhysicsBody->GetPhysicsData().vVelocity.x = 0;
			m_pPhysicsBody->GetPhysicsData().vVelocity.z = 0;
			m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			if (!bPhase2)
			{
				m_pSkinnedMesh->SetAnimationSetBlend(0, nAttackAni, false);
				g_pSoundManager->Play("MountAttack");
				bskillEffect = true;
			}
			else if (!bPhase3 && nCurMp >= 30)
			{
				m_pSkinnedMesh->SetAnimationSetBlend(0, nSkillAni, false);
				g_pSoundManager->Play("MountSkill");
				bskillEffect = false;
			}
			else if (!bPhase3 && nCurMp < 30)
			{
				m_pSkinnedMesh->SetAnimationSetBlend(0, nAttackAni, false);
				g_pSoundManager->Play("MountAttack");
				bskillEffect = true;
			}
			else if (bPhase3)
			{
				m_pSkinnedMesh->SetAnimationSetBlend(0, nSkillAni, false);
				g_pSoundManager->Play("MountSkill");
				bskillEffect = false;
			}
			bAttack = true;
		}
	}
	if (bAttack)
	{

		if (m_pSkinnedMesh->GetAniEnd() == true)
		{
			if (!bPhase2)
			{
				if (length <= 2.3f &&  fCos > cosf(D3DX_PI / 4.f))
				{
					m_pPlayer->PlayerDamaged(m_nAtkDamage);
				}
			}
			else if (!bPhase3 && nCurMp < 30)
			{
				if (length <= 2.3f &&  fCos > cosf(D3DX_PI / 4.f))
				{
					m_pPlayer->PlayerDamaged(m_nAtkDamage);
				}
			}
			bAttack = false;
		}

		else if (!bskillEffect)
		{
			if (m_pSkinnedMesh->getCurPosition() >= 0.6)
			{
				if (!bPhase3 && nCurMp >= 30)
				{
					if (length <= 3.f)
					{
						m_pPlayer->PlayerDamaged(m_nSkillDamage);
					}

					nCurMp -= 30;
					g_pSoundManager->Play("ShockWave");
					bskillEffect = true;
					shockwave();
				}

				else if (bPhase3)
				{
					if (length <= 3.f)
					{
						m_pPlayer->PlayerDamaged(m_nSkillDamage);
					}
					bskillEffect = true;
					g_pSoundManager->Play("ShockWave");
					shockwave();
				}
			}

		}
	}
}

void cBoss::Dead()
{
	if (!bDead)
	{
		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_pSkinnedMesh->SetAnimationSetBlend(0, nDeadAni, false);

		g_pSoundManager->Play("MountDead");
		sendGold();
		m_pPhysicsBody->GetBodyType() = PHYSICSBODYTYPE_NOCHECK;
		bDead = true;
	}
	if (m_pSkinnedMesh->GetAniEnd() == true)
	{
		bDeadbody = true;
		//bAttackAction = true;
	}
}

void cBoss::Jump()
{
	if (m_pPhysicsBody->GetPhysicsData().bOnGround == true)
	{
		m_pPhysicsBody->GetPhysicsData().vVelocity.y = 5.f;
		m_pSkinnedMesh->SetAnimationSetBlend(0, nJumpStartAni, false);
	}
	if (EnemyState == JUMP_START && m_pSkinnedMesh->GetAniEnd() == true)
	{
		EnemyState = JUMP;
		m_pSkinnedMesh->SetAnimationSetBlend(0, nJumpAni, false);
	}
	else if (EnemyState == JUMP && m_pPhysicsBody->GetPhysicsData().bOnGround == true)
	{
		EnemyState = JUMP_END;
		m_pSkinnedMesh->SetAnimationSetBlend(0, nJumpEndAni, false);
	}
	else if (EnemyState == JUMP_END && m_pSkinnedMesh->GetAniEnd() == true)
	{
		EnemyState = IDLE;
	}
}

void cBoss::Dijkstra(int temp, float fDelta)
{
	for (int i = 0; i < 3; i++)
	{
		if (tp[tp[temp].next[i]].curCost > tp[temp].curCost + tp[tp[temp].next[i]].cost)
		{
			tp[tp[temp].next[i]].curCost = tp[temp].curCost + tp[tp[temp].next[i]].cost;
			tp[tp[temp].next[i]].prev = temp;
			if (tp[tp[temp].next[i]].goal == false)
			{
				Dijkstra(tp[temp].next[i], fDelta);

			}
		}
	}
}

void cBoss::find(int tpNum, float fDelta)
{
	if (tp[tp[tpNum].prev].start && !tp[tp[tpNum].prev].check)
	{
		tp[tp[tpNum].prev].bmove = true;
		Move(tp[tpNum].p, fDelta, tpNum);
	}
	else if (tp[tp[tpNum].prev].check)
	{
		tp[tpNum].bmove = true;
		Move(tp[tpNum].p, fDelta, tpNum);
	}
	else if (!tp[tp[tpNum].prev].check)
	{
		find(tp[tpNum].prev, fDelta);
	}
}

void cBoss::setPlayer(cPlayer* pSetPlayer)
{
	m_pPlayer = pSetPlayer;
}

void cBoss::getDamage(int nDamage)
{
	nCurHp -= nDamage;
}

void cBoss::HpManager()
{
	if (nCurHp > nMaxHp) nCurHp = nMaxHp;
	if (nCurHp <= 200 && bPhase2 == false)
	{
		EnemyState = HIT;
		m_pSkinnedMesh->SetAnimationSetBlend(0, nHitAni, false);
		bPhase2 = true;
	}
	else if (nCurHp <= 100 && bPhase3 == false)
	{
		EnemyState = HIT;
		m_pSkinnedMesh->SetAnimationSetBlend(0, nHitAni, false);
		bPhase3 = true;
	}
	if (nCurHp <= 0)
	{
		nCurHp = 0;
		EnemyState = DEAD;
	}
}

void cBoss::Hit()
{
	m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
	if (m_pSkinnedMesh->GetAniEnd() == true)
	{
		EnemyState = IDLE;
		bAttack = false;
	}
}

void cBoss::sendGold()
{
	m_pPlayer->SetPlayerGold(m_pPlayer->GetPlayerGold() + m_nGold);
}

void cBoss::shockwave()
{
	D3DXVECTOR3 vMakePos(0, 0, 0);
	D3DXVec3TransformCoord(&vMakePos, &vMakePos, &this->GetMatrixToWorld());

	m_pParticle->MakeShockWave(vMakePos, 5,200);
}

