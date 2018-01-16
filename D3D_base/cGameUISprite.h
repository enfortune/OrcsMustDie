#pragma once
#include "cGameNode.h"
class cGameSprite;

class cGameUISprite : public cGameNode
{
protected:
	SYNTHESIZE(cGameSprite*, m_pSprite, Sprite);

	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
	SYNTHESIZE_REF(float, m_fRotAngle, RotAngle); // Z angle
	SYNTHESIZE_REF(D3DXVECTOR2, m_vScale, Scale);
public:
	cGameUISprite();
	~cGameUISprite();

	virtual void Setup(std::string sFileName);
	virtual void Render() override;
	virtual void Delete() override;

	std::vector<D3DXVECTOR2> GetBoundingBox();
	virtual D3DXMATRIXA16 GetMatirixToParent() override;
};

