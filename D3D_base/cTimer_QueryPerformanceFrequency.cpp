#include "stdafx.h"
#include "cTimer_QueryPerformanceFrequency.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

cTimer_QueryPerformanceFrequency::cTimer_QueryPerformanceFrequency()
{
}


cTimer_QueryPerformanceFrequency::~cTimer_QueryPerformanceFrequency()
{
}


void cTimer_QueryPerformanceFrequency::Setup()
{
	//IRC
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;


		//호오오오옥시이이이나아아아아 씨피유가 고성능이 아니면
		_lastTime = timeGetTime();	//밀리세컨으로 고정
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;
	_secChecker = 0.0f;

	_isTickFPS = false;
}
void cTimer_QueryPerformanceFrequency::Update(float FPS)
{
	if (_isHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		_curTime = timeGetTime();
	}

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
float cTimer_QueryPerformanceFrequency::GetEllapsedTime()
{
	return _timeElapsed;
}
float cTimer_QueryPerformanceFrequency::GetWorldTime()
{
	return _worldTime;
}
float cTimer_QueryPerformanceFrequency::GetTickFPSEllapsedTime()
{
	return _FPSTimeElapsed;
}
unsigned long cTimer_QueryPerformanceFrequency::GetFrameRate()
{
	return _frameRate;
}
bool cTimer_QueryPerformanceFrequency::GetTickFPS()
{
	return _isTickFPS;
}