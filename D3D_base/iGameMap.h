#pragma once

class cGameNode;

class iGameMap
{
public:
	iGameMap();
	virtual ~iGameMap();
	virtual bool MapCollisionCheck(cGameNode* pNode) = 0; // �ϴ��� �̰ɷ� ����..
	//������ �� ���� ����� �ִٸ� �װɷ� ��ü�Ұ���
};
