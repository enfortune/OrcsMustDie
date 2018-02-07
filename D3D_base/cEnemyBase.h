#pragma once
#include "cChaNode.h"
class cEnemyBase :
	public cChaNode
{
public:
	cEnemyBase();
	virtual ~cEnemyBase();

	float fDeadCount;

	virtual void Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup) = 0;
	virtual void getDamage(int nDamage) = 0;
	virtual void sendGold() = 0;
};

