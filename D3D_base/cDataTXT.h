#pragma once
class cDataTXT
{
public:
	cDataTXT();
	~cDataTXT();

	//���̺�
	void Save(const char* saveFileName, std::vector<std::string> vStr);
	char* VectorArrayCombine(std::vector<std::string> vArray);

	//�ε�
	std::vector<std::string> Load(const char* loadFileName);
	std::vector<std::string> CharArraySeparation(char charArray[]);
};

