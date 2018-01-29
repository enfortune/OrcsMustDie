#include "stdafx.h"
#include "cGameNode.h"

#include "cTransformData.h"
#include "cPhysicsBody.h"

cGameNode::cGameNode()
	: m_pTransformData(nullptr)
	, m_pParent(nullptr)
	, m_pPhysicsBody(nullptr)
	, m_pCollisionNode(nullptr)
	, m_fCullingRadius(-1.f)
{
	
	m_setChild.clear();
}


cGameNode::~cGameNode()
{
	this->Destroy();
}


void cGameNode::Setup(bool bUseTransformData)
{
	if (bUseTransformData)
	{
		m_pTransformData = new cTransformData;
	}
}

void cGameNode::Update(float fDelta)
{
	for each (cGameNode* node in m_setChild)
	{
		node->Update(fDelta);
	}

	m_pCollisionNode = nullptr;
}
void cGameNode::Render()
{
	ST_SPHERE stSphere;
	ST_FRUSTUM stFrustum;
	stFrustum = GetViewFrustum();
	for each (cGameNode* node in m_setChild)
	{
		if (node->GetTransformData() != nullptr &&
			node->GetCullingRadius() >= -0.0001f)
		{                                                
			stSphere.vCenter = node->GetTransformData()->GetPosition();
			D3DXVec3TransformCoord(&stSphere.vCenter, &stSphere.vCenter, &this->GetMatrixToWorld());
			stSphere.fRadius = node->GetCullingRadius();
			if (CheckFrustumIntersectSphere(&stFrustum, &stSphere))
				node->Render();
		}
		else
			node->Render();
	}
}
void cGameNode::Destroy()
{
	
	this->RemoveAllChildren();
	SAFE_DELETE(m_pTransformData);
	SAFE_DELETE(m_pPhysicsBody);
	this->RemoveFromParent();

	//this->Delete();
}
void cGameNode::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	for each (cGameNode* node in m_setChild)
	{
		node->WndProc(hWnd, message, wParam, lParam);
	}
}

void cGameNode::UpdateTempPhysics(float fDelta)
{
	assert(m_pPhysicsBody != nullptr && "PhysicsBody 없이 PhysicsNode에 추가해선 안된다");
	m_pPhysicsBody->UpdateTempPhysics(fDelta);
}

void cGameNode::UpdatePhysics(float fDelta)
{
	assert(m_pPhysicsBody != nullptr && "PhysicsBody 없이 PhysicsNode에 추가해선 안된다");

	m_pPhysicsBody->UpdatePhysics(fDelta);
	if (m_pTransformData)
	{
		m_pTransformData->SetPosition(m_pPhysicsBody->GetPhysicsData().vPos);
		m_pTransformData->SetAxis(m_pPhysicsBody->GetPhysicsData().vAxis);
		m_pTransformData->SetRotAngle(m_pPhysicsBody->GetPhysicsData().fRotAngle);
	}
}

void cGameNode::CollisionWithNode(cGameNode* pNode)
{
	cPhysicsBody* pBody = pNode->GetPhysicsBody();
	if (pBody == nullptr) return;

	D3DXMATRIXA16 matMyWorld, matMyParentWorld, matMyR, matMyT;
	if (this->GetParentNode() == nullptr)
		D3DXMatrixIdentity(&matMyParentWorld);
	else matMyParentWorld = this->GetParentNode()->GetMatrixToWorld();

	D3DXMatrixRotationAxis(&matMyR,
		&m_pPhysicsBody->GetTempPhysicsData().vAxis,
		m_pPhysicsBody->GetTempPhysicsData().fRotAngle);
	D3DXMatrixTranslation(&matMyT,
		m_pPhysicsBody->GetTempPhysicsData().vPos.x,
		m_pPhysicsBody->GetTempPhysicsData().vPos.y,
		m_pPhysicsBody->GetTempPhysicsData().vPos.z);

	matMyWorld = matMyR * matMyT * matMyParentWorld;

	D3DXMATRIXA16 matWorld, matParentWorld, matR, matT;
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

	ST_FRUSTUM stMyFrustum = m_pPhysicsBody->GetShapeData().stCuboid.TransformCoord(&matMyWorld);
	ST_SPHERE  stMySphere = m_pPhysicsBody->GetShapeData().stSphere.TransformCoord(&matMyWorld);
	ST_FRUSTUM stOpponentFrustum = pBody->GetShapeData().stCuboid.TransformCoord(&matWorld);
	ST_SPHERE  stOpponentSphere = pBody->GetShapeData().stSphere.TransformCoord(&matWorld);

	D3DXVECTOR3 vMyCrushNorm;
	D3DXVECTOR3 vMyGroundCheckNorm;
	float fMyDot;
	D3DXVECTOR3 vOpponentCrushNorm;
	D3DXVECTOR3 vOpponentGroundCheckNorm;
	float fOpponentDot;
	bool bIsCollision = false;


	if (m_pPhysicsBody->GetShapeData().enShapeType == PHYSICSSHAPETYPE_SPHERE)
	{
		if (pBody->GetShapeData().enShapeType == PHYSICSSHAPETYPE_SPHERE)
		{
			if (CheckSphereIntersectSphere(&stMySphere, &stOpponentSphere))
			{
				vMyCrushNorm = stMySphere.vCenter - stOpponentSphere.vCenter;
				fMyDot = D3DXVec3Dot(&m_pPhysicsBody->GetTempPhysicsData().vVelocity, &vMyCrushNorm);
				vOpponentCrushNorm = -vMyCrushNorm;
				fOpponentDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vOpponentCrushNorm);
				bIsCollision = true;
			}
		}
		else if (pBody->GetShapeData().enShapeType == PHYSICSSHAPETYPE_CUBOID)
		{
			if (CheckSphereIntersectSphere(&stMySphere, &stOpponentSphere))
				if (CheckFrustumIntersectSphere(&stOpponentFrustum, &stMySphere))
				{
					vMyCrushNorm = stOpponentFrustum.GetNearestSideNormalVec3(&stMySphere.vCenter);
					fMyDot = D3DXVec3Dot(&m_pPhysicsBody->GetTempPhysicsData().vVelocity, &vMyCrushNorm);
					vOpponentCrushNorm = -vMyCrushNorm;
					fOpponentDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vOpponentCrushNorm);
					bIsCollision = true;
				}
		}
	}
	else if (m_pPhysicsBody->GetShapeData().enShapeType == PHYSICSSHAPETYPE_CUBOID)
	{
		if (pBody->GetShapeData().enShapeType == PHYSICSSHAPETYPE_SPHERE)
		{
			if (CheckSphereIntersectSphere(&stMySphere, &stOpponentSphere))
				if (CheckFrustumIntersectSphere(&stMyFrustum, &stOpponentSphere))
				{
					vOpponentCrushNorm = stMyFrustum.GetNearestSideNormalVec3(&stOpponentSphere.vCenter);
					fOpponentDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vOpponentCrushNorm);
					vMyCrushNorm = -vOpponentCrushNorm;
					fMyDot = D3DXVec3Dot(&m_pPhysicsBody->GetTempPhysicsData().vVelocity, &vMyCrushNorm);
					bIsCollision = true;
				}
		}
		else if (pBody->GetShapeData().enShapeType == PHYSICSSHAPETYPE_CUBOID)
		{
			if (CheckSphereIntersectSphere(&stMySphere, &stOpponentSphere))
				if (CheckFrustumIntersectFrustum(&stMyFrustum, &stOpponentFrustum))
				{
					vMyCrushNorm = stOpponentFrustum.GetNearestSideNormalVec3(&stMyFrustum);
					fMyDot = D3DXVec3Dot(&m_pPhysicsBody->GetTempPhysicsData().vVelocity, &vMyCrushNorm);
					vOpponentCrushNorm = -vMyCrushNorm;
					fOpponentDot = D3DXVec3Dot(&pBody->GetTempPhysicsData().vVelocity, &vOpponentCrushNorm);
					bIsCollision = true;
				}
		}
	}
	if (bIsCollision == true)
	{
		m_pCollisionNode = pNode;

		D3DXVec3Normalize(&vMyGroundCheckNorm, &vMyCrushNorm);
		if (D3DXVec3Dot(&vMyGroundCheckNorm, &D3DXVECTOR3(0.f, 1.f, 0.f)) > cosf(PI / 4) && fMyDot < 0)
			m_pPhysicsBody->GetPhysicsData().bOnGround = true;
		D3DXVec3Normalize(&vOpponentGroundCheckNorm, &vOpponentCrushNorm);
		if (D3DXVec3Dot(&vOpponentGroundCheckNorm, &D3DXVECTOR3(0.f, 1.f, 0.f)) > cosf(PI / 4) && fOpponentDot < 0)
			pBody->GetPhysicsData().bOnGround = true;

		if (m_pPhysicsBody->GetBodyType() == PHYSICSBODYTYPE_STATIC &&
			pBody->GetBodyType() == PHYSICSBODYTYPE_STATIC)
		{
			if (fMyDot < 0)
			{
				m_pPhysicsBody->GetTempPhysicsData().vVelocity += -(vMyCrushNorm * fMyDot) * (1.f/* + m_pPhysicsBody->GetTempPhysicsData().fElasticity*/);
				m_pPhysicsBody->GetPhysicsData().vVelocity += -(vMyCrushNorm * fMyDot) * (1.f/* + m_pPhysicsBody->GetTempPhysicsData().fElasticity*/);
			}
			if (fOpponentDot < 0)
			{
				pBody->GetTempPhysicsData().vVelocity += -(vOpponentCrushNorm * fOpponentDot) * (1.f/* + pBody->GetTempPhysicsData().fElasticity*/);
				pBody->GetPhysicsData().vVelocity += -(vOpponentCrushNorm * fOpponentDot) * (1.f/* + pBody->GetTempPhysicsData().fElasticity*/);
			}
		}
		else if (m_pPhysicsBody->GetBodyType() == PHYSICSBODYTYPE_DINAMIC &&
			pBody->GetBodyType() == PHYSICSBODYTYPE_STATIC)
		{
			if (fMyDot < 0)
			{
				m_pPhysicsBody->GetTempPhysicsData().vVelocity += -(vMyCrushNorm * fMyDot) * (1.f + m_pPhysicsBody->GetTempPhysicsData().fElasticity);
				m_pPhysicsBody->GetTempPhysicsData().vVelocity += (vMyCrushNorm * fOpponentDot);

				m_pPhysicsBody->GetPhysicsData().vVelocity += -(vMyCrushNorm * fMyDot) * (1.f + m_pPhysicsBody->GetTempPhysicsData().fElasticity);
				m_pPhysicsBody->GetPhysicsData().vVelocity += (vMyCrushNorm * fOpponentDot);
			}
		}
		else if (m_pPhysicsBody->GetBodyType() == PHYSICSBODYTYPE_STATIC &&
			pBody->GetBodyType() == PHYSICSBODYTYPE_DINAMIC)
		{
			if (fOpponentDot < 0)
			{
				pBody->GetTempPhysicsData().vVelocity += -(vOpponentCrushNorm * fOpponentDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
				pBody->GetTempPhysicsData().vVelocity += (vOpponentCrushNorm * fMyDot);

				pBody->GetPhysicsData().vVelocity += -(vOpponentCrushNorm * fOpponentDot) * (1.f + pBody->GetTempPhysicsData().fElasticity);
				pBody->GetPhysicsData().vVelocity += (vOpponentCrushNorm * fMyDot);
			}
		}
		else if (m_pPhysicsBody->GetBodyType() == PHYSICSBODYTYPE_DINAMIC &&
			pBody->GetBodyType() == PHYSICSBODYTYPE_DINAMIC)
		{
			if (fMyDot < 0)
			{
				m_pPhysicsBody->GetTempPhysicsData().vVelocity += -(vMyCrushNorm * fMyDot) * (1.f + m_pPhysicsBody->GetTempPhysicsData().fElasticity* pBody->GetPhysicsData().fMass / m_pPhysicsBody->GetPhysicsData().fMass);
				pBody->GetTempPhysicsData().vVelocity += (vMyCrushNorm * fMyDot) * (1.f + m_pPhysicsBody->GetTempPhysicsData().fElasticity * m_pPhysicsBody->GetPhysicsData().fMass / pBody->GetPhysicsData().fMass);

				m_pPhysicsBody->GetPhysicsData().vVelocity += -(vMyCrushNorm * fMyDot) * (1.f + m_pPhysicsBody->GetTempPhysicsData().fElasticity* pBody->GetPhysicsData().fMass / m_pPhysicsBody->GetPhysicsData().fMass);
				pBody->GetPhysicsData().vVelocity += (vMyCrushNorm * fMyDot) * (1.f + m_pPhysicsBody->GetTempPhysicsData().fElasticity * m_pPhysicsBody->GetPhysicsData().fMass / pBody->GetPhysicsData().fMass);
			}
			if (fOpponentDot < 0)
			{
				pBody->GetTempPhysicsData().vVelocity += -(vOpponentCrushNorm * fOpponentDot) * (1.f + pBody->GetTempPhysicsData().fElasticity * m_pPhysicsBody->GetPhysicsData().fMass / m_pPhysicsBody->GetPhysicsData().fMass);
				m_pPhysicsBody->GetTempPhysicsData().vVelocity += (vOpponentCrushNorm * fOpponentDot) * (1.f + pBody->GetTempPhysicsData().fElasticity * pBody->GetPhysicsData().fMass / m_pPhysicsBody->GetPhysicsData().fMass);

				pBody->GetPhysicsData().vVelocity += -(vOpponentCrushNorm * fOpponentDot) * (1.f + pBody->GetTempPhysicsData().fElasticity * m_pPhysicsBody->GetPhysicsData().fMass / m_pPhysicsBody->GetPhysicsData().fMass);
				m_pPhysicsBody->GetPhysicsData().vVelocity += (vOpponentCrushNorm * fOpponentDot) * (1.f + pBody->GetTempPhysicsData().fElasticity * pBody->GetPhysicsData().fMass / m_pPhysicsBody->GetPhysicsData().fMass);
			}
		}
		//m_pPhysicsBody->GetPhysicsData().vVelocity = m_pPhysicsBody->GetTempPhysicsData().vVelocity;
		//pBody->GetPhysicsData().vVelocity = pBody->GetTempPhysicsData().vVelocity;
	}

}


void cGameNode::AddChild(cGameNode* pNode)
{
	if (pNode != nullptr)
	{
		assert(pNode->GetParentNode() == nullptr && "node는 하나의 parent만 가질 수 있음.");
		pNode->AddRef();
		m_setChild.insert(pNode);
		//cGameNode *node = this;
		//this->AddRef();
		pNode->SetParentNode(this);
	}		
}
void cGameNode::RemoveChild(cGameNode* pNode)
{
	std::set<cGameNode*>::iterator Iter = m_setChild.find(pNode);
	if (Iter == m_setChild.end()) return;

	(*Iter)->SetParentNode(nullptr);
	(*Iter)->Release();
	Iter = m_setChild.erase(Iter);
}
void cGameNode::RemoveAllChildren()
{
	std::set<cGameNode*>::iterator Iter = m_setChild.begin();
	while (Iter != m_setChild.end())
	{
		(*Iter)->RemoveAllChildren();
		(*Iter)->RemoveFromParent();
		Iter = m_setChild.begin();
	}
}
void cGameNode::RemoveFromParent()
{
	if (m_pParent == nullptr) return;
	
	m_pParent->RemoveChild(this);
	//m_pParent->Release();
	
	//this->Release();

}
D3DXMATRIXA16 cGameNode::GetMatirixToParent()
{
	if (m_pTransformData)
	{
		return m_pTransformData->GetTransformMatrix();
	}
	else
	{
		D3DXMATRIXA16 matI;
		D3DXMatrixIdentity(&matI);
		return matI;
	}	
}

D3DXMATRIXA16 cGameNode::GetMatrixToWorld()
{
	D3DXMATRIXA16 matToWorld;
	D3DXMatrixIdentity(&matToWorld);
	cGameNode* node = this;
	while (node != nullptr /*&& node->GetTransformData() != nullptr*/)
	{
		matToWorld *= node->GetMatirixToParent();
		node = node->GetParentNode();
	}
	return matToWorld;
}
//D3DXMATRIXA16 cGameNode::GetMatrixToViewPort()
//{
//	D3DXMATRIXA16 matToWorld = this->GetMatrixToWorld();
//	D3DXMATRIXA16 matView, matProjection, matViewport;
//	g_pDeviceManager->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
//	g_pDeviceManager->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);
//	g_pDeviceManager->GetDevice()->GetTransform(D3DTS_, &matProjection);
//
//	//return this-> 
//}
