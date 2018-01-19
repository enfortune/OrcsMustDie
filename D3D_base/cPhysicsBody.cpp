#include "stdafx.h"
#include "cPhysicsBody.h"


cPhysicsBody::cPhysicsBody()
	: m_vDir(0.0f, 0.0f, 1.0f)
	//, m_enShapeType(PHYSICSSHAPETYPE_END)
	, m_enBodyType(PHYSICSBODYTYPE_END)
	, m_vPos(0.0f, 0.0f, 0.0f)
	, m_vAxis(0.0f, 1.0f, 0.0f)
	, m_vRotAngle(0.0f)
	, m_fMass(0.0f)
	, m_vForce(0.0f, 0.0f, 0.0f)
	, m_vAccel(0.0f, 0.0f, 0.0f)
	, m_vVelocity(0.0f, 0.0f, 0.0f)
	, m_fElasticity(0.0f)
	, m_fFriction(0.0f)
{
}


cPhysicsBody::~cPhysicsBody()
{
}

void cPhysicsBody::UpdatePhysics()
{
	// TODO: 물리코드 추가 필요
}