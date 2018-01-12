#include "stdafx.h"
#include "cDataINI.h"


cDataINI::cDataINI()
{
}


cDataINI::~cDataINI()
{
}


//�縶õ - ���
void cDataINI::AddData(const char* subject, const char* title, const char* body)
{
	//������ ���� ����ü�� �����ϰ�
	ST_INIDATA iniData;
	iniData.subject = subject;
	iniData.title = title;
	iniData.body = body;

	//�� ����ü�� �������� ���� ���͸� �����ϰ�
	vecIniData vecIniData;
	//���� ����ش�
	vecIniData.push_back(iniData);
	m_vecvecIniData.push_back(vecIniData);

}

void cDataINI::Save(char* fileName)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	for (int i = 0; i < m_vecvecIniData.size(); i++)
	{
		vecIniData vecData = m_vecvecIniData[i];
		WritePrivateProfileString(vecData[0].subject, vecData[0].title, vecData[0].body, str);

		vecData.clear();
	}

	m_vecvecIniData.clear();

}


char* cDataINI::LoadDataString(const char* fileName, const char* subject, const char* title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	char data[65] = { NULL };
	GetPrivateProfileString(subject, title, "", data, 64, str);

	return data;
}

int cDataINI::LoadDataInterger(const char* fileName, const char* subject, const char* title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);



	return GetPrivateProfileInt(subject, title, 0, str);
}
