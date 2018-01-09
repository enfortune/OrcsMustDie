#pragma once
class cMtlTex;
class cAseNode;

class cAseLoader
{
private:
	FILE*		m_fp;
	char		m_szToken[1024];

	std::vector<cMtlTex*>				m_vecMtlTex;
	std::map<std::string, cAseNode*>	m_mapNode;
	cAseNode*							m_pRoot;

public:
	cAseLoader();
	~cAseLoader();

	cAseNode* Load(char* szFullPath);

	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwTicksPerFrame;

private:
	char*	GetToken();
	int		GetInteager();
	float	GetFloat();
	bool	IsWhite(char c);
	bool	IsEqual(char* str1, char* str2);

	void ProcessScene();
	void ProcessMATERIAL_LIST();
	void ProcessMaterial(cMtlTex* pMtlTex);
	void ProcessMAP_DIFFUSE(cMtlTex* pMtlTex);


	cAseNode* ProcessGEOMETRY();
	void ProcessNODE_TM(cAseNode* pNode);
	void ProcessMESH(cAseNode* pNode);
	void ProcessTM_ANIMATION(cAseNode* node);

	void ProcessPOS_TRACK(cAseNode* node);
	void ProcessROT_TRACK(cAseNode* node);

	void ProcessMESH_VERTEX_LIST(std::vector<D3DXVECTOR3>& vecV);

	void ProcessMESH_FACE_LIST(
		std::vector<ST_PTN_VERTEX>& vecVertex,
		std::vector<D3DXVECTOR3> vecV);
	void ProcessMESH_TVERTLIST(std::vector<D3DXVECTOR2>& vecVT);
	void ProcessMESH_TFACELIST(
		std::vector<ST_PTN_VERTEX>& vecVertex,
		std::vector<D3DXVECTOR2> vecVT);

	void ProcessMESH_NORMALS(std::vector<ST_PTN_VERTEX>& vecVertex);

	void SetSceneInfo(cAseNode* pRoot);
};

