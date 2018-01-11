#include "stdafx.h"
#include "cTimer_GetTickCount.h"


cTimer_GetTickCount::cTimer_GetTickCount()
{
}


cTimer_GetTickCount::~cTimer_GetTickCount()
{
}

void cTimer_GetTickCount::Setup()
{
	_lastTime = GetTickCount();
	_timeScale = 0.001f;
	
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;
	_secChecker = 0.0f;

	_isTickFPS = false;
}
void cTimer_GetTickCount::Update(float FPS)
{
	_curTime = GetTickCount();

	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	_isTickFPS = false;

	if (FPS > 0.0f)
	{
		_FPSTimeElapsed += _timeElapsed;
		if (_FPSTimeElapsed > (1.0f / FPS))
		{
			_isTickFPS = true;
			_FPSFrameCount++;
			while (_FPSTimeElapsed > (1.0f / FPS)) // TODO: 테스트 필요
			{
				_FPSTimeElapsed = _FPSTimeElapsed - (1.0f / FPS);
			}
		}
	}

	_lastTime = _curTime;
	_worldTime += _timeElapsed;
	_secChecker += _timeElapsed;

	if (_secChecker > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		while (_secChecker > 1.0f) // TODO: 테스트 필요
		{
			_secChecker = _secChecker - 1.0f;
		}
	}
}
float cTimer_GetTickCount::GetEllapsedTime()
{
	return _timeElapsed;
}
float cTimer_GetTickCount::GetWorldTime()
{
	return _worldTime;
}
float cTimer_GetTickCount::GetTickFPSEllapsedTime()
{
	return _FPSTimeElapsed;
}
unsigned long cTimer_GetTickCount::GetFrameRate()
{
	return _frameRate;
}
bool cTimer_GetTickCount::GetTickFPS()
{
	return _isTickFPS;
}