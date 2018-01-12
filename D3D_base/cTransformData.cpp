#include "stdafx.h"
#include "cTransformData.h"


cTransformData::cTransformData()
	: m_vPos(0.0f, 0.0f, 0.0f)
	, m_vAxis(0.0f, 1.0f, 0.0f)
	, m_fRotAngle(0.0f)
	, m_vScale(0.0f, 0.0f, 0.0f)
{
}


cTransformData::~cTransformData()
{
}

D3DXMATRIXA16 cTransformData::GetTransformMatrix()
{
	D3DXMATRIXA16 matTransform, matS, matR, matT;
	D3DXMatrixIdentity(&matTransform);

	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationAxis(&matR, &m_vAxis, m_fRotAngle);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	matTransform = matS * matR * matT;
	return matTransform;
}