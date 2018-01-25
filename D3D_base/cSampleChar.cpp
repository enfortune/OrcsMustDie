#include "stdafx.h"
#include "cSampleChar.h"
#include "cSkinnedMesh.h"
#include "cTransformData.h"

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
	bMove = FALSE;
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

	move1 = true;
	move2 = false;
	move3 = false;
}

void cSampleChar::Update(float fDelta)
{
	m_pSkinnedMesh->Update();
	m_pSkinnedMesh->UpdateAnimation(fDelta);
	GetTransformData()->SetPosition(vPos);
	if (move1)
	{
		Move(D3DXVECTOR3(-7, 0, -5), fDelta);
	}
	else if (move2)
	{
		Move(D3DXVECTOR3(5, 0, 6), fDelta);
	}
	else if (move3)
	{
		Move(D3DXVECTOR3(-4, 0, 4), fDelta);
	}
	
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

void cSampleChar::UpdatePhysics(float fDelta)
{
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
			m_pSkinnedMesh->SetAnimationSet(0, 2);
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
			GetTransformData()->SetAxis(D3DXVECTOR3(0, 1, 0));
			GetTransformData()->SetRotAngle(tempRot);
			vPos += vDir * (fDelta * 5.f);
		}
		else
		{
			vPos = vGoal;
			bMove = FALSE;
			m_pSkinnedMesh->SetAnimationSet(0, 3);
			if (move1)
			{
				move1 = false;
				move2 = true;
			}
			else if (move2)
			{
				move2 = false;
				move3 = true;
			}
			else if (move3)
			{
				move3 = false;
				move1 = true;
			}
		}
	}
}

void cSampleChar::Attack(D3DXVECTOR3 vPlayer)
{

}

