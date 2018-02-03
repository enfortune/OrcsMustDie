#include "stdafx.h"
#include "cPlayerCamera.h"

cPlayerCamera::cPlayerCamera()
	: m_vEye(0, 5, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fCamDist(5.0f)
	, m_isLButtonDown(false)
	, m_vCamRotAngle(0, 0, 0)
	, m_pvTargetPos(NULL)
	, m_pvTargetDir(NULL)
	, m_pPlayerRotationY_C(0.0f)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}

cPlayerCamera::~cPlayerCamera()
{
}

void cPlayerCamera::Setup(D3DXVECTOR3* pvTargetPos, float vRotAngle/*, D3DXVECTOR3* pvTargetDir*/)
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

	m_pvTargetPos = pvTargetPos;
	//m_pvTargetDir = pvTargetDir;
	m_pPlayerRotationY_C = vRotAngle;

	RECT rcC; GetClientRect(g_hWnd, &rcC);
	RECT rcW; GetWindowRect(g_hWnd, &rcW);

	SetCursorPos(GetRectCenter(rcW).x, GetRectCenter(rcW).y);
	m_ptPrevMouse = GetRectCenter(rcC);
}

void cPlayerCamera::Update()
{
	POINT ptCurrMouse;
	
	float fDeltaX = 0.f;
	float fDeltaY = 0.f;

	if (g_bIsClientActivated)
	{
		ptCurrMouse = g_ptMouse;

		if (g_pKeyManager->IsToggleKey(VK_CONTROL))
		{
			fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
			fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;

			RECT rcC; GetClientRect(g_hWnd, &rcC);

			ptCurrMouse = GetRectCenter(rcC);
			ClientToScreen(g_hWnd, &ptCurrMouse);
			SetCursorPos(ptCurrMouse.x, ptCurrMouse.y);
			ptCurrMouse = GetRectCenter(rcC);
			m_ptPrevMouse = ptCurrMouse;
		}
		else
		{
			if (g_pKeyManager->IsStayKeyDown(VK_LBUTTON))
			{
				ptCurrMouse = g_ptMouse;
				fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
				fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;
			}
			m_ptPrevMouse = g_ptMouse;
		}
	}

	

	if (fabs(fDeltaX) + fabs(fDeltaY) > 1.f)
	{
		m_vCamRotAngle.x += (fDeltaY / 150.0f);
		m_vCamRotAngle.y += (fDeltaX / 150.0f);
	}

	if (m_vCamRotAngle.x < -D3DX_PI / 4.0f)
	{
		m_vCamRotAngle.x = -D3DX_PI / 4.0f;
	}
	if (m_vCamRotAngle.x > D3DX_PI / 4.0f)
	{
		m_vCamRotAngle.x = D3DX_PI / 4.0f;
	}
	
	D3DXMATRIXA16	matView, matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;

	m_vEye = D3DXVECTOR3(0, 0, -m_fCamDist);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	if (m_pvTargetPos)
	{
		m_vLookAt = (*m_pvTargetPos + D3DXVECTOR3(0,0.7,0));
		if (!m_pvTargetDir) m_vEye = m_vEye + (*m_pvTargetPos);
		else
		{
			m_vEye = D3DXVECTOR3(-m_pvTargetDir->x, 0, -m_pvTargetDir->z);
			m_vEye *= m_fCamDist;
			m_vEye.y = m_fCamDist / 3;
		
			m_vEye = m_vEye + (*m_pvTargetPos);
		}
	}
	
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cPlayerCamera::Delete()
{
}

void cPlayerCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_isLButtonDown = true;
		break;

	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEWHEEL:
		m_fCamDist -= (GET_WHEEL_DELTA_WPARAM(wParam) / 40.0f);
		if (m_fCamDist < 0.000001f)
			m_fCamDist = 0.000001f;
		break;
	}
}
