#pragma once

namespace D3DX_UTIL
{
	/* enum */
	typedef enum class DIRECTION_6
	{
		FRONT,
		REAR,
		LEFT,	// left
		RIGHT,	// right
		TOP,		// top
		BOTTOM,	// bottom
		END
	}DI6; // 4πÊ«‚
	
	/* struct */
	typedef struct ST_SPHERE
	{
		D3DXVECTOR3 vCenter;
		float fRadius;

		ST_SPHERE TransformCoord(D3DXMATRIXA16* pMat)
		{
			ST_SPHERE stRet;
			stRet.fRadius = fRadius;
			D3DXVec3TransformCoord(&stRet.vCenter, &vCenter, pMat);
			return stRet;
		}
	}SPHERE;

	typedef struct ST_FRUSTUM
	{
		D3DXVECTOR3 vNear_00;
		D3DXVECTOR3 vNear_01;
		D3DXVECTOR3 vNear_10;
		D3DXVECTOR3 vNear_11;
		D3DXVECTOR3 vFar_00;
		D3DXVECTOR3 vFar_01;
		D3DXVECTOR3 vFar_10;
		D3DXVECTOR3 vFar_11;

		ST_FRUSTUM TransformCoord(D3DXMATRIXA16* pMat)
		{
			ST_FRUSTUM stRet;
			D3DXVec3TransformCoord(&stRet.vNear_00, &vNear_00, pMat);
			D3DXVec3TransformCoord(&stRet.vNear_01, &vNear_01, pMat);
			D3DXVec3TransformCoord(&stRet.vNear_10, &vNear_10, pMat);
			D3DXVec3TransformCoord(&stRet.vNear_11, &vNear_11, pMat);
			D3DXVec3TransformCoord(&stRet.vFar_00, &vFar_00, pMat);
			D3DXVec3TransformCoord(&stRet.vFar_01, &vFar_01, pMat);
			D3DXVec3TransformCoord(&stRet.vFar_10, &vFar_10, pMat);
			D3DXVec3TransformCoord(&stRet.vFar_11, &vFar_11, pMat);
			return stRet;
		}

		D3DXVECTOR3 GetCenterVec3(DIRECTION_6 enDirection)
		{
			D3DXVECTOR3 vRet = D3DXVECTOR3(0.f, 0.f, 0.f);
			switch (enDirection)
			{
				case D3DX_UTIL::DIRECTION_6::FRONT:
					vRet = (vFar_00 + vFar_01 + vFar_10 + vFar_11) / 4.f;
				break;
				case D3DX_UTIL::DIRECTION_6::REAR:
					vRet = (vNear_00 + vNear_01 + vNear_10 + vNear_11) / 4.f;
				break;
				case D3DX_UTIL::DIRECTION_6::LEFT:
					vRet = (vNear_00 + vNear_01 + vFar_00 + vFar_01) / 4.f;
				break;
				case D3DX_UTIL::DIRECTION_6::RIGHT:
					vRet = (vNear_10 + vNear_11 + vFar_10 + vFar_11) / 4.f;
				break;
				case D3DX_UTIL::DIRECTION_6::TOP:
					vRet = (vNear_01 + vNear_11 + vFar_01 + vFar_11) / 4.f;
				break;
				case D3DX_UTIL::DIRECTION_6::BOTTOM:
					vRet = (vNear_00 + vNear_10 + vFar_00 + vFar_10) / 4.f;
				break;
			}
			return vRet;
		}
		D3DXVECTOR3 GetNormalVec3(DIRECTION_6 enDirection)
		{
			D3DXVECTOR3 vRet = D3DXVECTOR3(0.f, 0.f, 1.f);

			switch (enDirection)
			{
				case D3DX_UTIL::DIRECTION_6::FRONT: // far
					D3DXVec3Cross(&vRet, &(vFar_11 - vFar_10), &(vFar_10 - vFar_00));
				break;
				case D3DX_UTIL::DIRECTION_6::REAR: // near
					D3DXVec3Cross(&vRet, &(vNear_10 - vNear_00), &(vNear_11 - vNear_10));
				break;
				case D3DX_UTIL::DIRECTION_6::LEFT:
					D3DXVec3Cross(&vRet, &(vNear_01 - vNear_00), &(vFar_01 - vNear_01));
				break;
				case D3DX_UTIL::DIRECTION_6::RIGHT:
					D3DXVec3Cross(&vRet, &(vFar_10 - vNear_10), &(vFar_11 - vFar_10));
				break;
				case D3DX_UTIL::DIRECTION_6::TOP:
					D3DXVec3Cross(&vRet, &(vNear_11 - vNear_01), &(vFar_11 - vNear_11));
				break;
				case D3DX_UTIL::DIRECTION_6::BOTTOM:
					D3DXVec3Cross(&vRet, &(vFar_00 - vNear_00), &(vFar_10 - vFar_00));
				break;
				default:
				break;
			}
			D3DXVec3Normalize(&vRet, &vRet);
			return vRet;
		}
		D3DXVECTOR3 GetNearestSideNormalVec3(D3DXVECTOR3* pSour)
		{
			DIRECTION_6 enDir = DIRECTION_6::END;
			float fNearestDist = -1.f;
			float fDist;
			D3DXVECTOR3 vCenter;	

			for (int i = 0; i < static_cast<int>(DIRECTION_6::END); i++)
			{
				vCenter = GetCenterVec3(static_cast<DIRECTION_6>(i));
				fDist = D3DXVec3Length(&((*pSour) - vCenter));
				if (fNearestDist < 0.f || fNearestDist > fDist)
				{
					fNearestDist = fDist;
					enDir = static_cast<DIRECTION_6>(i);
				}
			}
			return -GetNormalVec3(enDir);
		}
	}FRUSTUM;

	/* function */
	DWORD FtoDw(float f);
	float GetRandomFloat(float fMin, float fMax);
	void GetRandomVec3(OUT D3DXVECTOR3* vOut, IN D3DXVECTOR3* vMin, IN D3DXVECTOR3* vMax);
	bool CheckPolygonContainVec2(std::vector<D3DXVECTOR2>& vecPolygon, D3DXVECTOR2 p);
	bool CheckAABBRectosahedronContainVec3(float nStartX, float nEndX, float nStartY, float nEndY, float nStartZ, float nEndZ, D3DXVECTOR3 v);
	bool CheckFrustumIntersectSphere(ST_FRUSTUM* pFrustum, ST_SPHERE* pSphere);
	bool CheckSphereIntersectSphere(ST_SPHERE* pSphere1, ST_SPHERE* pSphere2);
	bool CheckFrustumIntersectFrustum(ST_FRUSTUM* pFrustum1, ST_FRUSTUM* pFrustum2);
	ST_FRUSTUM GetViewFrustum();
}