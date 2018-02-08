#include "stdafx.h"
#include "cSceneManager.h"
#include "cGameScene.h"

cSceneManager::cSceneManager()
	: m_pRunningScene(nullptr)
	, m_bSceneChangeInterrupt(false)
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
void cSceneManager::PreUpdate()
{
	m_bSceneChangeInterrupt = true;
	//1. 푸쉬 큐 처리
	if (m_quePushScene.size() > 0)
	{
		while (m_quePushScene.size() > 0)
		{
			m_stackScene.push_back(m_quePushScene.front());
			m_quePushScene.pop();
		}
	}
	
	//2. 다음씬 큐 처리
	if (m_quePostScene.size() > 0)
	{
		while (m_quePostScene.size() > 1)
		{
			m_quePreScene.push(m_quePostScene.front());
			m_quePostScene.pop();
		}
		m_pRunningScene = m_quePostScene.front();
		m_quePostScene.pop();
	}

	//3. 지난 씬 삭제처리
	while (m_quePreScene.size() > 0)
	{
		if (m_pRunningScene == m_quePreScene.front()) m_pRunningScene = nullptr;
		SAFE_RELEASE(m_quePreScene.front());
		m_quePreScene.pop();
	}

	m_bSceneChangeInterrupt = false;
}
void cSceneManager::Update(float fDelta)
{
	//1. 현재 씬 업데이트 실행
	if (m_pRunningScene != nullptr)
		m_pRunningScene->Update(fDelta);

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
	this->PreUpdate();
}
void cSceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pRunningScene != nullptr && m_bSceneChangeInterrupt == false)
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
	//1. 현재 실행중인 씬을 삭제 대기씬에 등록
	m_quePreScene.push(m_pRunningScene);
	
	//2. 씬스택에 씬이 남아있다면, 스택의 Top에 있는 씬을 다음 씬으로 등록
	if (m_stackScene.size() > 0)
	{
		m_quePostScene.push(m_stackScene[m_stackScene.size() - 1]);
		m_stackScene.pop_back();
	}
	else
	{
		m_quePostScene.push(nullptr);
	}
		
}
void cSceneManager::PushScene(cGameScene* sceneToPush, bool isUpdateInStack) // 기존의 씬을 스택에 밀고 새 씬을 넣음.
{
	if (sceneToPush == nullptr) return;

	if (m_pRunningScene == nullptr) this->ReplaceScene(sceneToPush);
	else
	{
		m_pRunningScene->SetUpdateEnable(isUpdateInStack);
		m_quePushScene.push(m_pRunningScene);

		sceneToPush->Setup();
		m_quePostScene.push(sceneToPush);
	}
}
void cSceneManager::ReplaceScene(cGameScene* sceneToReplace)
{
	//if (sceneToReplace == nullptr) return;

	m_quePreScene.push(m_pRunningScene);

	sceneToReplace->Setup();
	m_quePostScene.push(sceneToReplace);
}

void cSceneManager::ClearScene()
{
	//m_pRunningScene->RemoveAllChildren();
	m_quePreScene.push(m_pRunningScene);
	m_quePostScene.push(nullptr);

	this->ClearSceneStack();
}
void cSceneManager::ClearSceneStack()
{
	for (size_t i = 0; i < m_stackScene.size(); i++)
	{
		//m_stackScene[i]->RemoveAllChildren();
		SAFE_RELEASE(m_stackScene[i]);
	}
	m_stackScene.clear();
}
std::vector<cGameScene*>& cSceneManager::GetStackOfScene()
{
	return m_stackScene;
}