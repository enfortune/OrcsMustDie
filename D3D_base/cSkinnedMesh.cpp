#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh()
	: m_pRoot(nullptr)
{
}


cSkinnedMesh::~cSkinnedMesh()
{
	this->Destroy();
}

void cSkinnedMesh::Setup(IN char* szFolder, IN char* szFile)
{
	cAllocateHierarchy boneHierarchy;
	boneHierarchy.SetFolder(szFolder);

	D3DXLoadMeshHierarchyFromX(szFile,
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&boneHierarchy,
		NULL,
		&m_pRoot,
		&m_pAniCtrl);

	SetupBoneMatrixPtrs(m_pRoot);
	SetupAnimationSet(m_pAniCtrl);

	
	
}
void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?

	else pBone = static_cast<ST_BONE*>(pFrame);

	if (pParent == nullptr)
	{
		pBone->matCombinedTransformMatrix = pBone->TransformationMatrix;
	}
	else
	{
		ST_BONE* pParentFrame = static_cast<ST_BONE*>(pParent);
		D3DXMatrixMultiply(&pBone->matCombinedTransformMatrix,
			&pBone->TransformationMatrix,
			&pParentFrame->matCombinedTransformMatrix);
	}

	UpdateSkinnedMesh(pBone);
//	

	if (pBone->pFrameSibling != nullptr) Update(pBone->pFrameSibling, pParent);
	if (pBone->pFrameFirstChild != nullptr) Update(pBone->pFrameFirstChild, pFrame);

}
void cSkinnedMesh::Render(LPD3DXFRAME pFrame)
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?
	else pBone = static_cast<ST_BONE*>(pFrame);

	

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);
		
		if (pBoneMesh->pSkinInfo != NULL)
		{
			for (int i = 0; i < pBoneMesh->nNumAttributeGroups; i++)
			{
				int nAdj = pBoneMesh->pAttributeTable[i].AttribId;
				g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtl[nAdj]);
				g_pD3DDevice->SetTexture(NULL, pBoneMesh->vecTexture[nAdj]);
				pBoneMesh->MeshData.pMesh->DrawSubset(nAdj);
			}
		}
	}
	

	if (pBone->pFrameSibling != nullptr) Render(pBone->pFrameSibling);
	if (pBone->pFrameFirstChild != nullptr) Render(pBone->pFrameFirstChild);


}

void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy boneHierarchy;
	D3DXFrameDestroy(m_pRoot, &boneHierarchy);

}
void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?
	else pBone = static_cast<ST_BONE*>(pFrame);

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);

		if (pBoneMesh->pSkinInfo != NULL)
		{
			int nNumBones = static_cast<int>(pBoneMesh->pSkinInfo->GetNumBones());

			pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[nNumBones];

			for (int i = 0; i < nNumBones; i++)
			{
				ST_BONE* pB = static_cast<ST_BONE*>(
					D3DXFrameFind(
					m_pRoot,
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
	
	if (pBone->pFrameSibling != nullptr) SetupBoneMatrixPtrs(pBone->pFrameSibling);
	if (pBone->pFrameFirstChild != nullptr) SetupBoneMatrixPtrs(pBone->pFrameFirstChild);




}


void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?
	else pBone = static_cast<ST_BONE*>(pFrame);

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);

		if (pBoneMesh->pSkinInfo != NULL)
		{
			int nNumBones = static_cast<int>(pBoneMesh->pSkinInfo->GetNumBones());
			for (int i = 0; i < nNumBones; i++)
			{
				D3DXMatrixMultiply(&pBoneMesh->pCurrBoneMatrix[i],
					&pBoneMesh->pBoneOffsetMatrix[i],
					pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			/*unsigned char*/
			BYTE* lpSrc = NULL;
			void* lpDest = NULL;
			pBoneMesh->pOrigMesh->
				LockVertexBuffer(D3DLOCK_READONLY, (void**)&lpSrc);
			pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&lpDest);

			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrBoneMatrix,
				NULL, lpSrc, lpDest);

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}
}

void cSkinnedMesh::UpdateAnimation(float fDelta)
{
	
	m_pAniCtrl->AdvanceTime(fDelta, NULL);
}

void cSkinnedMesh::SetupAnimationSet(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	UINT nNumSet = pAniCtrl->GetNumAnimationSets();
	
	for (UINT i = 0; i < nNumSet; i++)
	{
		LPD3DXANIMATIONSET aniSet;
		LPTSTR	szName;
		pAniCtrl->GetAnimationSet(i, &aniSet);
		m_mapAniSet[aniSet->GetName()] = aniSet;
		m_vecAniSetName.push_back(aniSet->GetName());
	}
}

void cSkinnedMesh::SelectAnimationSet(UINT nTrack, LPCSTR szAniSetName)
{
	if (m_mapAniSet.find(szAniSetName) == m_mapAniSet.end()) return;

	m_pAniCtrl->SetTrackAnimationSet(nTrack, m_mapAniSet[szAniSetName]);
}
void cSkinnedMesh::SelectAnimationSet(UINT nTrack, int nAniID)
{
	if (nTrack >= m_vecAniSetName.size()) return;

	m_pAniCtrl->SetTrackAnimationSet(nTrack, (m_mapAniSet.find(m_vecAniSetName[nAniID]))->second);
}