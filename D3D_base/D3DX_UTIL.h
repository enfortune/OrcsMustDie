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
	}DI6; // 4방향
	
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
			D3DXVECTOR3 vC;
			D3DXVECTOR3 vN;
			D3DXVECTOR3 vTemp;
			float fCalc[static_cast<int>(DI6::END)];
			float fComp = -1.f;
			float fTemp;
			for (int i = 0; i < static_cast<int>(DI6::END); i++)
			{
				fCalc[i] = 0.f;

				vC = GetCenterVec3(static_cast<DIRECTION_6>(i));
				vN = GetNormalVec3(static_cast<DIRECTION_6>(i));

				/*fCalc[i] += D3DXVec3Length(&(pSour->vNear_00 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vNear_01 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vNear_10 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vNear_11 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vFar_00 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vFar_01 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vFar_10 - vC));
				fCalc[i] += D3DXVec3Length(&(pSour->vFar_11 - vC));*/


				D3DXVec3Normalize(&vTemp, &(pSour->vNear_00 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vNear_01 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vNear_10 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vNear_11 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vFar_00 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vFar_01 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vFar_10 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
				D3DXVec3Normalize(&vTemp, &(pSour->vFar_11 - vC));
				fTemp = D3DXVec3Dot(&vN, &vTemp);
				if (fTemp > 0)
				{
					fCalc[i] += fTemp;
				}
			}

			for (int i = 0; i < static_cast<int>(DI6::END); i++)
			{
				if (fComp < 0 || fCalc[i] < fComp)
				{
					fComp = fCalc[i];
					enDir = static_cast<DIRECTION_6>(i);
				}
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