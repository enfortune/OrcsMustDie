#pragma once
#include "iTimer.h"
class cTimer_GetTickCount : public iTimer
{
private:
	float _timeScale;
	float _timeElapsed;

	__int64 _curTime;
	__int64 _lastTime;
	__int64 _periodFrequency;

	unsigned long _frameRate;
	unsigned long _FPSFrameCount;
	float _FPSTimeElapsed;
	float _worldTime;

	float _secChecker;

	bool _isTickFPS;
public:
	cTimer_GetTickCount();
	virtual ~cTimer_GetTickCount();

	virtual void Setup() override;
	virtual void Update(float FPS) override;
	virtual float GetEllapsedTime() override;
	virtual float GetWorldTime() override;
	virtual float GetTickFPSEllapsedTime() override;
	virtual unsigned long GetFrameRate() override;
	virtual bool GetTickFPS() override;
};

