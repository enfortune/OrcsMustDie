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
void cPhysicsBody::MakeBodyCuboid(float fX, float fY, float fZ, D3DXVECTOR3 vCenter)
{
	m_stShapeData.enShapeType = PHYSICSSHAPETYPE_CUBOID;
	m_stShapeData.stSphere.vCenter = vCenter;
	//m_stShapeData.vecVertex.clear();

	D3DXVECTOR3 _000 = D3DXVECTOR3(vCenter.x - fX / 2.f, vCenter.y - fY / 2.f, vCenter.z - fZ / 2.f);
	D3DXVECTOR3 _001 = D3DXVECTOR3(vCenter.x - fX / 2.f, vCenter.y - fY / 2.f, vCenter.z + fZ / 2.f);
	D3DXVECTOR3 _010 = D3DXVECTOR3(vCenter.x - fX / 2.f, vCenter.y + fY / 2.f, vCenter.z - fZ / 2.f);
	D3DXVECTOR3 _011 = D3DXVECTOR3(vCenter.x - fX / 2.f, vCenter.y + fY / 2.f, vCenter.z + fZ / 2.f);
	D3DXVECTOR3 _100 = D3DXVECTOR3(vCenter.x + fX / 2.f, vCenter.y - fY / 2.f, vCenter.z - fZ / 2.f);
	D3DXVECTOR3 _101 = D3DXVECTOR3(vCenter.x + fX / 2.f, vCenter.y - fY / 2.f, vCenter.z + fZ / 2.f);
	D3DXVECTOR3 _110 = D3DXVECTOR3(vCenter.x + fX / 2.f, vCenter.y + fY / 2.f, vCenter.z - fZ / 2.f);
	D3DXVECTOR3 _111 = D3DXVECTOR3(vCenter.x + fX / 2.f, vCenter.y + fY / 2.f, vCenter.z + fZ / 2.f);

	/*m_stShapeData.vecVertex.push_back(_000);
	m_stShapeData.vecVertex.push_back(_001);
	m_stShapeData.vecVertex.push_back(_010);
	m_stShapeData.vecVertex.push_back(_011);
	m_stShapeData.vecVertex.push_back(_100);
	m_stShapeData.vecVertex.push_back(_101);
	m_stShapeData.vecVertex.push_back(_110);
	m_stShapeData.vecVertex.push_back(_111);*/

	m_stShapeData.stCuboid.vNear_00 =	_000;
	m_stShapeData.stCuboid.vNear_01 =	_010;
	m_stShapeData.stCuboid.vNear_10 =	_100;
	m_stShapeData.stCuboid.vNear_11 =	_110;
	m_stShapeData.stCuboid.vFar_00 =	_001;
	m_stShapeData.stCuboid.vFar_01 =	_011;
	m_stShapeData.stCuboid.vFar_10 =	_101;
	m_stShapeData.stCuboid.vFar_11 =	_111;

	m_stShapeData.stSphere.fRadius = sqrtf(powf(fX / 2.f, 2.f) + powf(fY / 2.f, 2.f) + powf(fZ / 2.f, 2.f));
}

void cPhysicsBody::MakeBodySphere(float fRadius, D3DXVECTOR3 vCenter)
{
	m_stShapeData.enShapeType = PHYSICSSHAPETYPE_SPHERE;
	m_stShapeData.stSphere.fRadius = fRadius;
	m_stShapeData.stSphere.vCenter = vCenter;
}

void cPhysicsBody::UpdateTempPhysics(float fDelta)
{
	m_stTempPhysicsData = m_stPhysicsData;

	while (m_stTempPhysicsData.fRotAngle >= PI * 2.f || m_stTempPhysicsData.fRotAngle < 0) // ∞¢∫∏¡§
	{
		if (m_stTempPhysicsData.fRotAngle >= PI * 2.f) m_stTempPhysicsData.fRotAngle -= PI * 2.f;
		if (m_stTempPhysicsData.fRotAngle < 0) m_stTempPhysicsData.fRotAngle += PI * 2.f;
	}

	m_stTempPhysicsData.fAngleAccel += m_stTempPhysicsData.fSurfaceTorque * fDelta / m_stTempPhysicsData.fMass;
	m_stTempPhysicsData.fAngleVelocity += m_stTempPhysicsData.fAngleAccel * fDelta;

	if (fabs(m_stTempPhysicsData.fAngleVelocity) <= PHYSICSBODY_ANGLEVELOCITYEPSILON) m_stTempPhysicsData.fAngleVelocity = 0.f;

	m_stTempPhysicsData.fRotAngle += m_stTempPhysicsData.fAngleVelocity * fDelta;

	if (m_stTempPhysicsData.fAngleVelocity > 0.f) // x ¥Ô«Œ
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

	m_stTempPhysicsData.vAccel += m_stTempPhysicsData.vForce * fDelta / m_stTempPhysicsData.fMass;
	m_stTempPhysicsData.vVelocity += m_stTempPhysicsData.vAccel * fDelta;
	if (m_stTempPhysicsData.vVelocity.x > 0.f) // x ¥Ô«Œ
	{
		m_stTempPhysicsData.vVelocity.x -= m_stTempPhysicsData.vDamping.x * fDelta;
		if (m_stTempPhysicsData.vVelocity.x < 0.f) m_stTempPhysicsData.vVelocity.x = 0.f;
	}
	else if (m_stTempPhysicsData.vVelocity.x < 0.f)
	{
		m_stTempPhysicsData.vVelocity.x += m_stTempPhysicsData.vDamping.x * fDelta;
		if (m_stTempPhysicsData.vVelocity.x > 0.f) m_stTempPhysicsData.vVelocity.x = 0.f;
	}

	if (m_stTempPhysicsData.vVelocity.y > 0.f) // y ¥Ô«Œ
	{
		m_stTempPhysicsData.vVelocity.y -= m_stTempPhysicsData.vDamping.y * fDelta;
		if (m_stTempPhysicsData.vVelocity.y < 0.f) m_stTempPhysicsData.vVelocity.y = 0.f;
	}
	else if (m_stTempPhysicsData.vVelocity.y < 0.f)
	{
		m_stTempPhysicsData.vVelocity.y += m_stTempPhysicsData.vDamping.y * fDelta;
		if (m_stTempPhysicsData.vVelocity.y > 0.f) m_stTempPhysicsData.vVelocity.y = 0.f;
	}

	if (m_stTempPhysicsData.vVelocity.z > 0.f) // z ¥Ô«Œ
	{
		m_stTempPhysicsData.vVelocity.z -= m_stTempPhysicsData.vDamping.z * fDelta;
		if (m_stTempPhysicsData.vVelocity.z < 0.f) m_stTempPhysicsData.vVelocity.z = 0.f;
	}
	else if (m_stTempPhysicsData.vVelocity.z < 0.f)
	{
		m_stTempPhysicsData.vVelocity.z += m_stTempPhysicsData.vDamping.z * fDelta;
		if (m_stTempPhysicsData.vVelocity.z > 0.f) m_stTempPhysicsData.vVelocity.z = 0.f;
	}

	if (fabs(m_stTempPhysicsData.vVelocity.x) <= PHYSICSBODY_VELOCITYEPSILON) m_stTempPhysicsData.vVelocity.x = 0.f;
	if (fabs(m_stTempPhysicsData.vVelocity.y) <= PHYSICSBODY_VELOCITYEPSILON) m_stTempPhysicsData.vVelocity.y = 0.f;
	if (fabs(m_stTempPhysicsData.vVelocity.z) <= PHYSICSBODY_VELOCITYEPSILON) m_stTempPhysicsData.vVelocity.z = 0.f;

	m_stTempPhysicsData.vPos += m_stTempPhysicsData.vVelocity * fDelta;

	


	m_stTempPhysicsData.vForce = D3DXVECTOR3(0.f, 0.f, 0.f); // ø‹∑¬ √ ±‚»≠
}

void cPhysicsBody::UpdatePhysics(float fDelta)
{

	while (m_stPhysicsData.fRotAngle >= PI * 2.f || m_stPhysicsData.fRotAngle < 0) // ∞¢∫∏¡§
	{
		if (m_stPhysicsData.fRotAngle >= PI * 2.f) m_stPhysicsData.fRotAngle -= PI * 2.f;
		if (m_stPhysicsData.fRotAngle < 0) m_stPhysicsData.fRotAngle += PI * 2.f;
	}

	m_stPhysicsData.fAngleAccel += m_stPhysicsData.fSurfaceTorque * fDelta / m_stPhysicsData.fMass;
	m_stPhysicsData.fAngleVelocity += m_stPhysicsData.fAngleAccel * fDelta;

	if (fabs(m_stPhysicsData.fAngleVelocity) <= PHYSICSBODY_ANGLEVELOCITYEPSILON) m_stPhysicsData.fAngleVelocity = 0.f;

	m_stPhysicsData.fRotAngle += m_stPhysicsData.fAngleVelocity * fDelta;

	if (m_stPhysicsData.fAngleVelocity > 0.f) // x ¥Ô«Œ
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


	m_stPhysicsData.vAccel += m_stPhysicsData.vForce * fDelta / m_stPhysicsData.fMass;
	m_stPhysicsData.vVelocity += m_stPhysicsData.vAccel * fDelta;


	if (m_stPhysicsData.vVelocity.x > 0.f) // x ¥Ô«Œ
	{
		m_stPhysicsData.vVelocity.x -= m_stPhysicsData.vDamping.x * fDelta;
		if (m_stPhysicsData.vVelocity.x < 0.f) m_stPhysicsData.vVelocity.x = 0.f;
	}
	else if (m_stPhysicsData.vVelocity.x < 0.f)
	{
		m_stPhysicsData.vVelocity.x += m_stPhysicsData.vDamping.x * fDelta;
		if (m_stPhysicsData.vVelocity.x > 0.f) m_stPhysicsData.vVelocity.x = 0.f;
	}

	if (m_stPhysicsData.vVelocity.y > 0.f) // y ¥Ô«Œ
	{
		m_stPhysicsData.vVelocity.y -= m_stPhysicsData.vDamping.y * fDelta;
		if (m_stPhysicsData.vVelocity.y < 0.f) m_stPhysicsData.vVelocity.y = 0.f;
	}
	else if (m_stPhysicsData.vVelocity.y < 0.f)
	{
		m_stPhysicsData.vVelocity.y += m_stPhysicsData.vDamping.y * fDelta;
		if (m_stPhysicsData.vVelocity.y > 0.f)m_stPhysicsData.vVelocity.y = 0.f;
	}

	if (m_stPhysicsData.vVelocity.z > 0.f) // z ¥Ô«Œ
	{
		m_stPhysicsData.vVelocity.z -= m_stPhysicsData.vDamping.z * fDelta;
		if (m_stPhysicsData.vVelocity.z < 0.f) m_stPhysicsData.vVelocity.z = 0.f;
	}
	else if (m_stPhysicsData.vVelocity.z < 0.f)
	{
		m_stPhysicsData.vVelocity.z += m_stPhysicsData.vDamping.z * fDelta;
		if (m_stPhysicsData.vVelocity.z > 0.f) m_stPhysicsData.vVelocity.z = 0.f;
	}


	if (fabs(m_stPhysicsData.vVelocity.x) <= PHYSICSBODY_VELOCITYEPSILON) m_stPhysicsData.vVelocity.x = 0.f;
	if (fabs(m_stPhysicsData.vVelocity.y) <= PHYSICSBODY_VELOCITYEPSILON) m_stPhysicsData.vVelocity.y = 0.f;
	if (fabs(m_stPhysicsData.vVelocity.z) <= PHYSICSBODY_VELOCITYEPSILON) m_stPhysicsData.vVelocity.z = 0.f;

	m_stPhysicsData.vPos += m_stPhysicsData.vVelocity * fDelta;

	m_stPhysicsData.vForce = D3DXVECTOR3(0.f, 0.f, 0.f); // ø‹∑¬ √ ±‚»≠

}