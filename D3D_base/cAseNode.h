#pragma once
#include "cObject.h"
#include "cMtlTex.h"

//class cMtlTex;
class cAseNode : public cObject
{
private:
	D3DXMATRIXA16	m_matLocalTM;
	D3DXMATRIXA16*	m_matParentWorldTM;

	SYNTHESIZE_REF_16(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	SYNTHESIZE_REF(std::vector<ST_PTN_VERTEX>, m_vecVertex, Vertex);

	std::vector<cAseNode*>	m_vecChild;
	SYNTHESIZE_REF(std::vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_REF(std::vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);


	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwTicksPerFrame;

	int							m_nNumTri;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

public:
	cAseNode();
	~cAseNode();

	void Update(int nFrame, D3DXMATRIXA16* pMatParent);
	void Render();
	void AddChild(cAseNode* pChild);
	void Destroy();

	void CalcOriginalLocalTM(D3DXMATRIXA16* pMatParent);
	void CalcLocalR(D3DXMATRIXA16& matR, int nFrame);
	void CalcLocalT(D3DXMATRIXA16& matT, int nFrame);

	int GetFrame();

	void BuildVB(std::vector<ST_PTN_VERTEX>& vecVertex);
};

