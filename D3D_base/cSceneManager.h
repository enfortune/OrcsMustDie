#pragma once
#define g_pSceneManager cSceneManager::GetInstance()

class cGameScene;

class cSceneManager
{
	SINGLETONE(cSceneManager);
private:
	std::vector<cGameScene*> m_stackScene;

	cGameScene* m_pRunningScene;

	//bool _isChangeScene;
public:
	void Setup(cGameScene* startScene);
	void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	void Render(void);
	void Destroy();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	cGameScene* GetRunningScene(bool isAddRef = false); // default : non add Ref Cnt;

	void PopScene();
	void PushScene(cGameScene* sceneToPush, bool isUpdateInStack = false); // 기존의 씬을 스택에 밀고 새 씬을 넣음.
	void ReplaceScene(cGameScene* sceneToReplace); // 현재 실행중인 씬을 해당 씬으로 바꿈. ( == changeScene)

	void ClearScene(); // 씬을 다 지움
	void ClearSceneStack(); // 현재 진행중인 씬을 제외하고 씬스택을 다 지움
	std::vector<cGameScene*>& GetStackOfScene(); // 씬 스택을 가져옴.
};

