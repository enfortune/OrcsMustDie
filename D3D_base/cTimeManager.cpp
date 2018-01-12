#include "stdafx.h"
#include "cTimeManager.h"

//#include "iTimer.h"
#include "cTimer_QueryPerformanceFrequency.h"

cTimeManager::cTimeManager()
	: m_pTimer(NULL)
{
}


cTimeManager::~cTimeManager()
{
	this->Destroy();
}

void cTimeManager::Setup()
{
	m_pTimer = new cTimer_QueryPerformanceFrequency;
	m_pTimer->Setup();
}
void cTimeManager::Update(float FPS)
{
	if (m_pTimer) m_pTimer->Update(FPS);
}
float cTimeManager::GetEllapsedTime()
{
	if (m_pTimer) return m_pTimer->GetEllapsedTime();
	else return 0.0f;
}
float cTimeManager::GetWorldTime()
{
	if (m_pTimer) return m_pTimer->GetWorldTime();
	else return 0.0f;
}
float cTimeManager::GetTickFPSEllapsedTime()
{
	if (m_pTimer) return m_pTimer->GetTickFPSEllapsedTime();
	else return 0.0f;
}
unsigned long cTimeManager::GetFrameRate()
{
	if (m_pTimer) return m_pTimer->GetFrameRate();
	return 0;
}
bool cTimeManager::GetTickFPS()
{
	if (m_pTimer) return m_pTimer->GetTickFPS();
	return false;
}

void cTimeManager::Destroy()
{
	SAFE_DELETE(m_pTimer);
}