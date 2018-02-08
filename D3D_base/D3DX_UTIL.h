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
		TOP,	// top
		BOTTOM,	// bottom
		END
	}DI6; // 4방향
	typedef struct ST_INDEX_XYZ
	{
		int x;
		int y;
		int z;
		ST_INDEX_XYZ(int nX = 0, int nY = 0, int nZ = 0) : x(nX), y(nY), z(nZ)
		{}
	}INDEX_XYZ;
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

		void MakeWithVec3MinMax(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
		{
			vNear_00 = vMin;
			vNear_01 = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
			vNear_10 = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
			vNear_11 = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);

			vFar_00 = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
			vFar_01 = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
			vFar_10 = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
			vFar_11 = vMax;
		}
		D3DXVECTOR3& operator[] (int n)
		{
			assert((n >= 0 && n < 8) &&  "FRUSTUM의 인덱스를 벗어났음");
			if (n == 0) return vNear_00;
			if (n == 1) return vNear_01;
			if (n == 2) return vNear_10;
			if (n == 3) return vNear_11;
			if (n == 4) return vFar_00;
			if (n == 5) return vFar_01;
			if (n == 6) return vFar_10;
			if (n == 7) return vFar_11;
		}
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
		void Render(LPDIRECT3DDEVICE9 lpDevice)
		{
			std::vector<D3DXVECTOR3> vecTemp(8);
			
			lpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			lpDevice->SetFVF(D3DFVF_XYZ);

			// 뒤
			vecTemp[0] = vecTemp[7] = vNear_00;
			vecTemp[2] = vecTemp[1] = vNear_01;
			vecTemp[4] = vecTemp[3] = vNear_11;
			vecTemp[6] = vecTemp[5] = vNear_10;
			lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecTemp[0], sizeof(D3DXVECTOR3));

			// 앞
			vecTemp[0] = vecTemp[7] = vFar_10;
			vecTemp[2] = vecTemp[1] = vFar_11;
			vecTemp[4] = vecTemp[3] = vFar_01;
			vecTemp[6] = vecTemp[5] = vFar_00;
			lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecTemp[0], sizeof(D3DXVECTOR3));

			// 위
			vecTemp[0] = vecTemp[7] = vNear_01;
			vecTemp[2] = vecTemp[1] = vFar_01;
			vecTemp[4] = vecTemp[3] = vFar_11;
			vecTemp[6] = vecTemp[5] = vNear_11;
			lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecTemp[0], sizeof(D3DXVECTOR3));

			// 아래
			vecTemp[0] = vecTemp[7] = vNear_10;
			vecTemp[2] = vecTemp[1] = vFar_10;
			vecTemp[4] = vecTemp[3] = vFar_00;
			vecTemp[6] = vecTemp[5] = vNear_00;
			lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecTemp[0], sizeof(D3DXVECTOR3));

			// 오른쪽
			vecTemp[0] = vecTemp[7] = vNear_10;
			vecTemp[2] = vecTemp[1] = vNear_11;
			vecTemp[4] = vecTemp[3] = vFar_11;
			vecTemp[6] = vecTemp[5] = vFar_10;
			lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecTemp[0], sizeof(D3DXVECTOR3));

			// 왼쪽
			vecTemp[0] = vecTemp[7] = vFar_00;
			vecTemp[2] = vecTemp[1] = vFar_01;
			vecTemp[4] = vecTemp[3] = vNear_01;
			vecTemp[6] = vecTemp[5] = vNear_00;
			lpDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecTemp[0], sizeof(D3DXVECTOR3));

			lpDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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
				case D3DX_UTIL::DIRECTION_6::END:
					vRet = (vNear_00 + vNear_01 + vNear_10 + vNear_11 + vFar_00 + vFar_01 + vFar_10 + vFar_11) / 8.f;
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
		/*D3DXVECTOR3 GetNearestSideNormalVec3(D3DXVECTOR3* pSour)
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
		}*/
		D3DXVECTOR3 GetNearestSideNormalVec3(D3DXVECTOR3* pSour)
		{
			DIRECTION_6 enDir = DIRECTION_6::END;

			D3DXVECTOR3 vCenter = GetCenterVec3(DIRECTION_6::END); // 중점을 찾은 후
			D3DXVECTOR3 vPlaneCenter;
			D3DXVECTOR3 vPlaneDist;
			D3DXVECTOR3 vDist;
			float fRatio;
			float fResult;
			float fNearestResult= -1;

			for (int i = 0; i < static_cast<int>(DIRECTION_6::END); i++)
			{
				vPlaneCenter = GetCenterVec3(static_cast<DIRECTION_6>(i));
				vPlaneDist = vPlaneCenter - vCenter;
				fRatio = D3DXVec3Length(&vPlaneDist);
				vDist = (*pSour) - vCenter;

				fResult = D3DXVec3Dot(&vPlaneDist, &vDist) / fRatio;

				if (fNearestResult < 0.f || fNearestResult < fResult)
				{
					fNearestResult = fResult;
					enDir = static_cast<DIRECTION_6>(i);
				}
			}
			return -GetNormalVec3(enDir);
		}
		D3DXVECTOR3 GetNearestSideNormalVec3(ST_FRUSTUM* pSour)
		{
			DIRECTION_6 enDir = DIRECTION_6::END;

			float fCalc[3];
			bool bPositive[3];
			float fComp = -1.f;
			float fDotTemp;

			D3DXVECTOR3 vPolygonNorm_1[3]; // 면의 법선벡터이자 그 면의 수직인 모서리의 벡터
			D3DXVECTOR3 vPolygonNorm_2[3];
			float vEdgeHalfLength_1[3]; // 각 축 모서리의 절반길이.
			float vEdgeHalfLength_2[3];

			D3DXVECTOR3 vSA; // Separating Axis(분리축)
			float fDistMin_1, fDistMax_1, fDistTemp_1; // 직육면체 1의 분리축 거리상수
			float fDistMin_2, fDistMax_2, fDistTemp_2; // 직육면체 2의 분리축 거리상수
			D3DXVECTOR3 vCenterInterval; // 두 정육면체의 중심간 차이 벡터
			float fCenterDist, fRadius_1, fRadius_2; // 중심간 거리, 직육면체1의 분리축반지름, 직육면체2의 분리축반지름

			//1. 필요한 벡터를 준비한다.
			//1-1 면의 법선벡터
			vPolygonNorm_1[0] = -GetNormalVec3(DIRECTION_6::RIGHT);	// x축
			vPolygonNorm_1[1] = -GetNormalVec3(DIRECTION_6::TOP);	// y축
			vPolygonNorm_1[2] = -GetNormalVec3(DIRECTION_6::FRONT);	// z축

			vPolygonNorm_2[0] = -pSour->GetNormalVec3(DIRECTION_6::RIGHT);
			vPolygonNorm_2[1] = -pSour->GetNormalVec3(DIRECTION_6::TOP);
			vPolygonNorm_2[2] = -pSour->GetNormalVec3(DIRECTION_6::FRONT);

			//1-2 엣지의 절반길이
			vEdgeHalfLength_1[0] = 0.5f * D3DXVec3Length(&(vNear_10 - vNear_00));	// x축
			vEdgeHalfLength_1[1] = 0.5f * D3DXVec3Length(&(vNear_01 - vNear_00));	// y축
			vEdgeHalfLength_1[2] = 0.5f * D3DXVec3Length(&(vFar_00 - vNear_00));	// z축

			vEdgeHalfLength_2[0] = 0.5f * D3DXVec3Length(&(pSour->vNear_10 - pSour->vNear_00));	// x축
			vEdgeHalfLength_2[1] = 0.5f * D3DXVec3Length(&(pSour->vNear_01 - pSour->vNear_00));	// y축
			vEdgeHalfLength_2[2] = 0.5f * D3DXVec3Length(&(pSour->vFar_00 - pSour->vNear_00));	// z축

			//1-3 중심간 차이
			vCenterInterval = pSour->GetCenterVec3(DIRECTION_6::END) - GetCenterVec3(DIRECTION_6::END);

			//2. 각 면에 대한 분리축을 테스트한다.
			//2-1 직육면체 1의 면의 법선벡터 테스트
			for (int i = 0; i < 3; i++)
			{
				vSA = vPolygonNorm_1[i]; // 축 하나를 선택한 후

				fDotTemp = D3DXVec3Dot(&vSA, &vCenterInterval);
				bPositive[i] = (fDotTemp > 0) ? (true) : (false);

				fCenterDist = fabs(fDotTemp);
				fRadius_1 =
					fabs(vEdgeHalfLength_1[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[0])) +
					fabs(vEdgeHalfLength_1[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[1])) +
					fabs(vEdgeHalfLength_1[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[2]));

				fRadius_2 =
					fabs(vEdgeHalfLength_2[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[0])) +
					fabs(vEdgeHalfLength_2[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[1])) +
					fabs(vEdgeHalfLength_2[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[2]));

				//if (fCenterDist > fRadius_1 + fRadius_2) return false;
				fCalc[i] = fCenterDist - (fRadius_1 + fRadius_2);
			}

			// 0 : RIGHT, 1 : TOP, 2 : FRONT

			
			fComp = fCalc[0];
			enDir = (bPositive[0])? (DIRECTION_6::RIGHT) : (DIRECTION_6::LEFT);
			
			if (fCalc[1] > fComp)
			{
				fComp = fCalc[1];
				enDir = (bPositive[1]) ? (DIRECTION_6::TOP) : (DIRECTION_6::BOTTOM);
			}
			if (fCalc[2] > fComp)
			{
				fComp = fCalc[2];
				enDir = (bPositive[2]) ? (DIRECTION_6::FRONT) : (DIRECTION_6::REAR);
			}

			return -GetNormalVec3(enDir);
		}


		float GetHighestPoint()
		{
			float fRet = vNear_00.y;
			if (fRet < vNear_01.y) fRet = vNear_01.y;
			if (fRet < vNear_10.y) fRet = vNear_10.y;
			if (fRet < vNear_11.y) fRet = vNear_11.y;
			if (fRet < vFar_00.y) fRet = vFar_00.y;
			if (fRet < vFar_01.y) fRet = vFar_01.y;
			if (fRet < vFar_10.y) fRet = vFar_10.y;
			if (fRet < vFar_11.y) fRet = vFar_11.y;

			return fRet;
		}
		float GetLowestPoint()
		{
			float fRet = vNear_00.y;
			if (fRet > vNear_01.y) fRet = vNear_01.y;
			if (fRet > vNear_10.y) fRet = vNear_10.y;
			if (fRet > vNear_11.y) fRet = vNear_11.y;
			if (fRet > vFar_00.y) fRet = vFar_00.y;
			if (fRet > vFar_01.y) fRet = vFar_01.y;
			if (fRet > vFar_10.y) fRet = vFar_10.y;
			if (fRet > vFar_11.y) fRet = vFar_11.y;
			return fRet;
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
	bool CheckOBBCollision(ST_FRUSTUM* pFrustum1, ST_FRUSTUM* pFrustum2);
	ST_FRUSTUM GetViewFrustum();
}