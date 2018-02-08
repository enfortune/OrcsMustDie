#pragma once

class cSkinnedMeshEX
{
protected:
	LPD3DXFRAME		m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl;
	std::vector<LPD3DXANIMATIONSET> m_vecAniSet;
	D3DXTRACK_DESC m_stTrackDesc;
	bool		m_isBlend;
	bool		m_bLoop;
	bool		m_bAniStart;

	int			m_startAniId;
	float		m_fPassedBlendTime;
	float		m_fBlendTime;
	float		m_fCurPosition;

	float		m_fDelta;
	float		m_fDeltaSigma;

protected:
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame); // 메시에 영향주는것들 연결
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);
	void UpdateFrames(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void RenderFrames(LPD3DXFRAME pFrame);
public:
	cSkinnedMeshEX();
	~cSkinnedMeshEX();
	SYNTHESIZE(bool, m_bAniEnd, AniEnd);

	void Setup(IN char* szFolder, IN char* szFile);
	void Update(float fDelta);
	void Render();
	void Destroy();


	void SetAnimationSet(UINT nTrack, int nAniID, bool Loop = false);
	void SetAnimationSetBlend(UINT nTrack, int nAniID, bool Loop = false);

	float	getCurPosition();
	D3DXMATRIXA16& FindBone(IN char* szFile);
};

