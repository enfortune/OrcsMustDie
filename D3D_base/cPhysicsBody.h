#pragma once
#include "enPhysicsBody.h"

typedef struct ST_PHYSICSSHAPEDATA
{
	SYNTHESIZE_REF(EN_PHYSICSSHAPETYPE, m_enShapeType, ShapeType);

	SYNTHESIZE_REF(std::vector<ST_PTN_VERTEX>, m_vecVertex, Vertex); // cube. (0,0,0)를 중심기준으로
	SYNTHESIZE_REF(float, m_fRadius, Radius); // sphere
	
}PHYSICSSHAPEDATA;

class cPhysicsBody
{
	/* Physics Shape */
	SYNTHESIZE_REF(ST_PHYSICSSHAPEDATA, m_stShapeData, ShapeData);

	/* Physics Type */
	SYNTHESIZE_REF(EN_PHYSICSBODYTYPE, m_enBodyType, BodyType);

	/* Physics Data */
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vDir, Direction);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vAxis, Axis);
	SYNTHESIZE_REF(float, m_vRotAngle, RotAngle);
	
	SYNTHESIZE_REF(float, m_fMass, Mass);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vForce, Force);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vAccel, Accel);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vVelocity, Velocity);

	SYNTHESIZE_REF(float, m_fElasticity, Elasticity);
	SYNTHESIZE_REF(float, m_fFriction, Friction);

	//SYNTHESIZE_REF(bool, m_bIsSpin, IsSpin);  // 이건 나중에 필요할 때 추가하면 될것임
public:
	cPhysicsBody();
	~cPhysicsBody();

	void UpdatePhysics();
};

