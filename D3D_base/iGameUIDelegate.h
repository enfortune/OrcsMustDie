#pragma once


class iGameUIDelegate
{

public:
	iGameUIDelegate();
	virtual ~iGameUIDelegate();

	virtual bool UIButtonAction(std::string sTag, BUTTON_STATE enState) = 0;

	//bool UISliderAction();
};

