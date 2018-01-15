#pragma once
#include "cGameScene.h"
#include "iGameUIDelegate.h"

class cGameSprite;
class cGameUIButton;

class cTestScene : public cGameScene, public iGameUIDelegate
{
	friend class iGameUIDelegate;
private:
	cGameSprite*	m_spritePoke[5];
	int				m_nSpriteIndex;

	cGameUIButton*	m_pButtonRender;
	cGameUIButton*	m_pButtonLeft;
	cGameUIButton*  m_pButtonRight;

	bool			m_bSpriteRender;
	
public:
	cTestScene();
	~cTestScene();

	virtual void Setup() override;
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	void ButtonSetup();
	void SpriteSetup();
	void DeleteButton();
	void DeleteSprite();
	void LeftButtonAction(std::string sTag, BUTTON_STATE enState);
	void RightButtonAction(std::string sTag, BUTTON_STATE enState);

public: // iGameUIDelegate
	virtual bool UIButtonAction(std::string sTag, BUTTON_STATE enState);
};

