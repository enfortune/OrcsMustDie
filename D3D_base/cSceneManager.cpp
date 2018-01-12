#include "stdafx.h"
#include "cSceneManager.h"
#include "cGameScene.h"

cSceneManager::cSceneManager()
	: m_pRunningScene(nullptr)
{
}


cSceneManager::~cSceneManager()
{
	this->Destroy();
}


void cSceneManager::Setup(cGameScene* startScene)
{
	if (m_pRunningScene != nullptr)
	{
		SAFE_RELEASE(m_pRunningScene);
	}

	if (startScene == nullptr) return;

	startScene->Setup();
	m_pRunningScene = startScene; // ref cnt = 1; (�Ҵ��� ���� ��������Ƿ�)
}
void cSceneManager::Update(float fDelta)
{
	//1. ���� �� ������Ʈ ����
	if (m_pRunningScene != nullptr)
		m_pRunningScene->Update();

	//2. ������ �� ������Ʈ ������ ���� : default���¶�� ������Ʈ�� �������� ����.
	int nIndex = (int)m_stackScene.size() - 1;
	while (nIndex >= 0)
	{
		if (m_stackScene[nIndex]->GetUpdateEnable() == false)
		{
			nIndex--;
			continue;
		}
		m_stackScene[nIndex--]->Update(fDelta);
	}
}
void cSceneManager::Render(void)
{
	//������ �������� ���� ���ؼ��� ������. (���� alphablend�� ������ ���ü����� �ϰڴٸ� ���� �����ϰ���.)
	if (m_pRunningScene != nullptr)
		m_pRunningScene->Render();
}
void cSceneManager::Destroy()
{
	this->ClearScene();
}
void cSceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pRunningScene != nullptr)
		m_pRunningScene->WndProc(hWnd, message, wParam, lParam);
}

cGameScene* cSceneManager::GetRunningScene(bool isAddRef)
{
	//���� ��򰡿� �����һ����� �ƴ϶�� false(default��)�� ����ϸ� ��
	if (isAddRef) m_pRunningScene->AddRef();
	return m_pRunningScene;
}

void cSceneManager::PopScene()
{
	//1. ���� �������� ���� ����
	SAFE_RELEASE(m_pRunningScene);
	
	//2. �����ÿ� ���� �����ִٸ�, ������ Top�� �ִ� ���� ���� ������ ����
	if (m_stackScene.size() > 0)
	{
		m_pRunningScene = m_stackScene[m_stackScene.size() - 1];
		m_stackScene.pop_back();
	}
		
}
void cSceneManager::PushScene(cGameScene* sceneToPush, bool isUpdateInStack) // ������ ���� ���ÿ� �а� �� ���� ����.
{
	if (sceneToPush == nullptr) return;

	if (m_pRunningScene == nullptr) this->ReplaceScene(sceneToPush);
	else
	{
		m_pRunningScene->SetUpdateEnable(isUpdateInStack);
		m_stackScene.push_back(m_pRunningScene);

		sceneToPush->Setup();
		m_pRunningScene = sceneToPush;
	}
}
void cSceneManager::ReplaceScene(cGameScene* sceneToReplace)
{
	//if (sceneToReplace == nullptr) return;

	cGameScene* buff = m_pRunningScene;
	m_pRunningScene = sceneToReplace;
	SAFE_RELEASE(buff);
}

void cSceneManager::ClearScene()
{
	SAFE_RELEASE(m_pRunningScene);

	this->ClearSceneStack();
}
void cSceneManager::ClearSceneStack()
{
	for (size_t i = 0; i < m_stackScene.size(); i++)
	{
		SAFE_RELEASE(m_stackScene[i]);
	}
	m_stackScene.clear();
}
std::vector<cGameScene*>& cSceneManager::GetStackOfScene()
{
	return m_stackScene;
}