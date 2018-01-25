#pragma once
#include "cGameNode.h"

class cTrap : public cGameNode
{
public:
	cTrap();
	~cTrap();

	virtual void Delete() override;
};

