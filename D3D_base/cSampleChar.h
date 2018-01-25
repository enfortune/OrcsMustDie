#pragma once
#include "cChaNode.h"

class cSkinnedMesh;

class cSampleChar : public cChaNode
{
private:

public:
	cSampleChar();
	virtual ~cSampleChar();


	virtual void Setup(bool move, D3DXVECTOR3 vPosSetup) override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

};

