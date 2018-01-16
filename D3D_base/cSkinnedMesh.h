#pragma once
class cSkinnedMesh
{
protected:
	LPD3DXFRAME		m_pRoot;

public:
	cSkinnedMesh();
	~cSkinnedMesh();

	void Setup(IN char* szFolder, IN char* szFile);
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame); // 메시에 영향주는것들 연결
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);
};

