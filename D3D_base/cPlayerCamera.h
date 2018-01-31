#pragma once

class cPlayerCamera
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;

	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	float			m_fCamDist;
	SYNTHESIZE_REF(D3DXVECTOR3,	m_vCamRotAngle, CamRotAngle);

	D3DXVECTOR3*	m_pvTargetPos;
	D3DXVECTOR3*	m_pvTargetDir;

	float			m_pPlayerRotationY_C;

public:
	cPlayerCamera();
	~cPlayerCamera();

	void Setup(D3DXVECTOR3* pvTargetPos, float vRotAngle/*, D3DXVECTOR3* pvTargetDir*/);
	void Update();
	void Delete();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};