#pragma once
#define g_pTimeManager cTimeManager::GetInstance()

class iTimer;

class cTimeManager
{
	SINGLETONE(cTimeManager);
private:
	iTimer* m_pTimer;
	
public:
	void Setup();
	void Update(float FPS);
	float GetEllapsedTime();
	float GetWorldTime();
	float GetTickFPSEllapsedTime();
	unsigned long GetFrameRate();
	bool GetTickFPS();

	void Destroy();
}; 

