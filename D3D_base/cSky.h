#pragma once
#include "cGameNode.h"

class cSkinnedMeshEX;

class cSky : public cGameNode
{
	cSkinnedMeshEX* m_pskyUp;
	cSkinnedMeshEX* m_pskyDown;

	SYNTHESIZE_REF(D3DXVECTOR3, m_vPos, Pos);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vScl, Scl);
public:
	cSky();
	~cSky();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void Render();
	virtual void Delete();
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

