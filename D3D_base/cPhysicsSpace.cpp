#include "stdafx.h"
#include "cPhysicsSpace.h"
#include "Trap.h"
#include "cPhysicsNode.h"
#include "cPhysicsBody.h"
#include "iGameMap.h"

#define PHYSICSNODE_DELTATIME_MAX 0.1f
#define PHYSICSNODE_DELTATIME_SKIP 0.2f

cPhysicsSpace::cPhysicsSpace()
	: m_pVecTrap(nullptr)
{
}
cPhysicsSpace::~cPhysicsSpace()
{
	this->Delete();
}
void cPhysicsSpace::Setup(iGameMap* pMapData, std::vector<Trap>* pVecTrap)
{
	cPhysicsNode::Setup(pMapData);
	m_pVecTrap = pVecTrap;

}
void cPhysicsSpace::Update(float fDelta)
{

	/* 충돌처리 들어가야 함*/
	// 상호작용
	std::set<cGameNode*>::iterator itSour, itDest;

	float fTotalDelta = fDelta;
	if (fTotalDelta > PHYSICSNODE_DELTATIME_SKIP) fTotalDelta = PHYSICSNODE_DELTATIME_SKIP;
	float fCurrDelta = fDelta;

	while (fTotalDelta > PHYSICSNODE_DELTATIME_MAX)
	{

		fCurrDelta = PHYSICSNODE_DELTATIME_MAX;
		fTotalDelta -= PHYSICSNODE_DELTATIME_MAX;

		for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
		{
			(*itSour)->Update(fCurrDelta);
			(*itSour)->GetPhysicsBody()->GetPhysicsData().vVelocity += m_stSpaceData.vGravity * fCurrDelta;
			(*itSour)->GetPhysicsBody()->GetPhysicsData().bOnGround = false;
			(*itSour)->UpdateTempPhysics(fCurrDelta);
		}

		for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
		{
			itDest = itSour;
			itDest++;
			for (; itDest != m_setChild.end(); itDest++)
			{
				(*itSour)->CollisionWithNode(*itDest);
			}
		}

		for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
		{
			if (m_pVecTrap) this->CollisionWithTrap((*itSour));
			this->CollisionWithMap((*itSour), fCurrDelta);
			(*itSour)->UpdatePhysics(fCurrDelta);
		}
	}

	fCurrDelta = fTotalDelta;

	for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
	{
		(*itSour)->Update(fCurrDelta);
		(*itSour)->GetPhysicsBody()->GetPhysicsData().vVelocity += m_stSpaceData.vGravity * fCurrDelta;
		(*itSour)->GetPhysicsBody()->GetPhysicsData().bOnGround = false;
		(*itSour)->UpdateTempPhysics(fCurrDelta);
	}

	for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
	{
		itDest = itSour;
		itDest++;
		for (; itDest != m_setChild.end(); itDest++)
		{
			(*itSour)->CollisionWithNode(*itDest);
		}
	}
	for (itSour = m_setChild.begin(); itSour != m_setChild.end(); itSour++)
	{
		if (m_pVecTrap) this->CollisionWithTrap((*itSour));
		this->CollisionWithMap((*itSour), fCurrDelta);
		(*itSour)->UpdatePhysics(fCurrDelta);
	}
}
void cPhysicsSpace::Render()
{


	cPhysicsNode::Render();
}
void cPhysicsSpace::Delete()
{

}
void cPhysicsSpace::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cPhysicsNode::WndProc(hWnd, message, wParam, lParam);
}


void cPhysicsSpace::CollisionWithTrap(cGameNode* pNode)
{
	cPhysicsBody* pBody;
	D3DXMATRIXA16 matWorld, matParentWorld, matR, matT;
	D3DXVECTOR3 vCrushNorm;
	D3DXVECTOR3 vGroundCheckNorm;
	float fDot;
	bool bIsCollision = false;
	/*float fVelocityIntervalDot;
	float fCrushVelocityDot;*/

	pBody = pNode->GetPhysicsBody();
	if (pBody == nullptr) return;

	if (pNode->GetParentNode() == nullptr)
		D3DXMatrixIdentity(&matParentWorld);
	else
		matParentWorld = pNode->GetParentNode()->GetMatrixToWorld();

	D3DXMatrixRotationAxis(&matR,
		&pBody->GetTempPhysicsData().vAxis,
		pBody->GetTempPhysicsData().fRotAngle);
	D3DXMatrixTranslation(&matT,
		pBody->GetTempPhysicsData().vPos.x,
		pBody->GetTempPhysicsData().vPos.y,
		pBody->GetTempPhysicsData().vPos.z);

	matWorld = matR * matT * matParentWorld;

	ST_FRUSTUM stFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
	ST_SPHERE  stSphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);


	for (size_t j = 0; j < m_pVecTrap->size(); ++j)
	{
		if (!(*m_pVecTrap)[j].isBlockable()) continue;
		//if (CheckSphereIntersectSphere(&stMySphere, &stEnemySphere))
		if (CheckOBBCollision(&(*m_pVecTrap)[j].getFrustum(), &stFrustum))
		{
			vCrushNorm = (*m_pVecTrap)[j].getFrustum().GetNearestSideNormalVec3(&stFrustum);
			fDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vCrushNorm);
			bIsCollision = true;

			if (fDot < -0.01f)
			{
				pBody->GetTempPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f);
				pBody->GetPhysicsData().vVelocity += -(vCrushNorm * fDot) * (1.f);

				D3DXVec3Normalize(&vGroundCheckNorm, &vCrushNorm);
				if (D3DXVec3Dot(&vGroundCheckNorm, &D3DXVECTOR3(0.f, 1.f, 0.f)) > cosf(PI / 4) && fDot < 0)
					pBody->GetPhysicsData().bOnGround = true;
			}
		}
	}
}