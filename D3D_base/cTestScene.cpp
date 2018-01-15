#include "stdafx.h"
#include "cTestScene.h"
#include "cGameSprite.h"
#include "cGameUIButton.h"

#define SCREEN_WIDTH GetRectWidth(GetScreenRect())
#define SCREEN_HEIGHT GetRectHeight(GetScreenRect())

cTestScene::cTestScene()
	: m_pButtonRender(NULL)
	, m_pButtonLeft(NULL)
	, m_pButtonRight(NULL)
{
}


cTestScene::~cTestScene()
{
	this->Delete();
}

void cTestScene::Setup()
{
	cGameScene::Setup();


	this->ButtonSetup();
	this->SpriteSetup();

	m_bSpriteRender = false;
	m_nSpriteIndex = 0;

}
void cTestScene::Update(float fDelta)
{
	
	cGameScene::Update(fDelta);
}
void cTestScene::Render()
{
	/*
	Update, Render, Destroy, WndProc 함수는
	클래스 자신의 함수 내용을 돌린 후에
	부모클래스의 렌더를 돌려야 함. (Setup은 부모먼저)
	*/

	if (m_bSpriteRender)
	{
		m_spritePoke[m_nSpriteIndex]->Render(); 
	}
		

	cGameScene::Render();  

	
}
void cTestScene::Delete()
{
	this->DeleteButton();
	this->DeleteSprite();
}

void cTestScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	cGameScene::WndProc(hWnd, message, wParam, lParam);
}

void cTestScene::ButtonSetup()
{
	m_pButtonRender = new cGameUIButton;
	m_pButtonRender->Setup("Render", this,
		"UITest/btn-med-up.png",	// 디폴트 스프라이트
		"UITest/btn-med-over.png",	// 마우스 올려놨을때 스프라이트
		"UITest/btn-med-down.png"); // 눌렀을 때 스프라이트
	m_pButtonRender->SetPosition(D3DXVECTOR2(
		SCREEN_WIDTH / 2.f,
		SCREEN_HEIGHT - 100));
	m_pButtonRender->SetEnable(true); // 기본으로 true 들어가있음. false 넣으면 안눌러짐
	this->AddChild(m_pButtonRender);  // 씬에 차일드로 붙이기만 하면 업데이트,렌더 등등을 신경쓰지 않아도 됨.
	m_pButtonRender->SetRotAngle(PI / 8.f); // 버튼 회전 테스트

	m_pButtonLeft = new cGameUIButton;
	m_pButtonLeft->Setup("Left", this,
		"UITest/arrow-btn-l-up.png"); // 디폴트 스프라이트 하나만 사용하는것도 가능
	m_pButtonLeft->SetPosition(D3DXVECTOR2(
		SCREEN_WIDTH / 2.f - 300,
		SCREEN_HEIGHT / 2.f));
	this->AddChild(m_pButtonLeft);

	m_pButtonRight = new cGameUIButton;
	m_pButtonRight->Setup("Left", this,
		"UITest/arrow-btn-r-up.png",
		"UITest/arrow-btn-r-over.png", 
		"UITest/arrow-btn-r-down.png");
	m_pButtonRight->SetPosition(D3DXVECTOR2(
		SCREEN_WIDTH / 2.f + 300,
		SCREEN_HEIGHT / 2.f));
	this->AddChild(m_pButtonRight);


}

void cTestScene::SpriteSetup()
{
	for (int i = 0; i < 5; i++)
	{
		m_spritePoke[i] = new cGameSprite;
		m_spritePoke[i]->SetPosition(D3DXVECTOR2(
			SCREEN_WIDTH / 2.f,
			SCREEN_HEIGHT / 2.f)); // 위치 조정
		m_spritePoke[i]->SetScale(D3DXVECTOR2(0.3f, 0.3f)); // 크기 조정
	}

	m_spritePoke[0]->Setup("UITest/파이리.png");
	m_spritePoke[1]->Setup("UITest/이상해씨.png");
	m_spritePoke[2]->Setup("UITest/꼬부기.png");
	m_spritePoke[3]->Setup("UITest/피카츄.png");
	m_spritePoke[4]->Setup("UITest/이브이.png");
}

void cTestScene::DeleteButton()
{
	// cGameUIButton 클래스는 GameNode를 상속받은 클래스이므로
	// SAFE_DELETE 대신 SAFE_RELEASE를 사용해야 함을 주의.

	SAFE_RELEASE(m_pButtonRender);
	SAFE_RELEASE(m_pButtonLeft);
	SAFE_RELEASE(m_pButtonRight);
}
void cTestScene::DeleteSprite()
{
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_spritePoke[i]);
	}
}

void cTestScene::LeftButtonAction(std::string sTag, BUTTON_STATE enState)
{
	if (sTag == "Left")
	{
		switch (enState)
		{
			case BUTTON_STATE::UP:
				m_nSpriteIndex--;
				if (m_nSpriteIndex < 0) m_nSpriteIndex = 4;
			break;
		}
	}
}
void cTestScene::RightButtonAction(std::string sTag, BUTTON_STATE enState)
{
	if (sTag == "Right")
	{
		switch (enState)
		{
			case BUTTON_STATE::UP:
				m_nSpriteIndex++;
				if (m_nSpriteIndex > 4) m_nSpriteIndex = 0;
			break;
		}
	}
}

/* iGameUIDelegate */
bool cTestScene::UIButtonAction(std::string sTag, BUTTON_STATE enState)
{
	if (sTag == "Render") // 버튼의 태그 값
	{
		switch (enState)  // 버튼의 상태 값 (눌렀는지 누르고있는지 뗐는지)
		{
			case BUTTON_STATE::UP:  // UP은 버튼을 눌렀다가 "뗐을 때" 한번 적용됨.
				m_bSpriteRender = !m_bSpriteRender; // 이런식으로 코드를 직접 넣어도 되고
			break;
		}
	}
	
	this->LeftButtonAction(sTag, enState); // 이렇게 함수로 빼도 됨
	this->RightButtonAction(sTag, enState);

	return true;
}