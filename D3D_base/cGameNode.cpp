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
	assert(m_pPhysicsBody != nullptr && "PhysicsBody 없이 PhysicsNode에 추가해선 안된다");

	m_pPhysicsBody->UpdatePhysics();
	if (m_pTransformData)
	{
		m_pTransformData->SetPosition(m_pPhysicsBody->GetPosition());
		m_pTransformData->SetAxis(m_pPhysicsBody->GetAxis());
		m_pTransformData->SetRotAngle(m_pPhysicsBody->GetRotAngle());
	}
}


void cGameNode::AddChild(cGameNode* node)
{
	if (node == nullptr)
	{
		assert(node->GetParentNode() != nullptr && "node는 하나의 parent만 가질 수 있음.");
		node->AddRef();
		m_setChild.insert(node);
		cGameNode *node = this;
		node->SetParentNode(node);
	}		
}
void cGameNode::RemoveChild(cGameNode* node)
{
	std::set<cGameNode*>::iterator Iter = m_setChild.find(node);
	if (Iter == m_setChild.end()) return;

	(*Iter)->Release();
	(*Iter)->SetParentNode(nullptr);
	Iter = m_setChild.erase(Iter);
}
void cGameNode::RemoveAllChildren()
{
	for each (cGameNode* node in m_setChild)
	{
		node->RemoveFromParent();
	}
}
void cGameNode::RemoveFromParent()
{
	if (m_pParent == nullptr) return;
	
	m_pParent->RemoveChild(this);
	m_pParent = nullptr;
	this->Release();

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
	
	while (node != nullptr)
	{
		matToWorld *= node->GetMatirixToParent();
		node = node->GetParentNode();
	}
	return matToWorld;
}
D3DXMATRIXA16 cGameNode::GetMatrixViewPort()
{
	// TODO: 만드세용
	return D3DXMATRIXA16();
}