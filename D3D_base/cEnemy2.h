#pragma once
#include "cChaNode.h"
#include "cEnemyBase.h"

class cSkinnedMeshEX;
class cPlayer;
class GraphFindPath;
class Trap;
class cSubject;

class cEnemy2 : public cEnemyBase
{
private:
	D3DXVECTOR3 vPlayerPos;
	cPlayer* m_pPlayer;
	GraphFindPath* m_pFindPath;

	std::vector<D3DXVECTOR3> m_vPath;
	std::vector<Trap> * m_vTrap;
	cSubject* m_pAnduin;


public:
	cEnemy2();
	virtual ~cEnemy2();


	virtual void Setup(bool bUseTransformData, D3DXVECTOR3 vPosSetup);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void Idle();
	void Move(D3DXVECTOR3 vGoal, float fDelta);
	void Move(D3DXVECTOR3 vGoal, float fDelta, int dijkNum);
	void Attack(float fDelta);
	void Attackbarricade(float fDelta);
	void AttackAnduin(float fDelta);
	int Findbarricade(float fDelta, bool bPass);
	void Dead();
	void Jump();
	void find(int tpNum, float fDelta);
	void setPlayer(cPlayer* pSetPlayer);
	void getDamage(int nDamage);
	void setPath(GraphFindPath* pFindPath);
	void setAnduin(cSubject* Anduin);
	void setTrap(std::vector<Trap> & vTrap);
	void HpManager();
	void sendGold();

};

