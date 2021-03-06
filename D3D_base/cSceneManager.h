#pragma once
#define g_pSceneManager cSceneManager::GetInstance()

class cGameScene;

class cSceneManager
{
	SINGLETONE(cSceneManager);
private:
	std::vector<cGameScene*> m_stackScene;

	cGameScene* m_pRunningScene;

	std::queue<cGameScene*> m_quePreScene;
	std::queue<cGameScene*> m_quePostScene;
	std::queue<cGameScene*> m_quePushScene;

	bool m_bSceneChangeInterrupt;
public:
	void Setup(cGameScene* startScene);
	void PreUpdate();
	void Update(float fDelta = g_pTimeManager->GetEllapsedTime());
	void Render(void);
	void Destroy();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	cGameScene* GetRunningScene(bool isAddRef = false); // default : non add Ref Cnt;

	void PopScene();
	void PushScene(cGameScene* sceneToPush, bool isUpdateInStack = false); // ������ ���� ���ÿ� �а� �� ���� ����.
	void ReplaceScene(cGameScene* sceneToReplace); // ���� �������� ���� �ش� ������ �ٲ�. ( == changeScene)

	void ClearScene(); // ���� �� ����
	void ClearSceneStack(); // ���� �������� ���� �����ϰ� �������� �� ����
	std::vector<cGameScene*>& GetStackOfScene(); // �� ������ ������.
};

