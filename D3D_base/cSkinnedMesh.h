#pragma once


	

class cSkinnedMesh
{
protected:
	LPD3DXFRAME		m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl;
	std::map<LPCSTR,LPD3DXANIMATIONSET> m_mapAniSet;
	std::vector<LPCSTR> m_vecAniSetName;

public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Setup(IN char* szFolder, IN char* szFile);
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void Destroy();
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame); // 메시에 영향주는것들 연결
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	void SetupAnimationSet(LPD3DXANIMATIONCONTROLLER pAniCtrl);

	void SelectAnimationSet(UINT nTrack, LPCSTR szAniSetName);
	void SelectAnimationSet(UINT nTrack, int nAniID);

	void UpdateAnimation(float fDelta);

};

