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
	Update, Render, Destroy, WndProc �Լ���
	Ŭ���� �ڽ��� �Լ� ������ ���� �Ŀ�
	�θ�Ŭ������ ������ ������ ��. (Setup�� �θ����)
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
		"UITest/btn-med-up.png",	// ����Ʈ ��������Ʈ
		"UITest/btn-med-over.png",	// ���콺 �÷������� ��������Ʈ
		"UITest/btn-med-down.png"); // ������ �� ��������Ʈ
	m_pButtonRender->SetPosition(D3DXVECTOR2(
		SCREEN_WIDTH / 2.f,
		SCREEN_HEIGHT - 100));
	m_pButtonRender->SetEnable(true); // �⺻���� true ������. false ������ �ȴ�����
	this->AddChild(m_pButtonRender);  // ���� ���ϵ�� ���̱⸸ �ϸ� ������Ʈ,���� ����� �Ű澲�� �ʾƵ� ��.
	m_pButtonRender->SetRotAngle(PI / 8.f); // ��ư ȸ�� �׽�Ʈ

	m_pButtonLeft = new cGameUIButton;
	m_pButtonLeft->Setup("Left", this,
		"UITest/arrow-btn-l-up.png"); // ����Ʈ ��������Ʈ �ϳ��� ����ϴ°͵� ����
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
			SCREEN_HEIGHT / 2.f)); // ��ġ ����
		m_spritePoke[i]->SetScale(D3DXVECTOR2(0.3f, 0.3f)); // ũ�� ����
	}

	m_spritePoke[0]->Setup("UITest/���̸�.png");
	m_spritePoke[1]->Setup("UITest/�̻��ؾ�.png");
	m_spritePoke[2]->Setup("UITest/���α�.png");
	m_spritePoke[3]->Setup("UITest/��ī��.png");
	m_spritePoke[4]->Setup("UITest/�̺���.png");
}

void cTestScene::DeleteButton()
{
	// cGameUIButton Ŭ������ GameNode�� ��ӹ��� Ŭ�����̹Ƿ�
	// SAFE_DELETE ��� SAFE_RELEASE�� ����ؾ� ���� ����.

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
	if (sTag == "Render") // ��ư�� �±� ��
	{
		switch (enState)  // ��ư�� ���� �� (�������� �������ִ��� �ô���)
		{
			case BUTTON_STATE::UP:  // UP�� ��ư�� �����ٰ� "���� ��" �ѹ� �����.
				m_bSpriteRender = !m_bSpriteRender; // �̷������� �ڵ带 ���� �־ �ǰ�
			break;
		}
	}
	
	this->LeftButtonAction(sTag, enState); // �̷��� �Լ��� ���� ��
	this->RightButtonAction(sTag, enState);

	return true;
}