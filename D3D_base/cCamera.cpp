#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
	: m_vEye(0, 5, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fCamDist(5.0f)
	, m_isLButtonDown(false)
	, m_vCamRotAngle(0, 0, 0)
	, m_pvTarget(NULL)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}


cCamera::~cCamera()
{
}

void cCamera::Setup(D3DXVECTOR3* pvTarget)
{
	D3DXMATRIXA16	matProj;

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f,
		(float)rc.right / rc.bottom,
		1.0f,
		1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pvTarget = pvTarget;
}

void cCamera::Update()
{
	D3DXMATRIXA16	matView, matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;

	m_vEye = D3DXVECTOR3(0, 0, -m_fCamDist);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	if (m_pvTarget)
	{
		m_vLookAt = (*m_pvTarget);
		m_vEye = m_vEye + (*m_pvTarget);
	}

	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);

	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_isLButtonDown = true;
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		break;

	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);
			float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
			float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;

			m_vCamRotAngle.x += (fDeltaY / 100.0f);
			m_vCamRotAngle.y += (fDeltaX / 100.0f);
			if (m_vCamRotAngle.x < -D3DX_PI / 2.0f)
				m_vCamRotAngle.x = -D3DX_PI / 2.0f;
			if (m_vCamRotAngle.x > D3DX_PI / 2.0f)
				m_vCamRotAngle.x = D3DX_PI / 2.0f;
			m_ptPrevMouse = ptCurrMouse;
		}
		break;

	case WM_MOUSEWHEEL:
		m_fCamDist -= (GET_WHEEL_DELTA_WPARAM(wParam) / 40.0f);
		if (m_fCamDist < 0.000001f)
			m_fCamDist = 0.000001f;
		break;
	}
}
