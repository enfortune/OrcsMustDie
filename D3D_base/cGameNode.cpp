#include "stdafx.h"
#include "cGameNode.h"

#include "cPhysicsBody.h"

cGameNode::cGameNode()
	: m_vPos(0.0f,0.0f,0.0f)
	, m_vAxis(0.0f,1.0f,0.0f)
	, m_fRotAngle(0.0f)
	, m_vScale(0.0f,0.0f,0.0f)
	, m_pParent(nullptr)
	, m_pPhysicsBody(nullptr)
{
	
	m_setChild.clear();
}


cGameNode::~cGameNode()
{
	for each (cGameNode* node in m_setChild)
	{
		node->RemoveFromParent();
	}
}


void cGameNode::Setup()
{

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
	m_vPos = m_pPhysicsBody->GetPosition();
	m_vAxis = m_pPhysicsBody->GetAxis();
	m_fRotAngle = m_pPhysicsBody->GetRotAngle();
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
void cGameNode::RemoveFromParent()
{
	if (m_pParent == nullptr) return;
	
	m_pParent->RemoveChild(this);
	m_pParent = nullptr;
	this->Release();

}
D3DXMATRIXA16 cGameNode::GetMatirixToParent()
{
	D3DXMATRIXA16 matToParent, matS, matR, matT;
	D3DXMatrixIdentity(&matToParent);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationAxis(&matR, &m_vAxis, m_fRotAngle);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	matToParent = matS * matR * matT;
	return matToParent;
}

D3DXMATRIXA16 cGameNode::GetMatrixToWorld()
{
	D3DXMATRIXA16 matToWorld;
	D3DXMatrixIdentity(&matToWorld);
	cGameNode* node = this;
	
	while (node->GetParentNode() != nullptr)
	{
		matToWorld *= node->GetMatirixToParent();
		node = node->GetParentNode();
	}
	return matToWorld;
}
D3DXMATRIXA16 cGameNode::GetMatrixViewPort()
{
	return D3DXMATRIXA16();
}