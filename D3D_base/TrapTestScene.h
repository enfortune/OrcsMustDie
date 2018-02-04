#pragma once

#include "cGameScene.h"
#include "TrapTypeManager.h"

#include "cGrid.h"
#include "cCamera.h"

#include "Trap.h"

#include "cSkinnedMeshEX.h"

class TrapTestScene : public cGameScene
{
private:
	TrapTypeManager trapTypeManager_;

	cGrid grid_;
	cCamera camera_;

	cSkinnedMeshEX barricade_;

	std::vector<Trap> trapList_;

public:
	TrapTestScene();
	virtual ~TrapTestScene();

public:
	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

