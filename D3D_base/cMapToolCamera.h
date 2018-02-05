#pragma once
class cMapToolCamera
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;

	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	float			m_fCamDist;
	D3DXVECTOR3		m_vCamRotAngle;

	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;

public:
	cMapToolCamera();
	~cMapToolCamera();

	void Setup();
	void Update(float fDelta);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

