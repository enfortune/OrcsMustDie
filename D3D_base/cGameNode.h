#pragma once
#include "cObject.h"
#include "PhysicsTagDefine.h"

class cPhysicsBody;
class cTransformData;

class cGameNode : public cObject
{
protected:
	/* parent */
	SYNTHESIZE_REF(cGameNode*, m_pParent, ParentNode);

	/* child */
	SYNTHESIZE_REF(std::set<cGameNode*>, m_setChild, Children);

	/* transform */
	SYNTHESIZE_REF(cTransformData*, m_pTransformData, TransformData);

	/* physics */
	SYNTHESIZE_REF(std::string, m_sPhysicsTag, PhysicsTag);
	SYNTHESIZE_REF(cPhysicsBody*, m_pPhysicsBody, PhysicsBody);

public:
	cGameNode();
	virtual ~cGameNode();
	void Destroy();

	virtual void Setup(bool bUseTransformData = true);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void Render();
	virtual void Delete() = 0;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void UpdateTempPhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void UpdatePhysics(float fDelta = g_pTimeManager->GetEllapsedTime());
	virtual void CollisionWithNode(cGameNode* node);
	
	void AddChild(cGameNode* pNode);
	void RemoveChild(cGameNode* pNode);
	void RemoveAllChildren();
	void RemoveFromParent();

	virtual D3DXMATRIXA16 GetMatirixToParent();
	D3DXMATRIXA16 GetMatrixToWorld(); 
	//D3DXMATRIXA16 GetMatrixToViewPort();
};
