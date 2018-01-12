#include "stdafx.h"
#include "cFileManager.h"


cFileManager::cFileManager()
{
	
}


cFileManager::~cFileManager()
{
}

void cFileManager::Setup()
{
	m_pDataTXT = new cDataTXT;
	m_pDataINI = new cDataINI;
}
void cFileManager::Destroy()
{
	SAFE_DELETE(m_pDataTXT);
	SAFE_DELETE(m_pDataINI);
}
