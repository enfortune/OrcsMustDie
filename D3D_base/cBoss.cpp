#include "stdafx.h"
#include "cBoss.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPlayer.h"
#include "cPhysicsBody.h"
#include "cGameParticleShockwave.h"
#include "GraphFindPath.h"
#include "Trap.h"
#include "cSubject.h"

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
	nDijkNum = 0;
	m_nAtkDamage = 50;
	m_nSkillDamage = 100;
	nMaxHp = 400;
	nCurHp = 400;
	nMaxMp = 150;
	nCurMp = 150;
	bAttackAction = false;
	bAttack = false;
	bMove = FALSE;
	bAttackMove = false;

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
	bPhase1 = true;
	bPhase2 = false;
	bPhase3 = false;
	bskillEffect = false;
	bAttackTrap = false;
	fDeadCount = 0.f;
	bFinish = false;

	m_pFindPath->findPath(0, 4, &m_vPath);

	EnemyState = MOVE;

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
			find(nDijkNum, fDelta);
		}
		break;
		case ATTACK:
		{
			Attack(fDelta);
		}
		break;
		case ATTACK_TRAP:
		{
			Attackbarricade(fDelta);
		}
		break;
		case FINISH:
		{
			AttackAnduin(fDelta);
		}
		break;
		case DEAD:
		{
			Dead();
		}
		break;
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
			if (bFinish)
			{

			}
			else if (bAttack == true)
			{

			}
			else if (bAttackTrap == true)
			{

			}
			else
			{
				if (length > 6.f)
				{
					EnemyState = MOVE;
					//EnemyState = IDLE;
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
			m_pSkinnedMesh->Update(0);
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
	if (bAttackMove)
	{
		bMove = false;
		bAttackMove = false;
		bAttackTrap = false;
	}
	if (!bMove)
	{
		if (tempLength > 0.2)
		{
			bMove = TRUE;
			bIdle = false;
			m_pSkinnedMesh->SetAnimationSetBlend(0, nMoveAni, true);
		}
	}
	else
	{
		if (tempLength > 0.2)
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

				if (dot < 1.5)
					m_pPhysicsBody->GetPhysicsData().vAccel
					= vDir * (30.f + D3DXVec3Length(&m_pPhysicsBody->GetPhysicsData().vDamping));
				else
					m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			}
			else
			{
				m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			}
		}
		else if (tempLength <= 1.f)
		{
			bMove = FALSE;
			nDijkNum++;
			if (nDijkNum >= 4)
			{
				nDijkNum = 4;
				bFinish = true;
				EnemyState = FINISH;
			}
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
	if (bMove)
	{
		bAttackTrap = false;
		bMove = false;
		bAttackMove = false;
	}
	if (!bAttackMove)
	{
		if (tempLength > 0.1)
		{
			bAttackMove = true;
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
			bMove = false;
			bAttackMove = false;
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

				bMove = false;
				bAttackMove = false;
			}

		}
	}
}

void cBoss::Attackbarricade(float fDelta)
{
	int tempNum;
	tempNum = 0;
	if ((*m_vTrap).size() != 0)
	{
		for (int i = 0; i < (*m_vTrap).size(); i++)
		{
			if ((*m_vTrap)[i].isBlockable())
			{
				float tempLength;
				tempLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vTrap)[i].getFrustumCenter()));
				tempLength -= (*m_vTrap)[i].getFrustumMaxLength() / 2;
				if (tempLength <= 1.3f)
				{
					tempNum = i;
				}
			}
		}
		D3DXVECTOR3 vDist = (*m_vTrap)[tempNum].getFrustumCenter() - m_pTransformData->GetPosition();

		D3DXVECTOR3 vTempDir = (*m_vTrap)[tempNum].getFrustumCenter() - vPos;
		D3DXVec3Normalize(&vDir, &vTempDir);

		float tempRot;
		tempRot = atan((-1 * vTempDir.z) / vTempDir.x);
		if (vTempDir.x < 0) tempRot -= D3DX_PI;

		tempRot = tempRot - (D3DX_PI / 2);
		m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
		m_pPhysicsBody->GetPhysicsData().fRotAngle = tempRot;
	}

	else if ((*m_vTrap).size() == 0)
	{
		bAttack = false;
		bMove = false;
		EnemyState = MOVE;
	}

	if (!bAttack)
	{
		if (m_pPhysicsBody->GetPhysicsData().bOnGround == true)
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
			m_pSkinnedMesh->SetAnimationSetBlend(0, nAttackAni, false);
			bAttack = true;
		}
	}
	if (bAttack)
	{

		if (m_pSkinnedMesh->GetAniEnd() == true)
		{
			//if ((*m_vTrap)[tempNum].isBlockable() == true)
			//{
			//	(*m_vTrap)[tempNum].onHit(m_nAtkDamage);
			//	g_pSoundManager->Play("OrcAttack");
			//	bAttack = false;
			//	EnemyState = MOVE;
			//}
			//else if ((*m_vTrap)[tempNum].isBlockable() == false)
			//{
			//	bAttack = false;
			//	EnemyState = MOVE;
			//}
			for (int i = 0; i < (*m_vTrap).size(); i++)
			{
				float tempLength;
				tempLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vTrap)[i].getFrustumCenter()));
				tempLength -= (*m_vTrap)[i].getFrustumMaxLength() / 2;

				if (tempLength < 1.3f)
				{
					if ((*m_vTrap)[i].isBlockable())
					{
						(*m_vTrap)[i].onHit(m_nAtkDamage);
						g_pSoundManager->Play("OrcAttack");
						bAttack = false;
						bMove = false;
						bAttackTrap = false;
						EnemyState = MOVE;
						nTrapNum = 0;
					}
					else if ((*m_vTrap)[i].isBlockable())
					{
						bAttack = false;
						bMove = false;
						bAttackTrap = false;
						EnemyState = MOVE;
						nTrapNum = 0;
					}
				}
				else if (tempLength >= 1.3f)
				{
					bAttack = false;
					bMove = false;
					bAttackTrap = false;
					EnemyState = MOVE;
				}
			}
		}

	}
}

void cBoss::AttackAnduin(float fDelta)
{
	float length;
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - m_pAnduin->GetAPos()));

	D3DXVECTOR3 vDist = m_pAnduin->GetAPos() - m_pTransformData->GetPosition();

	D3DXVec3Normalize(&vDist, &vDist);
	D3DXVec3Normalize(&vDir, &vDir);
	float fCos = D3DXVec3Dot(&vDist, &vDir);

	if (!bAttack)
	{
		if (length > 2.f)
		{
			Move(m_pAnduin->GetAPos(), fDelta);
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
					m_pAnduin->SetAnduinHp(m_pAnduin->GetAnduinHp() - m_nAtkDamage);
				}
			}
			else if (!bPhase3 && nCurMp < 30)
			{
				if (length <= 2.3f &&  fCos > cosf(D3DX_PI / 4.f))
				{
					m_pAnduin->SetAnduinHp(m_pAnduin->GetAnduinHp() - m_nAtkDamage);
				}
			}
			bAttack = false;
			bMove = false;
			bAttackMove = false;
		}

		else if (!bskillEffect)
		{
			if (m_pSkinnedMesh->getCurPosition() >= 0.6)
			{
				if (!bPhase3 && nCurMp >= 30)
				{
					if (length <= 3.f)
					{
						m_pAnduin->SetAnduinHp(m_pAnduin->GetAnduinHp() - m_nAtkDamage);;
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
						m_pAnduin->SetAnduinHp(m_pAnduin->GetAnduinHp() - m_nAtkDamage);;
					}
					bskillEffect = true;
					g_pSoundManager->Play("ShockWave");
					shockwave();
				}

				bMove = false;
				bAttackMove = false;
			}

		}
	}
}

int cBoss::Findbarricade(float fDelta, bool bPass)
{
	for (int i = 0; i < (*m_vTrap).size(); i++)
	{
		if (bPass == false)
		{
			if ((*m_vTrap)[i].isBlockable())
			{
				float tempLength;
				tempLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vTrap)[i].getFrustumCenter()));
				tempLength -= (*m_vTrap)[i].getFrustumMaxLength() / 2;
				if (tempLength <= 1.3f)
				{
					nTrapNum = i;
					EnemyState = ATTACK_TRAP;
					bMove = false;
					bAttack = false;
					bAttackTrap = true;
					return 0;
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


void cBoss::find(int tpNum, float fDelta)
{
	Move(m_vPath[nDijkNum], fDelta, nDijkNum);
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

void cBoss::setPath(GraphFindPath * pFindPath)
{
	m_pFindPath = pFindPath;
}

void cBoss::setTrap(std::vector<Trap>& vTrap)
{
	m_vTrap = &vTrap;
}

void cBoss::setAnduin(cSubject * Anduin)
{
	m_pAnduin = Anduin;
}

void cBoss::shockwave()
{
	D3DXVECTOR3 vMakePos(0, 0, 0);
	D3DXVec3TransformCoord(&vMakePos, &vMakePos, &this->GetMatrixToWorld());

	m_pParticle->MakeShockWave(vMakePos, 5,200);
}

