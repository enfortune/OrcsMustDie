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

		SetupBoneMatrixPtrs(stTemp.pRoot, stTemp.pRoot);
		if (stTemp.pAniCtrl)
			SetupAnimationSet(stTemp.pAniCtrl, stTemp.vecAniSet);

		m_mapXFile[sKey] = stTemp;

		stRet.pRoot = stTemp.pRoot;
		if (stTemp.pAniCtrl)
		{
			stTemp.pAniCtrl->CloneAnimationController(
				stTemp.pAniCtrl->GetMaxNumAnimationOutputs(),
				stTemp.pAniCtrl->GetMaxNumAnimationSets(),
				stTemp.pAniCtrl->GetMaxNumTracks(),
				stTemp.pAniCtrl->GetMaxNumEvents(),
				&stRet.pAniCtrl);

			stRet.vecAniSet = stTemp.vecAniSet;
		}
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
		{
			iter->second.pAniCtrl->CloneAnimationController(
				iter->second.pAniCtrl->GetMaxNumAnimationOutputs(),
				iter->second.pAniCtrl->GetMaxNumAnimationSets(),
				iter->second.pAniCtrl->GetMaxNumTracks(),
				iter->second.pAniCtrl->GetMaxNumEvents(),
				&stRet.pAniCtrl);

			stRet.vecAniSet = iter->second.vecAniSet;
		}
			
	}

	return stRet;
}

void cXFileManager::DestroyAllXFile()
{
	std::map<std::string, ST_XFILE>::iterator iter;
	for (iter = m_mapXFile.begin(); iter != m_mapXFile.end(); )
	{
		cAllocateHierarchyEX boneHierarchy;
		if (iter->second.pAniCtrl) iter->second.pAniCtrl->Release();
		D3DXFrameDestroy(iter->second.pRoot, &boneHierarchy);
		iter = m_mapXFile.erase(iter);
	}
}

void cXFileManager::Destroy()
{
	this->DestroyAllXFile();
}


void cXFileManager::SetupBoneMatrixPtrs(LPD3DXFRAME pRoot, LPD3DXFRAME pFrame)// 연관된 본의 매트릭스 포인터를 연결함. (스키닝을 위해)
{
	ST_BONE* pBone = static_cast<ST_BONE*>(pFrame);

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);

		if (pBoneMesh->pSkinInfo != nullptr)
		{
			DWORD dwNumBones = pBoneMesh->pSkinInfo->GetNumBones();
			//pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones]; // AllocateHierarchy 클래스에서 이미 메모리 할당해놨음.
			for (DWORD i = 0; i < dwNumBones; i++)
			{
				ST_BONE* pB = static_cast<ST_BONE*>(
					D3DXFrameFind(
						pRoot,
						pBoneMesh->pSkinInfo->GetBoneName(i)));

				if (pB != nullptr)
				{
					pBoneMesh->ppBoneMatrixPtrs[i] =
						&pB->matCombinedTransformMatrix;
				}
				else
				{
					pBoneMesh->ppBoneMatrixPtrs[i] = nullptr;
				}
			}
		}
	}

	if (pBone->pFrameSibling != nullptr) SetupBoneMatrixPtrs(pRoot, pBone->pFrameSibling);
	if (pBone->pFrameFirstChild != nullptr) SetupBoneMatrixPtrs(pRoot, pBone->pFrameFirstChild);
}

void cXFileManager::SetupAnimationSet(IN LPD3DXANIMATIONCONTROLLER pAniCtrl, OUT std::vector<LPD3DXANIMATIONSET>& vecAniSet)
{
	if (pAniCtrl)
	{
		UINT nNumSet = pAniCtrl->GetMaxNumAnimationSets();

		for (UINT i = 0; i < nNumSet; i++)
		{
			LPD3DXANIMATIONSET aniSet;
			LPTSTR	szName;
			pAniCtrl->GetAnimationSet(i, &aniSet);
			vecAniSet.push_back(aniSet);
		}
	}
}