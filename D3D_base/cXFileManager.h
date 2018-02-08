#pragma once
#define g_pXFileManager cXFileManager::GetInstance()

struct ST_XFILE
{
	LPD3DXFRAME		pRoot;
	LPD3DXANIMATIONCONTROLLER	pAniCtrl;
	std::vector<LPD3DXANIMATIONSET> vecAniSet;

	ST_XFILE() : pRoot(nullptr), pAniCtrl(nullptr) {}
};

class cXFileManager
{
	SINGLETONE(cXFileManager);
private:
	std::map<std::string, ST_XFILE> m_mapXFile;

public:

	ST_XFILE LoadXFile(std::string sKey, IN char* szFolder, IN char* szFile);
	ST_XFILE GetXFile(std::string sKey);
	void DestroyAllXFile();

	void Destroy();

protected:
	void SetupBoneMatrixPtrs(LPD3DXFRAME pRoot, LPD3DXFRAME pFrame);
	void SetupAnimationSet(IN LPD3DXANIMATIONCONTROLLER pAniCtrl, OUT std::vector<LPD3DXANIMATIONSET>& vecAniSet);
};

