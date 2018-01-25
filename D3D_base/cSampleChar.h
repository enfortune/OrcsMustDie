#pragma once
#include "cChaNode.h"

class cSkinnedMesh;

class cSampleChar : public cChaNode
{
private:
	bool move1, move2, move3;
public:
	cSampleChar();
	virtual ~cSampleChar();


	virtual void Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup) override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	void Move(D3DXVECTOR3 vGoal, float fDelta);
	void Attack(D3DXVECTOR3 vPlayer);

};

