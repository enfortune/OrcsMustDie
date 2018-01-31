#pragma once
#include "cGameNode.h"

class cSkinnedMesh;
class cPlayer : public cGameNode
{
protected:
	cSkinnedMesh *	m_pPlayerMesh;

	//float			m_pRotationY;
	SYNTHESIZE_REF(float, m_pRotationY, RotationY);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPlayerPos, PlayerPos);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPlayerDir, PlayerDir);

public:
	cPlayer();
	~cPlayer();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

