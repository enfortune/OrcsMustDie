#include "stdafx.h"
#include "cSkinnedMeshEX.h"
#include "cAllocateHierarchyEX.h"


cSkinnedMeshEX::cSkinnedMeshEX()
	: m_pRoot(nullptr)
	, m_bLoop(true)
	, m_bAniEnd(false)
	, m_fPassedBlendTime(0.f)
	, m_fBlendTime(0.5f)
	, m_startAniId(NULL)
	, m_bAniStart(false)
{
}

cSkinnedMeshEX::~cSkinnedMeshEX()
{
	this->Destroy();
}

void cSkinnedMeshEX::Setup(IN char* szFolder, IN char* szFile)
{
	cAllocateHierarchyEX boneHierarchy;
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

void cSkinnedMeshEX::UpdateFrames(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
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

void cSkinnedMeshEX::Update()
{

	this->UpdateFrames(m_pRoot, nullptr);
	this->UpdateSkinnedMesh(m_pRoot); // software skinning

	LPD3DXANIMATIONSET		tempPeriod;

	m_pAniCtrl->GetTrackDesc(0, &m_stTrackDesc);
	m_pAniCtrl->GetTrackAnimationSet(0, &tempPeriod);
	float AnimationPlayFactor;
	AnimationPlayFactor = m_stTrackDesc.Position / tempPeriod->GetPeriod();
	AnimationPlayFactor = fmod(AnimationPlayFactor, 1.f);
	if (AnimationPlayFactor < 0.9f) m_bAniEnd = false;
	if (AnimationPlayFactor >= 0.9f)
	{
		if (this->m_bLoop == false)
		{
			//SetAnimationSetBlend(0, m_startAniId, true);
			m_bAniEnd = true;
		}
	}
	if (m_isBlend)
	{
		D3DXTRACK_DESC desc1;
		LPD3DXANIMATIONSET aniSet1;
		m_pAniCtrl->GetTrackDesc(1, &desc1);
		m_pAniCtrl->GetTrackAnimationSet(1, &aniSet1);
		m_fPassedBlendTime += g_pTimeManager->GetEllapsedTime();
		if (m_fPassedBlendTime >= m_fBlendTime)
		{
			m_pAniCtrl->SetTrackWeight(0, 1.0f);
			m_pAniCtrl->SetTrackEnable(1, false);
		}
		else
		{
			if (aniSet1 != nullptr)
			{
				float fTrack1Time = desc1.Position / aniSet1->GetPeriod();
				if (fTrack1Time > 0.9f)
				{
					m_pAniCtrl->SetTrackWeight(0, 1.0f);
					m_pAniCtrl->SetTrackWeight(1, 0.0f);
				}
				else
				{
					float fWeight = m_fPassedBlendTime / m_fBlendTime;
					m_pAniCtrl->SetTrackWeight(0, fWeight);
					m_pAniCtrl->SetTrackWeight(1, 1.0f - fWeight);
				}
			}
			else
			{
				float fWeight = m_fPassedBlendTime / m_fBlendTime;
				m_pAniCtrl->SetTrackWeight(0, fWeight);
				m_pAniCtrl->SetTrackWeight(1, 1.0f - fWeight);
			}
		}
	}

	SAFE_RELEASE(tempPeriod);
}

void cSkinnedMeshEX::RenderFrames(LPD3DXFRAME pFrame)
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

void cSkinnedMeshEX::Render()
{
	this->RenderFrames(m_pRoot);
}

void cSkinnedMeshEX::Destroy()
{
	cAllocateHierarchyEX boneHierarchy;
	D3DXFrameDestroy(m_pRoot, &boneHierarchy);

}

void cSkinnedMeshEX::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame) // 연관된 본의 매트릭스 포인터를 연결함. (스키닝을 위해)
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

void cSkinnedMeshEX::UpdateSkinnedMesh(LPD3DXFRAME pFrame) // skinning
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

void cSkinnedMeshEX::UpdateAnimation(float fDelta)
{

	m_pAniCtrl->AdvanceTime(fDelta, NULL);
}

void cSkinnedMeshEX::SetupAnimationSet(LPD3DXANIMATIONCONTROLLER pAniCtrl)
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

void cSkinnedMeshEX::SetAnimationSet(UINT nTrack, LPCSTR szAniSetName)
{
	if (m_mapAniSet.find(szAniSetName) == m_mapAniSet.end()) return;

	m_pAniCtrl->SetTrackAnimationSet(nTrack, m_mapAniSet[szAniSetName]);
}

void cSkinnedMeshEX::SetAnimationSet(UINT nTrack, int nAniID, bool Loop)
{
	if (nTrack >= m_vecAniSetName.size()) return;
	if (!m_bAniStart)
	{
		m_startAniId = nAniID;
		m_bAniStart = true;
	}
	m_pAniCtrl->SetTrackAnimationSet(nTrack, (m_mapAniSet.find(m_vecAniSetName[nAniID]))->second);
	(m_mapAniSet.find(m_vecAniSetName[nAniID]))->second->GetPeriod();
	D3DXTRACK_DESC			tempDesc;
	m_bLoop = Loop;
}

void cSkinnedMeshEX::SetAnimationSetBlend(UINT nTrack, int nAniID, bool Loop)
{
	m_isBlend = true;
	m_fPassedBlendTime = 0.0f;
	m_bLoop = Loop;

	if (!m_bAniStart)
	{
		m_startAniId = nAniID;
		m_bAniStart = true;
	}

	int nMax = m_pAniCtrl->GetNumAnimationSets();
	if (nAniID > nMax)	nAniID = nAniID % nMax;

	LPD3DXANIMATIONSET	pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET	pNextAnimSet = NULL;
	
	m_pAniCtrl->GetTrackDesc(0, &m_stTrackDesc);
	
	m_pAniCtrl->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAniCtrl->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAniCtrl->SetTrackDesc(1, &m_stTrackDesc);


	m_pAniCtrl->GetAnimationSet(nAniID, &pNextAnimSet);
	m_pAniCtrl->SetTrackAnimationSet(0, pNextAnimSet);

	m_pAniCtrl->GetTrackDesc(0, &m_stTrackDesc);
	m_stTrackDesc.Position = 0.f;
	m_pAniCtrl->SetTrackDesc(0, &m_stTrackDesc);

	m_pAniCtrl->SetTrackWeight(0, 0.0f);
	m_pAniCtrl->SetTrackWeight(1, 1.0f);

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}