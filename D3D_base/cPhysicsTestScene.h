#pragma once
#include "cGameScene.h"
class cMapData;
class cPhysicsNode;
class cCamera;
class cGrid;
class cSampleChar;

class cPhysicsTestScene : public cGameScene
{
	cCamera* m_pCamera;
	cGrid*   m_pGrid;
	
	cMapData* m_pMapData;
	cPhysicsNode* m_pSpace;

	cSampleChar* m_pEnemy;
	cSampleChar* m_pEnemy2;
	cSampleChar* m_pEnemy3;
public:
	cPhysicsTestScene();
	~cPhysicsTestScene();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

