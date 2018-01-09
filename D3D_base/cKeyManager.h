#pragma once

#define KEYMAX 256

#define g_pKeyManager cKeyManager::GetInstance()

class cKeyManager
{
	SINGLETONE(cKeyManager);
private:
	std::bitset<KEYMAX> m_bsOnceKeyUp;
	std::bitset<KEYMAX> m_bsOnceKeyDown;
	std::bitset<KEYMAX> m_bsStayKeyDown;

public:
	void Setup(void);
	void Update(void);

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);
	bool IsToggleKey(int key);
};

