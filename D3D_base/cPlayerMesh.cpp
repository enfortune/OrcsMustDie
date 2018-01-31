#include "stdafx.h"
#include "cPlayerMesh.h"
#include "cSkinnedMesh.h"
#include "cTransformData.h"
#include "cPhysicsBody.h"

cPlayerMesh::cPlayerMesh()
	: m_pRotationY(0.0f)
	, m_vPlayerPos(0,0,0)
	, m_vPlayerDir(0,0,0)
{
}

cPlayerMesh::~cPlayerMesh()
{
	Delete();
}

void cPlayerMesh::Setup()
{
	cGameNode::Setup();

	m_pPlayerMesh = new cSkinnedMesh;
	m_pPlayerMesh->Setup("XFileSample","XFileSample/True_King.X");
	m_pPlayerMesh->SetAnimationSet(0, 11);

	m_vPlayerPos = GetTransformData()->GetPosition();
	m_vPlayerDir = D3DXVECTOR3(0, 0, 1);

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(-PI / 4.f);
	m_pPhysicsBody->MakeBodyCuboid(0.4f, 1.f, 0.2f, D3DXVECTOR3(0.f, 0.5f, 0.f));
	m_pPhysicsBody->GetPhysicsData().vPos = m_vPlayerPos;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(20.f, 0.f, 20.f);
	m_pPhysicsBody->SetBodyType(PHYSICSBODYTYPE_DINAMIC);
}

void cPlayerMesh::Update(float fDelta)
{
	m_pPlayerMesh->Update();
	m_pPlayerMesh->UpdateAnimation(fDelta);

	float speedX = 0.f;
	float speedZ = 0.f;
	float speedY = 0.f;

	if (g_pKeyManager->IsStayKeyDown('W'))
	{
		speedZ += 3.f;
		m_pPlayerMesh->SetAnimationSet(0, 10);
	}
	if (g_pKeyManager->IsOnceKeyUp('W')) m_pPlayerMesh->SetAnimationSet(0, 11);

	if (g_pKeyManager->IsStayKeyDown('S'))
	{
		speedZ -= 3.f;
		m_pPlayerMesh->SetAnimationSet(0, 10);
	}
	if (g_pKeyManager->IsOnceKeyUp('S')) m_pPlayerMesh->SetAnimationSet(0, 11);

	D3DXVECTOR3 vLeft;
	D3DXVec3Cross(&vLeft, &m_vPlayerDir, &D3DXVECTOR3(0, 1, 0));
	if (g_pKeyManager->IsStayKeyDown('A'))
	{
		speedX -= 3.f;
		m_pPlayerMesh->SetAnimationSet(0, 10);
	}
	if (g_pKeyManager->IsOnceKeyUp('A')) m_pPlayerMesh->SetAnimationSet(0, 11);

	if (g_pKeyManager->IsStayKeyDown('D'))
	{
		speedX += 3.f;
		m_pPlayerMesh->SetAnimationSet(0, 10);
	}
	if (g_pKeyManager->IsOnceKeyUp('D')) m_pPlayerMesh->SetAnimationSet(0, 11);

	if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
	{
		m_pPlayerMesh->SetAnimationSet(0, 1);
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		speedY += 7.f;
	}

	m_pPhysicsBody->GetPhysicsData().vVelocity.x = speedX;
	m_pPhysicsBody->GetPhysicsData().vVelocity.z = speedZ;

	if (m_pPhysicsBody->GetPhysicsData().bOnGround == true) 
		m_pPhysicsBody->GetPhysicsData().vVelocity.y = speedY;

	//GetTransformData()->SetRotAngle(m_pRotationY);
	//GetTransformData()->SetAxis(D3DXVECTOR3(0, 1, 0));
	//GetTransformData()->SetPosition(m_vPlayerPos);
	
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, m_pRotationY);
	m_vPlayerDir = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vPlayerDir, &m_vPlayerDir, &matR);

	cGameNode::Update(fDelta);
}

void cPlayerMesh::Render()
{
	D3DXMATRIXA16 matMeshWorld;

	D3DXMatrixRotationY(&matMeshWorld, D3DX_PI / 1.f);
	matMeshWorld *= this->GetMatrixToWorld();
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matMeshWorld);

	m_pPlayerMesh->Render();
	//this->m_pPhysicsBody->GetShapeData().stCuboid.Render(g_pD3DDevice);

	cGameNode::Render();
}

void cPlayerMesh::Delete()
{
	SAFE_DELETE(m_pPlayerMesh);
}

void cPlayerMesh::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
