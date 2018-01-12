#pragma once
class cDataTXT
{
public:
	cDataTXT();
	~cDataTXT();

	//세이브
	void Save(const char* saveFileName, std::vector<std::string> vStr);
	char* VectorArrayCombine(std::vector<std::string> vArray);

	//로드
	std::vector<std::string> Load(const char* loadFileName);
	std::vector<std::string> CharArraySeparation(char charArray[]);
};

