#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
{
	m_dwLastUpdateTime = GetTickCount();
}


cTimeManager::~cTimeManager()
{
}

void cTimeManager::Update()
{
	DWORD dwCurrentTime = GetTickCount();
	m_fEllapsedTime = (dwCurrentTime - m_dwLastUpdateTime) / 1000.0f;
	m_dwLastUpdateTime = dwCurrentTime;
}

float cTimeManager::GetEllapsedTime()
{
	return m_fEllapsedTime;
}

float cTimeManager::GetLastUpdateTime()
{
	return m_dwLastUpdateTime / 1000.0f;
}
