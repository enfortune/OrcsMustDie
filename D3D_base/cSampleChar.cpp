#include "stdafx.h"
#include "cSampleChar.h"
#include "cSkinnedMesh.h"
#include "cTransformData.h"
#include "cPhysicsBody.h"

cSampleChar::cSampleChar()
{
}


cSampleChar::~cSampleChar()
{
	Delete();
}

void cSampleChar::Setup(bool move, D3DXVECTOR3 vPosSetup)
{
	cGameNode::Setup(true);
	bMove = move;
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup("XFileSample", "XFileSample/chockchock.X");
	m_pSkinnedMesh->SetAnimationSet(0, 3);
	vDir = D3DXVECTOR3(0, 0, -1);
	fRotY = 0.f;
	GetTransformData()->SetPosition(vPosSetup);
	GetTransformData()->SetAxis(vDir);
	vPos = vPosSetup;

	m_pPhysicsBody = new cPhysicsBody;
	m_pPhysicsBody->Setup(-PI / 4.f);
	m_pPhysicsBody->MakeBodyCuboid(0.4f, 1.f, 0.2f, D3DXVECTOR3(0.f, 0.5f, 0.f));
	m_pPhysicsBody->GetPhysicsData().vPos = vPosSetup;
	m_pPhysicsBody->GetPhysicsData().vDamping = D3DXVECTOR3(20.f, 0.f, 20.f);
	m_pPhysicsBody->SetBodyType(PHYSICSBODYTYPE_DINAMIC); // PHYSICSBODYTYPE_STATIC은 밀어낼 수 없는 속성.

}

void cSampleChar::Update(float fDelta)
{
	m_pSkinnedMesh->Update();
	m_pSkinnedMesh->UpdateAnimation(fDelta);



	//m_pPhysicsBody->GetPhysicsData().vVelocity.z
	if (bMove)
	{
		float speedX = 0.f;
		float speedZ = 0.f;
		float speedY = 0.f;

		if (g_pKeyManager->IsStayKeyDown('W')) speedZ += 3.f;
		if (g_pKeyManager->IsStayKeyDown('S')) speedZ -= 3.f;
		if (g_pKeyManager->IsStayKeyDown('A')) speedX -= 3.f;
		if (g_pKeyManager->IsStayKeyDown('D')) speedX += 3.f;
		if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))  speedY += 7.f;

		m_pPhysicsBody->GetPhysicsData().vVelocity.x = speedX;
		m_pPhysicsBody->GetPhysicsData().vVelocity.z = speedZ;

		if (m_pPhysicsBody->GetPhysicsData().bOnGround == true) m_pPhysicsBody->GetPhysicsData().vVelocity.y = speedY;
	}
	

	cGameNode::Update(fDelta);
}

void cSampleChar::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &this->GetMatrixToWorld());

	//m_pSkinnedMesh->Render();
	this->m_pPhysicsBody->GetShapeData().stCuboid.Render(g_pD3DDevice);

	cGameNode::Render();
}

void cSampleChar::Delete()
{
	SAFE_DELETE(m_pSkinnedMesh);

}

void cSampleChar::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}
