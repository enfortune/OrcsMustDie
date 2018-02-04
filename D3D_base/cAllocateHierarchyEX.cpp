#include "stdafx.h"
#include "cAllocateHierarchyEX.h"


cAllocateHierarchyEX::cAllocateHierarchyEX()
{
}


cAllocateHierarchyEX::~cAllocateHierarchyEX()
{
}

STDMETHODIMP cAllocateHierarchyEX::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	ST_BONE* pBone = new ST_BONE;
	ZeroMemory(pBone, sizeof(ST_BONE));

	if (Name)
	{
		pBone->Name = new char[strlen(Name) + 1];
		UINT length = lstrlen(Name) + 1;
		strcpy_s(pBone->Name, length * sizeof(TCHAR), Name);
	}
	else pBone->Name = NULL;

	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->matCombinedTransformMatrix);

	*ppNewFrame = pBone;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchyEX::CreateMeshContainer(
	LPCSTR Name, CONST D3DXMESHDATA * pMeshData,
	CONST D3DXMATERIAL * pMaterials,
	CONST D3DXEFFECTINSTANCE * pEffectInstances,
	DWORD NumMaterials, CONST DWORD * pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;
	ZeroMemory(pBoneMesh, sizeof(ST_BONE_MESH));

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		pBoneMesh->vecMtl.push_back(pMaterials[i].MatD3D);

		std::string sFullPath = m_sFolder;
		sFullPath = sFullPath + std::string("/")
			+ std::string((pMaterials[i].pTextureFilename) ? (pMaterials[i].pTextureFilename) : (""));

		pBoneMesh->vecTexture.push_back(
			g_pTextureManager->GetTexture(sFullPath));
	}

	if (pSkinInfo)
	{
		pSkinInfo->AddRef();
		pBoneMesh->pSkinInfo = pSkinInfo;

		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->pBoneOffsetMatrix = new D3DXMATRIX[dwNumBones];
		pBoneMesh->pCurrBoneMatrix = new D3DXMATRIX[dwNumBones];
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];

		for (DWORD i = 0; i < dwNumBones; i++)
		{
			pBoneMesh->pBoneOffsetMatrix[i] =
				(*pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}
	else
	{
		pBoneMesh->pBoneOffsetMatrix = nullptr;
		pBoneMesh->pCurrBoneMatrix = nullptr;
		pBoneMesh->ppBoneMatrixPtrs = nullptr;
	}

	pMeshData->pMesh->AddRef();
	pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
	pMeshData->pMesh->CloneMeshFVF(
		pMeshData->pMesh->GetOptions(),
		pMeshData->pMesh->GetFVF(),
		g_pD3DDevice,
		&pBoneMesh->pOrigMesh);

	pBoneMesh->MeshData.pMesh->GetAttributeTable(NULL, &pBoneMesh->nNumAttributeGroups);
	pBoneMesh->pAttributeTable = new D3DXATTRIBUTERANGE[pBoneMesh->nNumAttributeGroups];
	pBoneMesh->MeshData.pMesh->GetAttributeTable(pBoneMesh->pAttributeTable, NULL);

	*ppNewMeshContainer = pBoneMesh;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchyEX::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	ST_BONE* pBone = (ST_BONE*)pFrameToFree;
	SAFE_DELETE_ARRAY(pBone->Name);

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}

STDMETHODIMP cAllocateHierarchyEX::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;

	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->MeshData.pMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);

	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrix);
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrBoneMatrix);
	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);

	SAFE_DELETE(pMeshContainerToFree);

	return S_OK;
}
