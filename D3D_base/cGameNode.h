#pragma once
#include "cGameObject.h"

class cPhysicsBody;

class cGameNode :
	public cGameObject
{
	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vAxis, Axis);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle);
	SYNTHESIZE_REF(D3DXVECTOR3, m_vScale, Scale);

	/* parent */
	SYNTHESIZE_REF(cGameNode*, m_pParent, ParentNode);

	/* child */
	SYNTHESIZE_REF(std::set<cGameNode*>, m_setChild, Children);

	/* physics */
	SYNTHESIZE_REF(std::string, m_sPhysicsTag, PhysicsTag);
	SYNTHESIZE_REF(cPhysicsBody*, m_pPhysicsBody, PhysicsBody);

public:
	cGameNode();
	virtual ~cGameNode();

	virtual void Setup();
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
	
	void AddChild(cGameNode* node);
	void RemoveChild(cGameNode* node);
	void RemoveFromParent();
	D3DXMATRIXA16 GetMatirixToParent();
	D3DXMATRIXA16 GetMatrixToWorld(); 
};

