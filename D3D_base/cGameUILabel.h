#pragma once
#include "cGameNode.h"

class cGameText;

class cGameUILabel : public cGameNode
{
	SYNTHESIZE(cGameText*, m_pText, Text);

	/* basic data for matrix */
	SYNTHESIZE_REF(D3DXVECTOR2, m_vPos, Position);
public:
	cGameUILabel();
	virtual ~cGameUILabel();

	void SetText(std::string sText);

	virtual void Setup(cFontManager::eFontType enFontType, std::string sText);
	virtual void Render() override;
	virtual void Delete() override;

	virtual D3DXMATRIXA16 GetMatirixToParent() override;
};

