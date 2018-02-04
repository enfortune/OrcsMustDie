#include "stdafx.h"
#include "cMapToolCamera.h"

const float def_nCameraSpeed = 10.f;

cMapToolCamera::cMapToolCamera()
	: m_vEye(0, 5, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fCamDist(20.0f)
	, m_isLButtonDown(false)
	, m_vCamRotAngle(D3DX_PI / 4.f, D3DX_PI / 4.f, 0)
	, m_vPos(5.f, 0.f, 5.f)
	, m_vDir(0.f, 0.f, 1.f)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}

cMapToolCamera::~cMapToolCamera()
{
}

void cMapToolCamera::Setup()
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
}

void cMapToolCamera::Update(float fDelta)
{
	D3DXVECTOR3 vCross;
	D3DXVec3Cross(&vCross, &m_vDir, &m_vUp);
	if (g_pKeyManager->IsStayKeyDown('W'))
	{
		m_vPos += m_vDir * def_nCameraSpeed * fDelta;
	}
	if (g_pKeyManager->IsStayKeyDown('S'))
	{
		m_vPos -= m_vDir * def_nCameraSpeed * fDelta;
	}
	if (g_pKeyManager->IsStayKeyDown('A'))
	{
		m_vPos += vCross * def_nCameraSpeed * fDelta;
	}
	if (g_pKeyManager->IsStayKeyDown('D'))
	{
		m_vPos -= vCross * def_nCameraSpeed * fDelta;
	}

	D3DXMATRIXA16	matView, matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;

	D3DXVECTOR3 vTotalDir = D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVec3TransformCoord(&vTotalDir, &vTotalDir, &matR);

	m_vDir = D3DXVECTOR3(vTotalDir.x, 0.f, vTotalDir.z);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_vLookAt = m_vPos;
	m_vEye = m_vPos + vTotalDir * -m_fCamDist;

	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);

	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cMapToolCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) m_isLButtonDown = true;
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		break;

	case WM_LBUTTONUP:
		 m_isLButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) m_isLButtonDown = false;
		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);
			float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
			float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;

			m_vCamRotAngle.x += (fDeltaY / 100.0f);
			m_vCamRotAngle.y += (fDeltaX / 100.0f);
			if (m_vCamRotAngle.x < -(D3DX_PI / 2.0f - D3DX_PI / 16.f))
				m_vCamRotAngle.x = -(D3DX_PI / 2.0f - D3DX_PI / 16.f);
			if (m_vCamRotAngle.x > (D3DX_PI / 2.0f - D3DX_PI / 16.f))
				m_vCamRotAngle.x = (D3DX_PI / 2.0f - D3DX_PI / 16.f);

			if (m_vCamRotAngle.y < -D3DX_PI * 2.0f)
				m_vCamRotAngle.y += D3DX_PI * 2.0f;
			if (m_vCamRotAngle.y > D3DX_PI * 2.0f)
				m_vCamRotAngle.y -= D3DX_PI * 2.0f;

			/*if (m_vCamRotAngle.y < -D3DX_PI / 2.0f)
				m_vCamRotAngle.y = -D3DX_PI / 2.0f;
			if (m_vCamRotAngle.y > D3DX_PI / 2.0f)
				m_vCamRotAngle.y = D3DX_PI / 2.0f;*/

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
