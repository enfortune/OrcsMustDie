#pragma once
#include "enPhysicsBody.h"

typedef struct ST_PHYSICSSHAPEDATA
{
	EN_PHYSICSSHAPETYPE enShapeType;
	
	std::vector<D3DXVECTOR3>vecVertex; // Rectosahedron.
	D3DXVECTOR3 vCenter; // 무게 중심
	float fRadius; // Sphere
	
}PHYSICSSHAPEDATA;

typedef struct ST_PHYSICSDATA
{
	D3DXVECTOR3 vPos;
	float fOriginAngle;
	D3DXVECTOR3 vAxis;
	float fSurfaceTorque; // 표면을 기준으로 한 각속도 (r = radius)
	float fAngleAccel;
	float fAngleDamping;
	float fAngleVelocity;
	float fRotAngle;


	float fMass;
	D3DXVECTOR3 vForce;
	D3DXVECTOR3 vAccel;
	D3DXVECTOR3 vDamping;
	D3DXVECTOR3 vVelocity;

	float fElasticity;
	ST_PHYSICSDATA()
		: fOriginAngle(0.0f)
		, vPos(0.0f, 0.0f, 0.0f)
		, vAxis(0.0f, 1.0f, 0.0f)
		, fRotAngle(0.0f)
		, fSurfaceTorque(0.0f)
		, fAngleAccel(0.0f)
		, fAngleDamping(0.0f)
		, fAngleVelocity(0.0f)
		, fMass(0.0f)
		, vForce(0.0f, 0.0f, 0.0f)
		, vAccel(0.0f, 0.0f, 0.0f)
		, vDamping(0.0f, 0.0f, 0.0f)
		, vVelocity(0.0f, 0.0f, 0.0f)
		, fElasticity(0.0f)
	{}
}PHYSICSDATA;

class cPhysicsBody
{
	/* Physics Shape */
	SYNTHESIZE_REF(ST_PHYSICSSHAPEDATA, m_stShapeData, ShapeData);

	/* Physics Type */
	SYNTHESIZE_REF(EN_PHYSICSBODYTYPE, m_enBodyType, BodyType);
	SYNTHESIZE_REF(bool, m_bSpinEnable, SpinEnable);

	/* Temp Physics Data */
	SYNTHESIZE_REF(ST_PHYSICSDATA, m_stTempPhysicsData, TempPhysicsData);

	/* Physics Data */
	SYNTHESIZE_REF(ST_PHYSICSDATA, m_stPhysicsData, PhysicsData);

public:
	cPhysicsBody();
	~cPhysicsBody();

	void Setup(float fOriginAngle);
	void MakeBodyShape(ST_PHYSICSSHAPEDATA stShapeData);
	void MakeBodyRectosahedron(float fX, float fY, float fZ, D3DXVECTOR3 vCenter); // Center는 x,y,z,값의 중점으로 잡는걸 권장
	void MakeBodySphere(float fRadius, D3DXVECTOR3 vCenter);

	void RenderBody();

	void UpdateTempPhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
	void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
};

