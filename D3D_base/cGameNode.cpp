#include "stdafx.h"
#include "cGameNode.h"

#include "cTransformData.h"
#include "cPhysicsBody.h"

cGameNode::cGameNode()
	: m_pTransformData(nullptr)
	, m_pParent(nullptr)
	, m_pPhysicsBody(nullptr)
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
		node->Update();
	}
}
void cGameNode::Render()
{
	for each (cGameNode* node in m_setChild)
	{
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

void cGameNode::UpdatePhysics(float fDelta)
{
	assert(m_pPhysicsBody == nullptr && "PhysicsBody 없이 PhysicsNode에 추가해선 안된다");

	m_pPhysicsBody->UpdatePhysics();
	if (m_pTransformData)
	{
		m_pTransformData->SetPosition(m_pPhysicsBody->GetPosition());
		m_pTransformData->SetAxis(m_pPhysicsBody->GetAxis());
		m_pTransformData->SetRotAngle(m_pPhysicsBody->GetRotAngle());
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