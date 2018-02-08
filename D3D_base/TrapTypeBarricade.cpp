#include "stdafx.h"
#include "TrapTypeBarricade.h"

#include "TrapTypeComponent.h"
#include "Trap.h"

#include "cSkinnedMeshEX.h"

TrapTypeBarricade::TrapTypeBarricade()
{
	modelList_.resize(4);
	modelList_[0].Setup("Resource/Trap/Barricade", "Resource/Trap/Barricade/Barricade.X");
	modelList_[1].Setup("Resource/Trap/Barricade", "Resource/Trap/Barricade/BarricadeDamageA.X");
	modelList_[2].Setup("Resource/Trap/Barricade", "Resource/Trap/Barricade/BarricadeDamageB.X");
	modelList_[3].Setup("Resource/Trap/Barricade", "Resource/Trap/Barricade/BarricadeDamageC.X");

	D3DXMATRIXA16 matrixScale {}, matrixRotation {};
	D3DXMatrixScaling(&matrixScale, 0.005f, 0.005f, 0.005f);
	D3DXMatrixRotationX(&matrixRotation, D3DX_PI * 0.5f);

	matrixLocalList_.resize(1);
	matrixLocalList_[0] = matrixScale * matrixRotation;

	frustumLocal_.vNear_00 = {-1.0f, 0.0f, -1.0f};
	frustumLocal_.vNear_01 = {-1.0f, 0.6f, -1.0f};
	frustumLocal_.vNear_10 = {-1.0f, 0.0f, +1.0f};
	frustumLocal_.vNear_11 = {-1.0f, 0.6f, +1.0f};
	frustumLocal_.vFar_00 = {1.0f, 0.0f, -1.0f};
	frustumLocal_.vFar_01 = {1.0f, 0.6f, -1.0f};
	frustumLocal_.vFar_10 = {1.0f, 0.0f, +1.0f};
	frustumLocal_.vFar_11 = {1.0f, 0.6f, +1.0f};

	frustumInteractionLocal_ = frustumLocal_;

	isBlockable_ = true;

	width_ = 1;
	height_ = 1;

	isInstallPositionArray_[static_cast<size_t> (TrapType::eInstallPosition::FLOOR)] = true;

	pTypeComponentBlockable_ = new TrapTypeComponentBlockable;
	pTypeComponentBlockable_->hpMax_ = 240;
	pTypeComponentBlockable_->timerRemainMax_ = 5.0f;

	moneyCost_ = 600;
}

TrapComponentBlockableBarricade::TrapComponentBlockableBarricade(TrapTypeComponentBlockable * pParent)
{ TrapComponentBlockable::TrapComponentBlockable(pParent); }

void TrapComponentBlockableBarricade::onDestroy(Trap & trap)
{
	TrapComponentBlockable::onDestroy(trap);

	trap.setRenderIndex(3);
}

void TrapComponentBlockableBarricade::onHit(Trap & trap, int damage)
{
	TrapComponentBlockable::onHit(trap, damage);

	if (hp_ > 0 && hp_ <= 80)
		trap.setRenderIndex(2);
	else if (hp_ <= 160)
		trap.setRenderIndex(1);
}

std::unique_ptr<TrapComponentBlockable> TrapTypeComponentBlockableBarricade::newComponentObject() const
{ return std::make_unique<TrapComponentBlockableBarricade> (const_cast<TrapTypeComponentBlockableBarricade *> (this)); }
