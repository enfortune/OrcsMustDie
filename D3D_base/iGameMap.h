#pragma once

class cGameNode;

class iGameMap
{
public:
	iGameMap();
	virtual ~iGameMap();
	virtual bool MapCollisionCheck(cGameNode* pNode, float fDelta) = 0; // �ϴ��� �̰ɷ� ����..
	//������ �� ���� ����� �ִٸ� �װɷ� ��ü�Ұ���
};
