#pragma once

class cGameNode;

class iGameMap
{
public:
	iGameMap();
	virtual ~iGameMap();
	virtual bool MapCollisionCheck(cGameNode* pNode) = 0; // 일단은 이걸로 쓰고..
	//하지만 더 나은 방식이 있다면 그걸로 교체할것임
};
