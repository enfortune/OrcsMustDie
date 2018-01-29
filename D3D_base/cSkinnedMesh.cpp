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
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		g_pD3DDevice,
		&boneHierarchy,
		NULL,
		&m_pRoot,
		&m_pAniCtrl);

	SetupBoneMatrixPtrs(m_pRoot);
	UpdateFrames(nullptr, nullptr); // 로드 후 한번 업데이트를 돌려준다.

	SetupAnimationSet(m_pAniCtrl);
}

void cSkinnedMesh::UpdateFrames(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
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

	if (pBone->pFrameSibling != nullptr) UpdateFrames(pBone->pFrameSibling, pParent);
	if (pBone->pFrameFirstChild != nullptr) UpdateFrames(pBone->pFrameFirstChild, pFrame);
}

void cSkinnedMesh::Update()
{
	this->UpdateFrames(m_pRoot, nullptr);
	this->UpdateSkinnedMesh(m_pRoot); // software skinning
}

void cSkinnedMesh::RenderFrames(LPD3DXFRAME pFrame)
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?
	else pBone = static_cast<ST_BONE*>(pFrame);

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);
		
		if (pBoneMesh->MeshData.pMesh != nullptr)
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

	if (pBone->pFrameSibling != nullptr) RenderFrames(pBone->pFrameSibling);
	if (pBone->pFrameFirstChild != nullptr) RenderFrames(pBone->pFrameFirstChild);
}

void cSkinnedMesh::Render()
{
	this->RenderFrames(m_pRoot);
}

void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy boneHierarchy;
	D3DXFrameDestroy(m_pRoot, &boneHierarchy);

}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame) // 연관된 본의 매트릭스 포인터를 연결함. (스키닝을 위해)
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?
	else pBone = static_cast<ST_BONE*>(pFrame);

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);

		if (pBoneMesh->pSkinInfo != nullptr)
		{
			DWORD dwNumBones =pBoneMesh->pSkinInfo->GetNumBones();
			//pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones]; // AllocateHierarchy 클래스에서 이미 메모리 할당해놨음.
			for (DWORD i = 0; i < dwNumBones; i++)
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

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame) // skinning
{
	ST_BONE* pBone;
	if (pFrame == nullptr) pBone = static_cast<ST_BONE*>(m_pRoot); // 초기에 null이 들어올 것이므로?
	else pBone = static_cast<ST_BONE*>(pFrame);

	if (pBone->pMeshContainer != nullptr)
	{
		ST_BONE_MESH* pBoneMesh = static_cast<ST_BONE_MESH*>(pBone->pMeshContainer);

		if (pBoneMesh->pSkinInfo != nullptr)
		{
			DWORD dwNumBones = pBoneMesh->pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; i++)
			{
				D3DXMatrixMultiply(&pBoneMesh->pCurrBoneMatrix[i],
					&pBoneMesh->pBoneOffsetMatrix[i],
					pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			//BYTE = unsigned char
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

	if (pBone->pFrameSibling != nullptr) UpdateSkinnedMesh(pBone->pFrameSibling);
	if (pBone->pFrameFirstChild != nullptr) UpdateSkinnedMesh(pBone->pFrameFirstChild);
}

void cSkinnedMesh::UpdateAnimation(float fDelta)
{
	m_pAniCtrl->AdvanceTime(fDelta, NULL);
}

void cSkinnedMesh::SetupAnimationSet(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	UINT nNumSet = pAniCtrl->GetMaxNumAnimationSets();
	
	for (UINT i = 0; i < nNumSet; i++)
	{
		LPD3DXANIMATIONSET aniSet;
		LPTSTR	szName;
		pAniCtrl->GetAnimationSet(i, &aniSet);
		m_mapAniSet[aniSet->GetName()] = aniSet;
		m_vecAniSetName.push_back(aniSet->GetName());
	}
}

void cSkinnedMesh::SetAnimationSet(UINT nTrack, LPCSTR szAniSetName)
{
	if (m_mapAniSet.find(szAniSetName) == m_mapAniSet.end()) return;

	m_pAniCtrl->SetTrackAnimationSet(nTrack, m_mapAniSet[szAniSetName]);


}

void cSkinnedMesh::SetAnimationSet(UINT nTrack, int nAniID)
{
	if (nTrack >= m_vecAniSetName.size()) return;

	m_pAniCtrl->SetTrackAnimationSet(nTrack, (m_mapAniSet.find(m_vecAniSetName[nAniID]))->second);
	
}