#include "stdafx.h"
#include "cXFileManager.h"
#include "cAllocateHierarchyEX.h"

cXFileManager::cXFileManager()
{
}


cXFileManager::~cXFileManager()
{
}

ST_XFILE cXFileManager::LoadXFile(std::string sKey, IN char * szFolder, IN char * szFile)
{
	ST_XFILE stRet;

	std::map<std::string, ST_XFILE>::iterator iter;
	iter = m_mapXFile.find(sKey);
	if (iter != m_mapXFile.end())
	{
		stRet.pRoot = iter->second.pRoot;
		if (iter->second.pAniCtrl)
			iter->second.pAniCtrl->CloneAnimationController(
				iter->second.pAniCtrl->GetMaxNumAnimationOutputs(),
				iter->second.pAniCtrl->GetMaxNumAnimationSets(),
				iter->second.pAniCtrl->GetMaxNumTracks(),
				iter->second.pAniCtrl->GetMaxNumEvents(),
				&stRet.pAniCtrl
				);
	}
	else
	{
		ST_XFILE stTemp;
		cAllocateHierarchyEX boneHierarchy;
		boneHierarchy.SetFolder(szFolder);

		std::string sFullPath;

		if (strlen(szFolder) > 0)
			sFullPath = szFolder + std::string("/") + szFile;
		else
			sFullPath = szFile;

		D3DXLoadMeshHierarchyFromX(szFile,
			D3DXMESH_MANAGED | D3DXMESH_32BIT,
			g_pD3DDevice,
			&boneHierarchy,
			NULL,
			&stTemp.pRoot,
			&stTemp.pAniCtrl);

		m_mapXFile[sKey] = stTemp;

		stRet.pRoot = stTemp.pRoot;
		if (stTemp.pAniCtrl)
			stTemp.pAniCtrl->CloneAnimationController(
				stTemp.pAniCtrl->GetMaxNumAnimationOutputs(),
				stTemp.pAniCtrl->GetMaxNumAnimationSets(),
				stTemp.pAniCtrl->GetMaxNumTracks(),
				stTemp.pAniCtrl->GetMaxNumEvents(),
				&stRet.pAniCtrl
		);
	}
	
	return stRet;
}

ST_XFILE cXFileManager::GetXFile(std::string sKey)
{
	ST_XFILE stRet;

	std::map<std::string, ST_XFILE>::iterator iter;
	iter = m_mapXFile.find(sKey);
	if (iter != m_mapXFile.end())
	{
		stRet.pRoot = iter->second.pRoot;
		if (iter->second.pAniCtrl)
			iter->second.pAniCtrl->CloneAnimationController(
				iter->second.pAniCtrl->GetMaxNumAnimationOutputs(),
				iter->second.pAniCtrl->GetMaxNumAnimationSets(),
				iter->second.pAniCtrl->GetMaxNumTracks(),
				iter->second.pAniCtrl->GetMaxNumEvents(),
				&stRet.pAniCtrl
			);
	}

	return stRet;
}

void cXFileManager::DestroyAllXFile()
{
	std::map<std::string, ST_XFILE>::iterator iter;
	for (iter = m_mapXFile.begin(); iter != m_mapXFile.end(); )
	{
		cAllocateHierarchyEX boneHierarchy;
		iter->second.pAniCtrl->Release();
		D3DXFrameDestroy(iter->second.pRoot, &boneHierarchy);
		iter = m_mapXFile.erase(iter);
	}
}

void cXFileManager::Destroy()
{
	this->DestroyAllXFile();
}
