#include "stdafx.h"
#include "cPhysicsBody.h"

#define PHYSICSBODY_ANGLEVELOCITYEPSILON 0.0001f
#define PHYSICSBODY_VELOCITYEPSILON 0.0001f

cPhysicsBody::cPhysicsBody()
	: m_enBodyType(PHYSICSBODYTYPE_END)
	, m_bSpinEnable(false)
	
{
}


cPhysicsBody::~cPhysicsBody()
{
}
void cPhysicsBody::Setup(float fOriginAngle)
{
	m_stPhysicsData.fOriginAngle = fOriginAngle;
}
void cPhysicsBody::MakeBodyShape(ST_PHYSICSSHAPEDATA stShapeData)
{
	m_stShapeData = stShapeData;
}
void cPhysicsBody::MakeBodyRectosahedron(float fX, float fY, float fZ, D3DXVECTOR3 vCenter)
{
	m_stShapeData.enShapeType = PHYSICSSHAPETYPE_RECTOSAHEDRON;
	m_stShapeData.vCenter = vCenter;
	m_stShapeData.vecVertex.clear();

	D3DXVECTOR3 _000 = D3DXVECTOR3(vCenter.x - fX / 2, vCenter.y - fY / 2, vCenter.z - fZ / 2);
	D3DXVECTOR3 _001 = D3DXVECTOR3(vCenter.x - fX / 2, vCenter.y - fY / 2, vCenter.z + fZ / 2);
	D3DXVECTOR3 _010 = D3DXVECTOR3(vCenter.x - fX / 2, vCenter.y + fY / 2, vCenter.z - fZ / 2);
	D3DXVECTOR3 _011 = D3DXVECTOR3(vCenter.x - fX / 2, vCenter.y + fY / 2, vCenter.z + fZ / 2);
	D3DXVECTOR3 _100 = D3DXVECTOR3(vCenter.x + fX / 2, vCenter.y - fY / 2, vCenter.z - fZ / 2);
	D3DXVECTOR3 _101 = D3DXVECTOR3(vCenter.x + fX / 2, vCenter.y - fY / 2, vCenter.z + fZ / 2);
	D3DXVECTOR3 _110 = D3DXVECTOR3(vCenter.x + fX / 2, vCenter.y + fY / 2, vCenter.z - fZ / 2);
	D3DXVECTOR3 _111 = D3DXVECTOR3(vCenter.x + fX / 2, vCenter.y + fY / 2, vCenter.z + fZ / 2);

	m_stShapeData.vecVertex.push_back(_000);
	m_stShapeData.vecVertex.push_back(_001);
	m_stShapeData.vecVertex.push_back(_010);
	m_stShapeData.vecVertex.push_back(_011);
	m_stShapeData.vecVertex.push_back(_100);
	m_stShapeData.vecVertex.push_back(_101);
	m_stShapeData.vecVertex.push_back(_110);
	m_stShapeData.vecVertex.push_back(_111);

	m_stShapeData.fRadius = sqrtf(powf(fX / 2, 2.f) + powf(fY / 2, 2.f) + powf(fZ / 2, 2.f));
}

void cPhysicsBody::MakeBodySphere(float fRadius, D3DXVECTOR3 vCenter)
{
	m_stShapeData.enShapeType = PHYSICSSHAPETYPE_SPHERE;
	m_stShapeData.fRadius = fRadius;
	m_stShapeData.vCenter = vCenter;
}

void cPhysicsBody::UpdateTempPhysics(float fDelta)
{
	m_stTempPhysicsData = m_stPhysicsData;

	if (m_bSpinEnable)
	{
		while (m_stTempPhysicsData.fRotAngle >= PI * 2 || m_stTempPhysicsData.fRotAngle < 0) // ������
		{
			if (m_stTempPhysicsData.fRotAngle >= PI * 2) m_stTempPhysicsData.fRotAngle -= PI * 2;
			if (m_stTempPhysicsData.fRotAngle < 0) m_stTempPhysicsData.fRotAngle += PI * 2;
		}

		m_stTempPhysicsData.fAngleAccel += m_stTempPhysicsData.fSurfaceTorque * fDelta / m_stTempPhysicsData.fMass;
		m_stTempPhysicsData.fAngleVelocity += m_stTempPhysicsData.fAngleAccel * fDelta;

		if (fabs(m_stTempPhysicsData.fAngleVelocity) <= PHYSICSBODY_ANGLEVELOCITYEPSILON) m_stTempPhysicsData.fAngleVelocity = 0.f;

		m_stTempPhysicsData.fRotAngle += m_stTempPhysicsData.fAngleVelocity * fDelta;

		if (m_stTempPhysicsData.fAngleVelocity > 0.f) // x ����
		{
			m_stTempPhysicsData.fAngleAccel -= m_stTempPhysicsData.fAngleDamping * fDelta;
			if (m_stTempPhysicsData.fAngleAccel < 0.f) m_stTempPhysicsData.fAngleAccel = 0.f;
		}
		else if (m_stTempPhysicsData.fAngleVelocity < 0.f)
		{
			m_stTempPhysicsData.fAngleAccel += m_stTempPhysicsData.fAngleDamping * fDelta;
			if (m_stTempPhysicsData.fAngleAccel > 0.f) m_stTempPhysicsData.fAngleAccel = 0.f;
		}


		m_stTempPhysicsData.fSurfaceTorque = 0.0f;
	}
	else
	{
		m_stTempPhysicsData.fSurfaceTorque = 0.0f;
	}

	m_stTempPhysicsData.vAccel += m_stTempPhysicsData.vForce * fDelta / m_stTempPhysicsData.fMass;
	m_stTempPhysicsData.vVelocity += m_stTempPhysicsData.vAccel * fDelta;
	if (fabs(m_stTempPhysicsData.vVelocity.x) <= PHYSICSBODY_VELOCITYEPSILON) m_stTempPhysicsData.vVelocity.x = 0.f;
	if (fabs(m_stTempPhysicsData.vVelocity.y) <= PHYSICSBODY_VELOCITYEPSILON) m_stTempPhysicsData.vVelocity.y = 0.f;
	if (fabs(m_stTempPhysicsData.vVelocity.z) <= PHYSICSBODY_VELOCITYEPSILON) m_stTempPhysicsData.vVelocity.z = 0.f;

	m_stTempPhysicsData.vPos += m_stTempPhysicsData.vVelocity * fDelta;

	if (m_stTempPhysicsData.vVelocity.x > 0.f) // x ����
	{
		m_stTempPhysicsData.vAccel.x -= m_stTempPhysicsData.vDamping.x * fDelta;
		if (m_stTempPhysicsData.vAccel.x < 0.f) m_stTempPhysicsData.vAccel.x = 0.f;
	}
	else if (m_stTempPhysicsData.vVelocity.x < 0.f)
	{
		m_stTempPhysicsData.vAccel.x += m_stTempPhysicsData.vDamping.x * fDelta;
		if (m_stTempPhysicsData.vAccel.x > 0.f) m_stTempPhysicsData.vAccel.x = 0.f;
	}

	if (m_stTempPhysicsData.vVelocity.y > 0.f) // y ����
	{
		m_stTempPhysicsData.vAccel.y -= m_stTempPhysicsData.vDamping.y * fDelta;
		if (m_stTempPhysicsData.vAccel.y < 0.f) m_stTempPhysicsData.vAccel.y = 0.f;
	}
	else if (m_stTempPhysicsData.vVelocity.y < 0.f)
	{
		m_stTempPhysicsData.vAccel.y += m_stTempPhysicsData.vDamping.y * fDelta;
		if (m_stTempPhysicsData.vAccel.y > 0.f) m_stTempPhysicsData.vAccel.y = 0.f;
	}

	if (m_stTempPhysicsData.vVelocity.z > 0.f) // z ����
	{
		m_stTempPhysicsData.vAccel.z -= m_stTempPhysicsData.vDamping.z * fDelta;
		if (m_stTempPhysicsData.vAccel.z < 0.f) m_stTempPhysicsData.vAccel.z = 0.f;
	}
	else if (m_stTempPhysicsData.vVelocity.z < 0.f)
	{
		m_stTempPhysicsData.vAccel.z += m_stTempPhysicsData.vDamping.z * fDelta;
		if (m_stTempPhysicsData.vAccel.z > 0.f) m_stTempPhysicsData.vAccel.z = 0.f;
	}


	m_stTempPhysicsData.vForce = D3DXVECTOR3(0.f, 0.f, 0.f); // �ܷ� �ʱ�ȭ
}

void cPhysicsBody::UpdatePhysics(float fDelta)
{
	if (m_bSpinEnable)
	{
		while (m_stPhysicsData.fRotAngle >= PI * 2 || m_stPhysicsData.fRotAngle < 0) // ������
		{
			if (m_stPhysicsData.fRotAngle >= PI * 2) m_stPhysicsData.fRotAngle -= PI * 2;
			if (m_stPhysicsData.fRotAngle < 0) m_stPhysicsData.fRotAngle += PI * 2;
		}

		m_stPhysicsData.fAngleAccel += m_stPhysicsData.fSurfaceTorque * fDelta / m_stPhysicsData.fMass;
		m_stPhysicsData.fAngleVelocity += m_stPhysicsData.fAngleAccel * fDelta;

		if (fabs(m_stPhysicsData.fAngleVelocity) <= PHYSICSBODY_ANGLEVELOCITYEPSILON) m_stPhysicsData.fAngleVelocity = 0.f;

		m_stPhysicsData.fRotAngle += m_stPhysicsData.fAngleVelocity * fDelta;

		if (m_stPhysicsData.fAngleVelocity > 0.f) // x ����
		{
			m_stPhysicsData.fAngleAccel -= m_stPhysicsData.fAngleDamping * fDelta;
			if (m_stPhysicsData.fAngleAccel < 0.f) m_stPhysicsData.fAngleAccel = 0.f;
		}
		else if (m_stPhysicsData.fAngleVelocity < 0.f)
		{
			m_stPhysicsData.fAngleAccel += m_stPhysicsData.fAngleDamping * fDelta;
			if (m_stPhysicsData.fAngleAccel > 0.f) m_stPhysicsData.fAngleAccel = 0.f;
		}

		
		m_stPhysicsData.fSurfaceTorque = 0.0f;
	}
	else
	{
		m_stPhysicsData.fSurfaceTorque = 0.0f;
	}	

	m_stPhysicsData.vAccel += m_stPhysicsData.vForce * fDelta / m_stPhysicsData.fMass;
	m_stPhysicsData.vVelocity += m_stPhysicsData.vAccel * fDelta;
	if (fabs(m_stPhysicsData.vVelocity.x) <= PHYSICSBODY_VELOCITYEPSILON) m_stPhysicsData.vVelocity.x = 0.f;
	if (fabs(m_stPhysicsData.vVelocity.y) <= PHYSICSBODY_VELOCITYEPSILON) m_stPhysicsData.vVelocity.y = 0.f;
	if (fabs(m_stPhysicsData.vVelocity.z) <= PHYSICSBODY_VELOCITYEPSILON) m_stPhysicsData.vVelocity.z = 0.f;

	m_stPhysicsData.vPos += m_stPhysicsData.vVelocity * fDelta;

	if (m_stPhysicsData.vVelocity.x > 0.f) // x ����
	{
		m_stPhysicsData.vAccel.x -= m_stPhysicsData.vDamping.x * fDelta;
		if (m_stPhysicsData.vAccel.x < 0.f) m_stPhysicsData.vAccel.x = 0.f;
	}
	else if (m_stPhysicsData.vVelocity.x < 0.f)
	{
		m_stPhysicsData.vAccel.x += m_stPhysicsData.vDamping.x * fDelta;
		if (m_stPhysicsData.vAccel.x > 0.f) m_stPhysicsData.vAccel.x = 0.f;
	}

	if (m_stPhysicsData.vVelocity.y > 0.f) // y ����
	{
		m_stPhysicsData.vAccel.y -= m_stPhysicsData.vDamping.y * fDelta;
		if (m_stPhysicsData.vAccel.y < 0.f) m_stPhysicsData.vAccel.y = 0.f;
	}
	else if (m_stPhysicsData.vVelocity.y < 0.f)
	{
		m_stPhysicsData.vAccel.y += m_stPhysicsData.vDamping.y * fDelta;
		if (m_stPhysicsData.vAccel.y > 0.f) m_stPhysicsData.vAccel.y = 0.f;
	}

	if (m_stPhysicsData.vVelocity.z > 0.f) // z ����
	{
		m_stPhysicsData.vAccel.z -= m_stPhysicsData.vDamping.z * fDelta;
		if (m_stPhysicsData.vAccel.z < 0.f) m_stPhysicsData.vAccel.z = 0.f;
	}
	else if (m_stPhysicsData.vVelocity.z < 0.f)
	{
		m_stPhysicsData.vAccel.z += m_stPhysicsData.vDamping.z * fDelta;
		if (m_stPhysicsData.vAccel.z > 0.f) m_stPhysicsData.vAccel.z = 0.f;
	}

	
	m_stPhysicsData.vForce = D3DXVECTOR3(0.f, 0.f, 0.f); // �ܷ� �ʱ�ȭ

}