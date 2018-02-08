#pragma once
#include "cGameNode.h"

class cSkinnedMeshEX;
class cSubject : public cGameNode
{
private:
	cSkinnedMeshEX* m_pZeppelin;
	cSkinnedMeshEX* m_pAnduin;

	SYNTHESIZE_REF(D3DXVECTOR3, m_vZPos, ZPos);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vZScl, ZScl);

	SYNTHESIZE_REF(D3DXVECTOR3, m_vAPos, APos);

public:
	cSubject();
	~cSubject();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void Render();
	virtual void Delete();
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

