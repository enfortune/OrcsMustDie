#pragma once
#include "cGameNode.h"

class cMapData;
enum EN_GRIDBOXKIND;

class cMapMaker : public cGameNode 
{
protected:
	cMapData* m_pMapData;
	SYNTHESIZE_REF(bool, m_bIsMakingEnable, MakingEnable);

	SYNTHESIZE_REF(RECT, m_rcActiveArea, ActiveArea);
	
protected:
	EN_GRIDBOXKIND m_enCurrBoxKind;
	bool m_bIsOnMapLayer;
	int m_nCurrX, m_nCurrY, m_nCurrZ;

protected:
	void MakeBlock(int x, int y, int z);

	void LayerUp();
	void LayerDown();

	void SaveData(std::string sFileName);
	void LoadData(std::string sFileName);
public:
	cMapMaker();
	~cMapMaker();

	virtual void Setup(cMapData* pMapData = nullptr);
	virtual void Update(float fDelta = g_pTimeManager->GetEllapsedTime()) override;
	virtual void Render() override;
	virtual void Delete() override;

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	void SetMapData(cMapData* pMapData);

	
};

