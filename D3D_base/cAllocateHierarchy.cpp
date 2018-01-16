#include "stdafx.h"
#include "cAllocateHierarchy.h"


cAllocateHierarchy::cAllocateHierarchy()
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, OUT LPD3DXFRAME *ppNewFrame)
{
	//D3DXFRAME stFrame;
	ST_BONE*	pBone = new ST_BONE;
	ZeroMemory(pBone, sizeof(ST_BONE));

	if (Name)
	{
		pBone->Name = new char[strlen(Name) + 1];
		UINT length = lstrlenA(Name) + 1;
		strcpy_s(pBone->Name, length * sizeof(TCHAR), Name);
	}
	
	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->matCombinedTransformMatrix);

	OUT *ppNewFrame = pBone;

	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(THIS_
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances, // 셰이더 관련
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	OUT LPD3DXMESHCONTAINER *ppNewMeshContainer)
{

	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;
	ZeroMemory(pBoneMesh, sizeof(ST_BONE_MESH));

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		pBoneMesh->vecMtl.push_back(pMaterials[i].MatD3D);

		std::string  sFullPath = m_sFolder;
		sFullPath = sFullPath + std::string("/") 
			+ std::string(pMaterials[i].pTextureFilename);

		pBoneMesh->vecTexture.push_back(
			g_pTextureManager->GetTexture(sFullPath));
	}

	// 1. skin info 저장 - 2. D3DXMESHDATA 구조체의 원본메시데이터...?

	pSkinInfo->AddRef();
	pBoneMesh->pSkinInfo = pSkinInfo;
	pMeshData->pMesh->AddRef();
	pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
	pMeshData->pMesh->CloneMeshFVF(
		pMeshData->pMesh->GetOptions(),
		pMeshData->pMesh->GetFVF(),
		g_pD3DDevice,
		&pBoneMesh->pOrigMesh
		);
	DWORD dwNumBones = pSkinInfo->GetNumBones();
	pBoneMesh->pBoneOffsetMatrix = new D3DXMATRIX[dwNumBones];
	pBoneMesh->pCurrBoneMatrix	= new D3DXMATRIX[dwNumBones];
	pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];

	for (DWORD i = 0; i < dwNumBones; i++)
	{
		pBoneMesh->pBoneOffsetMatrix[i] =
			(*pSkinInfo->GetBoneOffsetMatrix(i));
	}

	OUT *ppNewMeshContainer = (LPD3DXMESHCONTAINER)pBoneMesh;

	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	ST_BONE* pBone = (ST_BONE*)pFrameToFree;
	SAFE_DELETE_ARRAY(pBone->Name);

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}
STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
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