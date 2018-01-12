#pragma once
class iMap
{
public:
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
	// TODO: 함정 설치 가능한지 아닌지를 제공할 인터페이스 필요
};