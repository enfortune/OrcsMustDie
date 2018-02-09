#include "stdafx.h"
#include "cEnemy.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPlayer.h"
#include "cPhysicsBody.h"
#include "GraphFindPath.h"
#include "Trap.h"
#include "cSubject.h"


cEnemy::cEnemy()
{
}


cEnemy::~cEnemy()
{
	Delete();
}

void cEnemy::Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup)
{
	cGameNode::Setup(true);

	m_bBoss = false;

	nDijkNum = 0;

	m_nGold = 30;

	nIdleAni = 11;
	nAttackAni = 2;
	nDeadAni = 9;
	nMoveAni = 0;
	nAttackMoveAni = 10;
	nJumpStartAni = 7;
	nJumpAni = 5;
	nJumpEndAni = 6;
	nHitAni = 3;
	nSkillAni = 1;

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(0.0f);
	m_pPhysicsBody->MakeBodyCuboid(0.6, 1.f, 0.5,D3DXVECTOR3(0, 0.5, 0));
	m_pPhysicsBody->GetPhysicsData().fElasticity = 0.f;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(10.f, 0.f, 10.f);
	m_pPhysicsBody->GetBodyType() = PHYSICSBODYTYPE_DINAMIC;

	m_nAtkDamage = 7;
	nMaxHp = 40;
	nCurHp = 40;
	bAttackAction = false;
	bAttack = false;
	bMove = FALSE;
	bAttackMove = false;
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pSkinnedMesh = new cSkinnedMeshEX;
	m_pSkinnedMesh->Setup("Resource/XFile/Enemy", "Resource/XFile/Enemy/Orc_Sword.X");
	m_pSkinnedMesh->SetAnimationSet(0, nIdleAni, true);
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pPhysicsBody->GetPhysicsData().vPos = vPosSetup;
	m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
	vPos = vPosSetup;
	bDead = false;
	bIdle = true;
	bDeadbody = false;
	bAttackTrap = false;
	fDeadCount = 0.f;
	fFindTime = 0;
	bFinish = false;

	m_pFindPath->findPath(0, 4, &m_vPath);
	EnemyState = MOVE;
}

void cEnemy::Update(float fDelta)
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
			case SKILL:
			{

			}
			break;
			case HIT:
			{

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
				if (length <= 3.f)
				{
					EnemyState = ATTACK;
				}

				else if (length > 3.f)
				{
					EnemyState = MOVE;
					//EnemyState = IDLE;
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


		fFindTime += fDelta;

		cGameNode::Update(fDelta);
	}
}

void cEnemy::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());

	m_pSkinnedMesh->Render();
	

	cGameNode::Render();
}

void cEnemy::Delete()
{
	SAFE_DELETE(m_pSkinnedMesh);

}

void cEnemy::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void cEnemy::Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum)
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
		if (tempLength > 0.3)
		{
			Findbarricade(fDelta, false);
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
			//m_pSkinnedMesh->SetAnimationSetBlend(0, nIdleAni, true);
			nDijkNum++;
			if (nDijkNum >= 4)
			{
				nDijkNum = 4;
				bFinish = true;
				EnemyState = FINISH;
			}
			//tp[dijkNum].bmove = false;
			//tp[dijkNum].check = true;
		}
	}
}

void cEnemy::Idle()
{
	if (!bIdle)
	{
		m_pSkinnedMesh->SetAnimationSet(0, nIdleAni, true);

		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);

		bIdle = true;
		bMove = false;
	}
}

void cEnemy::Move(D3DXVECTOR3 vGoal, float fDelta)
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
			bAttackMove = TRUE;
			m_pSkinnedMesh->SetAnimationSetBlend(0, nAttackMoveAni, true);
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

				if (dot < 2)
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
	}
}

void cEnemy::Attack(float fDelta)
{
	float length;
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - vPlayerPos));

	D3DXVECTOR3 vDist = m_pPlayer->GetTransformData()->GetPosition() - m_pTransformData->GetPosition();

	D3DXVec3Normalize(&vDist, &vDist);
	D3DXVec3Normalize(&vDir, &vDir);
	float fCos = D3DXVec3Dot(&vDist, &vDir);

	if (!bAttack)
	{
		if (length > 0.7)
		{
			Move(vPlayerPos, fDelta);
			bAttack = false;
		}

		else if (length <= 0.7 && m_pPhysicsBody->GetPhysicsData().bOnGround == true)
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

		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
		if (m_pSkinnedMesh->GetAniEnd() == true)
		{
			if (length <= 1.3 &&  fCos > cosf(D3DX_PI / 4.f))
			{
				m_pPlayer->PlayerDamaged(m_nAtkDamage);
				g_pSoundManager->Play("OrcAttack");
			}
			bAttack = false;
			bMove = false;
			bAttackMove = false;
		}

	}
}

void cEnemy::Attackbarricade(float fDelta)
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
				if (tempLength <= 0.7f)
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

		//D3DXVec3Normalize(&vDist, &vDist);
		//D3DXVec3Normalize(&vDir, &vDir);
		//float fCos = D3DXVec3Dot(&vDist, &vDir);
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

				if (tempLength <= 0.7f)
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
				else if (tempLength >= 0.7f)
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

void cEnemy::AttackAnduin(float fDelta)
{
	float length;
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - m_pAnduin->GetAPos()));

	D3DXVECTOR3 vDist = m_pAnduin->GetAPos() - m_pTransformData->GetPosition();

	D3DXVec3Normalize(&vDist, &vDist);
	D3DXVec3Normalize(&vDir, &vDir);
	float fCos = D3DXVec3Dot(&vDist, &vDir);

	if (!bAttack)
	{
		if (length > 0.7)
		{
			Move(m_pAnduin->GetAPos(), fDelta);
			bAttack = false;
		}

		else if (length <= 0.7 && m_pPhysicsBody->GetPhysicsData().bOnGround == true)
		{
			D3DXVECTOR3 vTempDir;
			vTempDir = m_pAnduin->GetAPos() - vPos;
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

		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
		if (m_pSkinnedMesh->GetAniEnd() == true)
		{
			if (length <= 0.7 &&  fCos > cosf(D3DX_PI / 4.f))
			{
				m_pAnduin->SetAnduinHp(m_pAnduin->GetAnduinHp() - m_nAtkDamage);
				g_pSoundManager->Play("OrcAttack");
				if (m_pAnduin->GetAnduinHp() <= 0)
				{
					int a;
					a = m_pAnduin->GetAnduinHp();
				}
			}
			bAttack = false;
			bMove = false;
			bAttackMove = false;
		}

	}
}

int cEnemy::Findbarricade(float fDelta, bool bPass)
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
				if (tempLength <= 0.7f)
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

void cEnemy::Dead()
{
	if (!bDead)
	{
		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_pSkinnedMesh->SetAnimationSetBlend(0, nDeadAni, false);

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

void cEnemy::Jump()
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

void cEnemy::find(int tpNum, float fDelta)
{
	Move(m_vPath[nDijkNum], fDelta, nDijkNum);
}

void cEnemy::setPlayer(cPlayer* pSetPlayer)
{
	m_pPlayer = pSetPlayer;
}

void cEnemy::setPath(GraphFindPath * pFindPath)
{
	m_pFindPath = pFindPath;
}

void cEnemy::setTrap(std::vector<Trap> & vTrap)
{
	m_vTrap = & vTrap;
}

void cEnemy::setAnduin(cSubject * Anduin)
{
	m_pAnduin = Anduin;
}

void cEnemy::getDamage(int nDamage)
{
	nCurHp -= nDamage;
}

void cEnemy::HpManager()
{
	if (nCurHp > nMaxHp) nCurHp = nMaxHp;
	if (nCurHp <= 0)
	{
		nCurHp = 0;
		EnemyState = DEAD;
	}
}

void cEnemy::sendGold()
{
	m_pPlayer->SetPlayerGold(m_pPlayer->GetPlayerGold() + m_nGold);
}

