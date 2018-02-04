#pragma once

struct TrapType;

class TrapTypeManager
{
private:
	std::map<std::string, TrapType *> pTrapTypeMap_;

public:
	TrapTypeManager() = default;
	~TrapTypeManager();
	
	//UNCOPYABLE(TrapTypeManager);

public:
	TrapType * find(std::string key) const;

public:
	TrapType * add(std::string key, TrapType * pTrapType);

	void init();
};