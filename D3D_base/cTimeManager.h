#pragma once
#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
	SINGLETONE(cTimeManager);
private:
	DWORD	m_dwLastUpdateTime;
	float	m_fEllapsedTime;

public:
	void Update();
	float GetEllapsedTime();
	float GetLastUpdateTime();
};

