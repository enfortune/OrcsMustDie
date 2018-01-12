#pragma once
#include "cDataINI.h"
#include "cDataTXT.h"

#define g_pFileManager cFileManager::GetInstance()


class cFileManager
{
	SINGLETONE(cFileManager);
private:
	cDataTXT* m_pDataTXT;
	cDataINI* m_pDataINI;

public:
	void Setup();
	void Destroy();

	cDataTXT* GetData_TXT() { return m_pDataTXT; }
	cDataINI* GetData_INI() { return m_pDataINI; }
};


