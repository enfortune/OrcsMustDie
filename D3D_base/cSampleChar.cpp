#include "stdafx.h"
#include "cSampleChar.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPlayer.h"
#include "cPhysicsBody.h"

cSampleChar::cSampleChar()
{
}


cSampleChar::~cSampleChar()
{
	Delete();
}

void cSampleChar::Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup)
{
	cGameNode::Setup(true);

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(0.0f);
	m_pPhysicsBody->MakeBodyCuboid(0.7, 1.f, 0.5,D3DXVECTOR3(0, 0.5, 0));
	m_pPhysicsBody->GetPhysicsData().fElasticity = 0.f;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(10.f, 0.f, 10.f);
	m_pPhysicsBody->GetBodyType() = PHYSICSBODYTYPE_DINAMIC;

	m_nAtkDamage = 20;
	nMaxHp = 50;
	nCurHp = 50;
	bAttackAction = false;

	bMove = FALSE;
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pSkinnedMesh = new cSkinnedMeshEX;
	m_pSkinnedMesh->Setup("XFileSample", "XFileSample/chockchock.X");
	m_pSkinnedMesh->SetAnimationSet(0, 3, true);
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pPhysicsBody->GetPhysicsData().vPos = vPosSetup;
	m_pPhysicsBody->GetPhysicsData().vAxis = D3DXVECTOR3(0, 1, 0);
	vPos = vPosSetup;



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

	EnemyState = IDLE;
}

void cSampleChar::Update(float fDelta)
{
	switch (EnemyState)
	{
	case IDLE:
	{

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
	}
	float length;
	vPlayerPos = D3DXVECTOR3(m_pPlayer->GetTransformData()->GetPosition());
	vPos = m_pTransformData->GetPosition();
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - vPlayerPos));
	if (length > 3.f)
	{
		EnemyState = MOVE;
	}
	else if (length <= 3.f)
	{
		EnemyState = ATTACK;
	}
	if (nCurHp < 30)
	{
		int a = nCurHp;
	}
	m_pSkinnedMesh->Update();
	m_pSkinnedMesh->UpdateAnimation(fDelta);
	cGameNode::Update(fDelta);
}

void cSampleChar::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());

	m_pSkinnedMesh->Render();
	

	cGameNode::Render();
}

void cSampleChar::Delete()
{
	SAFE_DELETE(m_pSkinnedMesh);

}

void cSampleChar::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void cSampleChar::Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum)
{
	D3DXVECTOR3 vTempDir = vGoal - vPos;
	float tempLength = D3DXVec3Length(&vTempDir);
	if (!bMove)
	{
		if (tempLength > 0.1)
		{
			bMove = TRUE;
			m_pSkinnedMesh->SetAnimationSet(0, 2, true);
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
			vPos = vGoal;
			bMove = FALSE;
			m_pSkinnedMesh->SetAnimationSet(0, 3, true);
			tp[dijkNum].bmove = false;
			tp[dijkNum].check = true;
		}
	}
}

void cSampleChar::Move(D3DXVECTOR3 vGoal, float fDelta)
{
	D3DXVECTOR3 vTempDir = vGoal - vPos;
	float tempLength = D3DXVec3Length(&vTempDir);
	if (!bMove)
	{
		if (tempLength > 0.1)
		{
			bMove = TRUE;
			m_pSkinnedMesh->SetAnimationSet(0, 2, true);
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
			vPos = vGoal;
			bMove = FALSE;
			m_pSkinnedMesh->SetAnimationSet(0, 3, true);
		}
	}
}

void cSampleChar::Attack(float fDelta)
{
	float length;
	length = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - vPlayerPos));

	if (length > 0.7)
	{
		Move(vPlayerPos, fDelta);
		bAttack = false;
	}
	else if (length <= 0.7)
	{
		if (bAttack == false)
		{

			m_pPhysicsBody->GetPhysicsData().vVelocity.x = 0;
			m_pPhysicsBody->GetPhysicsData().vVelocity.z = 0;
			m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			m_pSkinnedMesh->SetAnimationSetBlend(0, 0, false);
			bAttack = true;
		}
		else if (bAttackAction)
		{
			m_pPhysicsBody->GetPhysicsData().vVelocity.x = 0;
			m_pPhysicsBody->GetPhysicsData().vVelocity.z = 0;
			m_pPhysicsBody->GetPhysicsData().vAccel = D3DXVECTOR3(0.f, 0.f, 0.f);
			m_pSkinnedMesh->SetAnimationSetBlend(0, 0, false);
			bAttackAction = false;
		}
		D3DXVECTOR3 vTempDir;
		vTempDir = vPlayerPos - vPos;
		D3DXVec3Normalize(&vDir, &vTempDir);

		if (m_pSkinnedMesh->GetAniEnd() == true)
		{
			if (!bAttackAction)
			{
				m_pPlayer->PlayerDamaged(m_nAtkDamage);
				bAttackAction = true;
			}
		}

		float tempRot;
		tempRot = atan((-1 * vTempDir.z) / vTempDir.x);
		if (vTempDir.x < 0) tempRot -= D3DX_PI;

		tempRot = tempRot - (D3DX_PI / 2);
		GetTransformData()->SetAxis(D3DXVECTOR3(0, 1, 0));
		GetTransformData()->SetRotAngle(tempRot);
		bMove = false;
	}
}

void cSampleChar::Dijkstra(int temp, float fDelta)
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

void cSampleChar::find(int tpNum, float fDelta)
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

void cSampleChar::setPlayer(cPlayer* pSetPlayer)
{
	m_pPlayer = pSetPlayer;
}

void cSampleChar::getDamage(int nDamage)
{
	nCurHp -= nDamage;
}

