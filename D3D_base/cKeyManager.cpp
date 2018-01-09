#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager()
{
}


cKeyManager::~cKeyManager()
{
}

void cKeyManager::Setup()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		m_bsOnceKeyUp.set(i, false);
		m_bsOnceKeyDown.set(i, false);
		m_bsStayKeyDown.set(i, false);
	}
}
void cKeyManager::Update()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			m_bsOnceKeyUp.set(i, false);
			if (!m_bsStayKeyDown[i]) m_bsOnceKeyDown.set(i, true);
			else m_bsOnceKeyDown.set(i, false);
			m_bsStayKeyDown.set(i, true);
		}
		else
		{
			if (m_bsStayKeyDown[i]) m_bsOnceKeyUp.set(i, true);
			else m_bsOnceKeyUp.set(i, false);
			m_bsOnceKeyDown.set(i, false);
			m_bsStayKeyDown.set(i, false);
		}
	}
}

bool cKeyManager::IsOnceKeyDown(int key)
{
	return m_bsOnceKeyDown[key];
}
bool cKeyManager::IsOnceKeyUp(int key)
{
	return m_bsOnceKeyUp[key];
}
bool cKeyManager::IsStayKeyDown(int key)
{
	return m_bsStayKeyDown[key];
}
bool cKeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}