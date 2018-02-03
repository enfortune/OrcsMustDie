#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMeshEX.h"
#include "cTransformData.h"
#include "cPhysicsBody.h"
#include "cEnemy.h"

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
	, m_vEnemy(NULL)
	, isJump(false)
{
}

cPlayer::~cPlayer()
{
	Delete();
}

void cPlayer::Setup()
{
	cGameNode::Setup();

	m_pPlayerMesh = new cSkinnedMeshEX;
	m_pPlayerMesh->Setup("XFileSample","XFileSample/True_King.X");
	m_pPlayerState = PLAYERSTATE_STAND;
	IsPlayerState();
	m_vPlayerPos = GetTransformData()->GetPosition();
	m_vPlayerDir = D3DXVECTOR3(0, 0, 1);

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(-PI / 4.f);
	m_pPhysicsBody->MakeBodyCuboid(0.4f, 1.f, 0.2f, D3DXVECTOR3(0.f, 0.5f, 0.f));
	m_pPhysicsBody->GetPhysicsData().vPos = m_vPlayerPos;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(20.f, 0.f, 20.f);
	m_pPhysicsBody->SetBodyType(PHYSICSBODYTYPE_DINAMIC);

	nPlayerMaxHp = 100;
	nPlayerCurHp = 100;
	nPlayerMaxMp = 10;
	nPlayerCurMp = 10;
	m_nPlayerGold = 1000;
	m_nPlayerAtkDamage = 10;
}

void cPlayer::Update(float fDelta)
{
	m_pPlayerMesh->Update();
	m_pPlayerMesh->UpdateAnimation(fDelta);

	float speedX = 0.f;
	float speedZ = 0.f;

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

	if (g_pKeyManager->IsStayKeyDown('W') && m_pPlayerState != PLAYERSTATE_ATTACK)
	{
		speedX += 5.f *  m_vPlayerDir.x;
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
	if (g_pKeyManager->IsOnceKeyUp('W'))
	{
		if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
			m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
			&& m_pPlayerState != PLAYERSTATE_JUMPING)
		{
			m_pPlayerState = PLAYERSTATE_STAND;
			IsPlayerState();
		}
	}

	if (g_pKeyManager->IsStayKeyDown('S') && m_pPlayerState != PLAYERSTATE_ATTACK)
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
	if (g_pKeyManager->IsOnceKeyUp('S'))
	{
		if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
			m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
			&& m_pPlayerState != PLAYERSTATE_JUMPING)
		{
			m_pPlayerState = PLAYERSTATE_STAND;
			IsPlayerState();
		}
	}
	if (g_pKeyManager->IsStayKeyDown('A') && m_pPlayerState != PLAYERSTATE_ATTACK)
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
	if (g_pKeyManager->IsOnceKeyUp('A'))
	{
		if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
			m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
			&& m_pPlayerState != PLAYERSTATE_JUMPING)
		{
			m_pPlayerState = PLAYERSTATE_STAND;
			IsPlayerState();
		}
	}

	if (g_pKeyManager->IsStayKeyDown('D') && m_pPlayerState != PLAYERSTATE_ATTACK)
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
	if (g_pKeyManager->IsOnceKeyUp('D'))
	{
		if (m_pPlayerState != PLAYERSTATE_MOVE && m_pPlayerState != PLAYERSTATE_JUMPSTART &&
			m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND && m_pPlayerState != PLAYERSTATE_JUMPEND
			&& m_pPlayerState != PLAYERSTATE_JUMPING)
		{
			m_pPlayerState = PLAYERSTATE_STAND;
			IsPlayerState();
		}
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
	{
		if (m_pPlayerState != PLAYERSTATE_ATTACK)
		{
			m_pPlayerState = PLAYERSTATE_ATTACK;
			IsPlayerState();
		}
		PlayerAttacked();
	}

	float ManaCount = 0;

	if (g_pKeyManager->IsStayKeyDown('R'))
	{
		if (m_pPlayerState != PLAYERSTATE_SKILL_WHIRLWIND)
		{
			m_pPlayerState = PLAYERSTATE_SKILL_WHIRLWIND;
			IsPlayerState();
		}
		PlayerWhirlWind();
		//ManaCount += 0.1 * fDelta;
		//
		//if (m_pPlayerState == PLAYERSTATE_SKILL_WHIRLWIND && ManaCount == 1)
		//{
		//	nPlayerCurMp = nPlayerCurMp - ManaCount; ManaCount = 0;
		//}
	}
	if (g_pKeyManager->IsOnceKeyUp('R'))
	{
		ManaCount = 0;
		m_pPlayerState = PLAYERSTATE_STAND;
	}
	
	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		if (m_pPhysicsBody->GetPhysicsData().bOnGround == true)
		{
			isJump = true;
			m_pPhysicsBody->GetPhysicsData().vVelocity.y = 5.f;
			m_pPlayerState = PLAYERSTATE_JUMPSTART;
			IsPlayerState();
		}
	}
	PlayerJumpBlend();
	

	//회전블랜딩
	if (fDelta)
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

	m_pPhysicsBody->GetPhysicsData().vVelocity.x = speedX;
	m_pPhysicsBody->GetPhysicsData().vVelocity.z = speedZ;

	m_pPhysicsBody->GetPhysicsData().fRotAngle = m_pRotationY;
	//GetTransformData()->SetAxis(D3DXVECTOR3(0, 1, 0));
	//GetTransformData()->SetPosition(m_vPlayerPos);
	
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, m_pRotationY);
	m_vPlayerDir = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vPlayerDir, &m_vPlayerDir, &matR);

	cGameNode::Update(fDelta);
}

void cPlayer::Render()
{
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
	//this->m_pPhysicsBody->GetShapeData().stCuboid.Render(g_pD3DDevice);

	cGameNode::Render();
}

void cPlayer::Delete()
{
	SAFE_DELETE(m_pPlayerMesh);
}

void cPlayer::PlayerDamaged(int damage)
{
	if (damage)
	{
		nPlayerCurHp = nPlayerCurHp - damage;
	}
}

void cPlayer::PlayerHPHealed(int HPheal)
{
	if (HPheal)
	{
		if (nPlayerMaxHp == nPlayerCurHp) return;
		else
		{
			nPlayerCurHp = nPlayerCurHp + HPheal;
		}
	}
}

void cPlayer::playerMPHealed(int MPheal)
{
	if (MPheal)
	{
		if (nPlayerMaxMp == nPlayerCurMp) return;
		else
		{
			nPlayerCurMp = nPlayerCurMp + MPheal;
		}
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
			(*m_vEnemy)[i]->getDamage(m_nPlayerAtkDamage);
		}
	}
}

void cPlayer::PlayerWhirlWind()
{
	for (int i = 0; i < (*m_vEnemy).size(); i++)
	{
		float PlayerLength;
		PlayerLength = D3DXVec3Length(&D3DXVECTOR3(GetTransformData()->GetPosition() - (*m_vEnemy)[i]->GetTransformData()->GetPosition()));
		if (PlayerLength < 0.8)
		{
			(*m_vEnemy)[i]->getDamage(m_nPlayerAtkDamage);
		}
	}
}

void cPlayer::PlayerShiledBash()
{
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
	else if (m_pPlayerState == PLAYERSTATE_JUMPEND && m_pPlayerMesh->GetAniEnd() == true)
	{
		m_pPlayerState = PLAYERSTATE_STAND;
		IsPlayerState();
	}

}

void cPlayer::setEnemy(std::vector<cEnemy*>* Enemy)
{
	m_vEnemy = Enemy;
}

void cPlayer::IsPlayerState()
{
	switch (m_pPlayerState)
	{
	case PLAYERSTATE_STAND:
		m_pPlayerMesh->SetAnimationSetBlend(0, 11, true);
		break;
	case PLAYERSTATE_MOVE:
		m_pPlayerMesh->SetAnimationSet(0, 10, true);
		break;
	case PLAYERSTATE_ATTACK:
		m_pPlayerMesh->SetAnimationSetBlend(0, 4, false);
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
		break;
	default:
		break;
	}

}

void cPlayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
