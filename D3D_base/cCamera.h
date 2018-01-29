#pragma once
class cCamera
{
private:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;

	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	float			m_fCamDist;
	D3DXVECTOR3		m_vCamRotAngle;

	D3DXVECTOR3*	m_pvTarget;

public:
	inline void setTarget(D3DXVECTOR3 * pTarget) { m_pvTarget = pTarget; }

public:
	cCamera();
	~cCamera();

	void Setup(D3DXVECTOR3* pvTarget = nullptr);
	void Update();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

