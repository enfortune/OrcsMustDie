#pragma once

#include "TrapType.h"
#include "TrapComponent.h"

class TrapTypeSpike : public TrapType
{
public:
	TrapTypeSpike();
	virtual ~TrapTypeSpike() = default;

public:
	//virtual void render(D3DXMATRIXA16 & matrixWorld) override;
};

class TrapComponentRenderSpike : public TrapComponentRender
{
public:
	TrapComponentRenderSpike() = default;
	virtual ~TrapComponentRenderSpike() = default;

public:
	virtual void render(TrapType & trapType, D3DXMATRIXA16 & matrixWorld) override;
};