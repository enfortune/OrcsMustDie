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
	m_pRunningScene = startScene; // ref cnt = 1; (할당한 순간 만들어지므로)
}
void cSceneManager::Update(float fDelta)
{
	//1. 현재 씬 업데이트 실행
	if (m_pRunningScene != nullptr)
		m_pRunningScene->Update();

	//2. 스택의 씬 업데이트 선택적 실행 : default상태라면 업데이트는 실행하지 않음.
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
	//렌더는 실행중인 씬에 대해서만 실행함. (씬을 alphablend로 여러개 동시송출을 하겠다면 추후 변경하겠음.)
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
	//따로 어딘가에 저장할생각이 아니라면 false(default값)을 사용하면 됨
	if (isAddRef) m_pRunningScene->AddRef();
	return m_pRunningScene;
}

void cSceneManager::PopScene()
{
	//1. 현재 실행중인 씬을 삭제
	SAFE_RELEASE(m_pRunningScene);
	
	//2. 씬스택에 씬이 남아있다면, 스택의 Top에 있는 씬을 현재 씬으로 변경
	if (m_stackScene.size() > 0)
	{
		m_pRunningScene = m_stackScene[m_stackScene.size() - 1];
		m_stackScene.pop_back();
	}
		
}
void cSceneManager::PushScene(cGameScene* sceneToPush, bool isUpdateInStack) // 기존의 씬을 스택에 밀고 새 씬을 넣음.
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