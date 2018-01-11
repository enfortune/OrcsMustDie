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


		//ȣ���������������̳��ƾƾƾ� �������� ������ �ƴϸ�
		_lastTime = timeGetTime();	//�и��������� ����
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
			while (_FPSTimeElapsed > (1.0f / FPS)) // TODO: �׽�Ʈ �ʿ�
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
		while (_secChecker > 1.0f) // TODO: �׽�Ʈ �ʿ�
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