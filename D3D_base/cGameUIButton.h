#pragma once
#include "cGameNode.h"

class iGameUIDelegate;
class cGameSprite;

typedef enum class EN_GAMEUIBUTTON_SPRITESTATE
{
	IDLE,
	ONMOUSE,
	PUSH,
	END
}BUTTON_SPRITESTATE;

class cGameUIButton : public cGameNode
{
	SYNTHESIZE(cGameSprite*, m_pSprite, Sprite);
	SYNTHESIZE(cGameSprite*, m_pSpriteOnMouse	, SpriteOnMouse);
	SYNTHESIZE(cGameSprite*, m_pSpritePush		, SpriteOnPush);
	SYNTHESIZE(iGameUIDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(bool, m_bEnable, Enable);


	SYNTHESIZE(BUTTON_STATE, m_enState, State);

	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle); // Z angle
	SYNTHESIZE_REF(D3DXVECTOR2, m_vScale, Scale);
protected:
	float					m_fButtonScale;
	BUTTON_SPRITESTATE		m_enSpriteState;
	std::string				m_sTag;
protected:
	void UpdateMatrixData();
public:
	cGameUIButton();
	~cGameUIButton();

	virtual void Setup(std::string sTag, iGameUIDelegate* pDelegate, std::string sSpriteFileName, std::string sSpriteOnMouseFileName = std::string(""), std::string sSpritePushFileName = std::string(""));
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Destroy() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

