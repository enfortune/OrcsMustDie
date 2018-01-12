#pragma once

struct ST_INIDATA
{
	const char* subject;	//�׸�	����
	const char* title;		//����	����
	const char* body;		//��		92
};

class cDataINI
{
private:
	typedef std::vector<ST_INIDATA> vecIniData;
	typedef std::vector<ST_INIDATA>::iterator vecIniDataIter;

	typedef std::vector<vecIniData> vecIniDatas;
	typedef std::vector<vecIniData>::iterator vecIniDatasIter;

private:
	vecIniDatas m_vecvecIniData;

public:

	//�縶õ - ���
	void AddData(const char* subject, const char* title, const char* body);
	void Save(char* fileName);

	char* LoadDataString(const char* fileName, const char* subject, const char* title);
	int LoadDataInterger(const char* fileName, const char* subject, const char* title);

	cDataINI();
	~cDataINI();
};

