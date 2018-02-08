#include "stdafx.h"
#include "cEnemy2.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPlayer.h"
#include "cPhysicsBody.h"
#include "GraphFindPath.h"


cEnemy2::cEnemy2()
{
}


cEnemy2::~cEnemy2()
{
	Delete();
}

void cEnemy2::Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup)
{
	cGameNode::Setup(true);

	m_bBoss = false;

	nDijkNum = 0;

	nIdleAni = 9;
	nAttackAni = 4;
	nDeadAni = 7;
	nMoveAni = 5;
	nAttackMoveAni = 8;
	nJumpStartAni = 3;
	nJumpAni = 1;
	nJumpEndAni = 2;
	nHitAni = 3;
	nSkillAni = 0;

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(0.0f);
	m_pPhysicsBody->MakeBodyCuboid(0.6, 0.5, 1.5,D3DXVECTOR3(0, 0.25, 0));
	m_pPhysicsBody->GetPhysicsData().fElasticity = 0.f;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(10.f, 0.f, 10.f);
	m_pPhysicsBody->GetBodyType() = PHYSICSBODYTYPE_DINAMIC;

	m_nAtkDamage = 15;
	nMaxHp = 30;
	nCurHp = 30;
	bAttackAction = false;
	bAttack = false;
	bMove = FALSE;
	bAttackMove = false;
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pSkinnedMesh = new cSkinnedMeshEX;
	m_pSkinnedMesh->Setup("Resource/XFile/Enemy", "Resource/XFile/Enemy/Wolf.X");
	m_pSkinnedMesh->SetAnimationSet(0, nIdleAni, true);
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pPhysicsBody->GetPhysicsData().vPos = vPosSetup;
	m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
	vPos = vPosSetup;
	bDead = false;
	bIdle = true;
	bDeadbody = false;
	fDeadCount = 0.f;

	m_nGold = 50;
	fFindTime = 0;
	m_pFindPath->findPath(0, 4, &m_vPath);

	EnemyState = MOVE;
}

void cEnemy2::Update(float fDelta)
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
			if (bAttack == true)
			{

			}
			else
			{
				if (length > 4.f)
				{
					EnemyState = MOVE;
					//EnemyState = IDLE;
				}
				else if (length <= 4.f)
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

void cEnemy2::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());

	m_pSkinnedMesh->Render();
	

	cGameNode::Render();
}

void cEnemy2::Delete()
{
	SAFE_DELETE(m_pSkinnedMesh);

}

void cEnemy2::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void cEnemy2::Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum)
{
	D3DXVECTOR3 vTempDir = vGoal - vPos;
	float tempLength = D3DXVec3Length(&vTempDir);
	if (bAttackMove)
	{
		bMove = false;
		bAttackMove = false;
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
			}
		}
	}
}

void cEnemy2::Idle()
{
	if (!bIdle)
	{
		m_pSkinnedMesh->SetAnimationSet(0, nIdleAni, true);

		m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);

		bIdle = true;
		bMove = false;
	}
}

void cEnemy2::Move(D3DXVECTOR3 vGoal, float fDelta)
{
	D3DXVECTOR3 vTempDir = vGoal - vPos;
	vTempDir = D3DXVECTOR3(vTempDir.x, 0, vTempDir.z);
	float tempLength = D3DXVec3Length(&vTempDir);

	if (bMove)
	{
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

				if (dot < 3)
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

void cEnemy2::Attack(float fDelta)
{
	float length;
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - vPlayerPos));

	D3DXVECTOR3 vDist = m_pPlayer->GetTransformData()->GetPosition() - m_pTransformData->GetPosition();

	D3DXVec3Normalize(&vDist, &vDist);
	D3DXVec3Normalize(&vDir, &vDir);
	float fCos = D3DXVec3Dot(&vDist, &vDir);

	if (!bAttack)
	{
		if (length > 1.3)
		{
			Move(vPlayerPos, fDelta);
			bAttack = false;
		}

		else if (length <= 1.3 && m_pPhysicsBody->GetPhysicsData().bOnGround == true)
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
			if (length <= 1.3 &&  fCos > cosf(D3DX_PI / 4.f))
			{
				m_pPlayer->PlayerDamaged(m_nAtkDamage);
				g_pSoundManager->Play("WolfBite");
			}
			bAttack = false;
		}

	}
}

void cEnemy2::Dead()
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

void cEnemy2::Jump()
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


void cEnemy2::find(int tpNum, float fDelta)
{
	Move(m_vPath[nDijkNum], fDelta, nDijkNum);
}

void cEnemy2::setPlayer(cPlayer* pSetPlayer)
{
	m_pPlayer = pSetPlayer;
}

void cEnemy2::getDamage(int nDamage)
{
	nCurHp -= nDamage;
}

void cEnemy2::setPath(GraphFindPath * pFindPath)
{
	m_pFindPath = pFindPath;
}

void cEnemy2::HpManager()
{
	if (nCurHp > nMaxHp) nCurHp = nMaxHp;
	if (nCurHp <= 0)
	{
		nCurHp = 0;
		EnemyState = DEAD;
	}
}

void cEnemy2::sendGold()
{
	m_pPlayer->SetPlayerGold(m_pPlayer->GetPlayerGold() + m_nGold);
}

