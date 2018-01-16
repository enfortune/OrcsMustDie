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
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame); // �޽ÿ� �����ִ°͵� ����
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);
};

