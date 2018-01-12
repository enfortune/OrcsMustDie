#pragma once
class cTransformData
{
	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vAxis, Axis);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vScale, Scale);


public:
	cTransformData();
	~cTransformData();

	D3DXMATRIXA16 GetTransformMatrix();
};

