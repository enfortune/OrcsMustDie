#include "stdafx.h"
#include "D3DX_UTIL.h"

bool onSegment(D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r);
int orientation(D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r);
bool doIntersect(D3DXVECTOR2 p1, D3DXVECTOR2 q1, D3DXVECTOR2 p2, D3DXVECTOR2 q2);

namespace D3DX_UTIL
{
#define INF 10000

	DWORD FtoDw(float f) { return *((DWORD*)&f); }

	float GetRandomFloat(float fMin, float fMax)
	{
		if (fMin >= fMax)
			return fMin;

		float f = (rand() % 10000) * 0.0001f;
		return (f * (fMax - fMin)) + fMin;
	}
	
	void GetRandomVec3(OUT D3DXVECTOR3* vOut, IN D3DXVECTOR3* vMin, IN D3DXVECTOR3* vMax)
	{
		vOut->x = GetRandomFloat(vMin->x, vMax->x);
		vOut->y = GetRandomFloat(vMin->y, vMax->y);
		vOut->z = GetRandomFloat(vMin->z, vMax->z);
	}


	bool CheckPolygonContainVec2(std::vector<D3DXVECTOR2>& vecPolygon, D3DXVECTOR2 p)
	{
		int n = static_cast<int>(vecPolygon.size());
		if (n < 3) return false;

		D3DXVECTOR2 extreme = D3DXVECTOR2(INF, p.y);

		int count = 0, i = 0;
		do
		{
			int next = (i + 1) % n;

			if (doIntersect(vecPolygon[i], vecPolygon[next], p, extreme))
			{

				if (orientation(vecPolygon[i], p, vecPolygon[next]) == 0)
					return onSegment(vecPolygon[i], p, vecPolygon[next]);

				count++;
			}
			i = next;
		} while (i != 0);

		return count & 1;
	}

	bool CheckAABBRectosahedronContainVec3(float nStartX, float nEndX, float nStartY, float nEndY, float nStartZ, float nEndZ, D3DXVECTOR3 v)
	{
		if (v.x >= nStartX && v.x <= nEndX &&
			v.y >= nStartY && v.y <= nEndY &&
			v.z >= nStartZ && v.z <= nEndZ) return true;
		else return false;
	}

	bool CheckFrustumIntersectSphere(ST_FRUSTUM* pFrustum, ST_SPHERE* pSphere)
	{
		D3DXVECTOR3 v1, v2 , vN, vC;
		for (int i = 0; i < static_cast<int>(DI6::END); i++)
		{
			vC = pFrustum->GetCenterVec3(static_cast<DIRECTION_6>(i));
			vN = pFrustum->GetNormalVec3(static_cast<DIRECTION_6>(i));
			v1 = pSphere->vCenter + pSphere->fRadius * vN;
			v2 = pSphere->vCenter - pSphere->fRadius * vN;

			if (D3DXVec3Dot(&vN, &(v1 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(v2 - vC)) <= 0) // 원의 일부조차 겹치지 않을 경우
				return false;
		}

		return true;
	}

	bool CheckSphereIntersectSphere(ST_SPHERE* pSphere1, ST_SPHERE* pSphere2)
	{
		if (D3DXVec3Length(&(pSphere1->vCenter - pSphere2->vCenter)) <= pSphere1->fRadius + pSphere2->fRadius)
			return true;
		else return false;
	}
	bool CheckFrustumIntersectFrustum(ST_FRUSTUM* pFrustum1, ST_FRUSTUM* pFrustum2)
	{
		D3DXVECTOR3 vN, vC;
		for (int i = 0; i < static_cast<int>(DI6::END); i++)
		{
			vC = pFrustum2->GetCenterVec3(static_cast<DIRECTION_6>(i));
			vN = pFrustum2->GetNormalVec3(static_cast<DIRECTION_6>(i));

			if (D3DXVec3Dot(&vN, &(pFrustum1->vNear_00 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vNear_01 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vNear_10 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vNear_11 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_00 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_01 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_10 - vC)) <= 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_11 - vC)) <= 0) // 절두체의 일부조차 겹치지 않을 경우
				return false;
		}
		return true;
	}

	ST_FRUSTUM GetViewFrustum()
	{
		ST_FRUSTUM stRet;
		D3DXMATRIXA16 matView, matProj, matInv;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		stRet.vNear_00 = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		stRet.vNear_01 = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
		stRet.vNear_10 = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		stRet.vNear_11 = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

		stRet.vFar_00 = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		stRet.vFar_01 = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		stRet.vFar_10 = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		stRet.vFar_11 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		D3DXMatrixInverse(&matInv, NULL, &(matView * matProj));

		D3DXVec3TransformCoord(&stRet.vNear_00, &stRet.vNear_00, &matInv);
		D3DXVec3TransformCoord(&stRet.vNear_01, &stRet.vNear_01, &matInv);
		D3DXVec3TransformCoord(&stRet.vNear_10, &stRet.vNear_10, &matInv);
		D3DXVec3TransformCoord(&stRet.vNear_11, &stRet.vNear_11, &matInv);
		D3DXVec3TransformCoord(&stRet.vFar_00, &stRet.vFar_00, &matInv);
		D3DXVec3TransformCoord(&stRet.vFar_01, &stRet.vFar_01, &matInv);
		D3DXVec3TransformCoord(&stRet.vFar_10, &stRet.vFar_10, &matInv);
		D3DXVec3TransformCoord(&stRet.vFar_11, &stRet.vFar_11, &matInv);

		return stRet;
	}


	bool CheckOBBCollision(ST_FRUSTUM* pFrustum1, ST_FRUSTUM* pFrustum2)
	{
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
		vPolygonNorm_1[0] = pFrustum1->GetNormalVec3(DIRECTION_6::RIGHT);	// x축
		vPolygonNorm_1[1] = pFrustum1->GetNormalVec3(DIRECTION_6::TOP);		// y축
		vPolygonNorm_1[2] = pFrustum1->GetNormalVec3(DIRECTION_6::FRONT);	// z축

		vPolygonNorm_2[0] = pFrustum2->GetNormalVec3(DIRECTION_6::RIGHT);
		vPolygonNorm_2[1] = pFrustum2->GetNormalVec3(DIRECTION_6::TOP);
		vPolygonNorm_2[2] = pFrustum2->GetNormalVec3(DIRECTION_6::FRONT);

		//1-2 엣지의 절반길이
		vEdgeHalfLength_1[0] = 0.5f * D3DXVec3Length(&(pFrustum1->vNear_10 - pFrustum1->vNear_00));	// x축
		vEdgeHalfLength_1[1] = 0.5f * D3DXVec3Length(&(pFrustum1->vNear_01 - pFrustum1->vNear_00));	// y축
		vEdgeHalfLength_1[2] = 0.5f * D3DXVec3Length(&(pFrustum1->vFar_00 - pFrustum1->vNear_00));	// z축

		vEdgeHalfLength_2[0] = 0.5f * D3DXVec3Length(&(pFrustum2->vNear_10 - pFrustum2->vNear_00));	// x축
		vEdgeHalfLength_2[1] = 0.5f * D3DXVec3Length(&(pFrustum2->vNear_01 - pFrustum2->vNear_00));	// y축
		vEdgeHalfLength_2[2] = 0.5f * D3DXVec3Length(&(pFrustum2->vFar_00 - pFrustum2->vNear_00));	// z축

		//1-3 중심간 차이
		vCenterInterval = pFrustum2->GetCenterVec3(DIRECTION_6::END) - pFrustum1->GetCenterVec3(DIRECTION_6::END);

		//2. 각 면에 대한 분리축을 테스트한다.
		//2-1 직육면체 1의 면의 법선벡터 테스트
		for (int i = 0; i < 3; i++)
		{
			vSA = vPolygonNorm_1[i]; // 축 하나를 선택한 후
			/*
			for (int j = 0; j < 8; j++) // 직육면체 1의 최대, 최소 거리상수 구함
			{
				fDistTemp_1 = D3DXVec3Dot(&(*pFrustum1)[j], &vSA);
				if (j == 0) fDistMin_1 = fDistMax_1 = fDistTemp_1;
				else
				{
					if (fDistTemp_1 < fDistMin_1) 
						fDistMin_1 = fDistTemp_1;
					else if (fDistTemp_1 > fDistMax_1) 
						fDistMax_1 = fDistTemp_1;
				}
			}
			for (int j = 0; j < 8; j++) // 직육면체 2의 최대, 최소 거리상수 구함
			{
				fDistTemp_2 = D3DXVec3Dot(&(*pFrustum2)[j], &vSA);
				if (j == 0) fDistMin_2 = fDistMax_2 = fDistTemp_2;
				else
				{
					if (fDistTemp_2 < fDistMin_2) fDistMin_2 = fDistTemp_2;
					else if (fDistTemp_2 > fDistMax_2) fDistMax_2 = fDistTemp_2;
				}
			}
			if (fDistMax_1 < fDistMin_2 || fDistMin_1 > fDistMax_2) return false; // 분리축이 존재하면 false를 리턴
			*/
			fCenterDist = fabs(D3DXVec3Dot(&vSA, &vCenterInterval));
			fRadius_1 =
				fabs(vEdgeHalfLength_1[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[0])) +
				fabs(vEdgeHalfLength_1[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[1])) +
				fabs(vEdgeHalfLength_1[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[2]));

			fRadius_2 = 
				fabs(vEdgeHalfLength_2[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[0])) +
				fabs(vEdgeHalfLength_2[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[1])) +
				fabs(vEdgeHalfLength_2[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[2]));

			if (fCenterDist > fRadius_1 + fRadius_2) return false;
		}
		//2-2 직육면체 2의 면의 법선벡터 테스트
		for (int i = 0; i < 3; i++)
		{
			vSA = vPolygonNorm_2[i]; // 축 하나를 선택한 후
			/*
			for (int j = 0; j < 8; j++) // 직육면체 1의 최대, 최소 거리상수 구함
			{
				fDistTemp_1 = D3DXVec3Dot(&(*pFrustum1)[j], &vSA);
				if (j == 0) fDistMin_1 = fDistMax_1 = fDistTemp_1;
				else
				{
					if (fDistTemp_1 < fDistMin_1) fDistMin_1 = fDistTemp_1;
					else if (fDistTemp_1 > fDistMax_1) fDistMax_1 = fDistTemp_1;
				}
			}
			for (int j = 0; j < 8; j++) // 직육면체 2의 최대, 최소 거리상수 구함
			{
				fDistTemp_2 = D3DXVec3Dot(&(*pFrustum2)[j], &vSA);
				if (j == 0) fDistMin_2 = fDistMax_2 = fDistTemp_2;
				else
				{
					if (fDistTemp_2 < fDistMin_2) fDistMin_2 = fDistTemp_2;
					else if (fDistTemp_2 > fDistMax_2) fDistMax_2 = fDistTemp_2;
				}
			}
			if (fDistMax_1 < fDistMin_2 || fDistMin_1 > fDistMax_2) return false; // 분리축이 존재하면 false를 리턴
			*/

			fCenterDist = fabs(D3DXVec3Dot(&vSA, &vCenterInterval));
			fRadius_1 =
				fabs(vEdgeHalfLength_1[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[0])) +
				fabs(vEdgeHalfLength_1[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[1])) +
				fabs(vEdgeHalfLength_1[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[2]));

			fRadius_2 =
				fabs(vEdgeHalfLength_2[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[0])) +
				fabs(vEdgeHalfLength_2[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[1])) +
				fabs(vEdgeHalfLength_2[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[2]));

			if (fCenterDist > fRadius_1 + fRadius_2) return false;
		}

		//3. 두 모서리의 외적으로 생기는 평면에 대한 분리축 테스트
		for (int _1 = 0; _1 < 3; _1++)
		{
			for (int _2 = 0; _2 < 3; _2++)
			{
				D3DXVec3Cross(&vSA, &vPolygonNorm_1[_1], &vPolygonNorm_2[_2]);
				D3DXVec3Normalize(&vSA, &vSA);
				/*
				for (int j = 0; j < 8; j++) // 직육면체 1의 최대, 최소 거리상수 구함
				{
					fDistTemp_1 = D3DXVec3Dot(&(*pFrustum1)[j], &vSA);
					if (j == 0) fDistMin_1 = fDistMax_1 = fDistTemp_1;
					else
					{
						if (fDistTemp_1 < fDistMin_1) fDistMin_1 = fDistTemp_1;
						else if (fDistTemp_1 > fDistMax_1) fDistMax_1 = fDistTemp_1;
					}
				}
				for (int j = 0; j < 8; j++) // 직육면체 2의 최대, 최소 거리상수 구함
				{
					fDistTemp_2 = D3DXVec3Dot(&(*pFrustum2)[j], &vSA);
					if (j == 0) fDistMin_2 = fDistMax_2 = fDistTemp_2;
					else
					{
						if (fDistTemp_2 < fDistMin_2) fDistMin_2 = fDistTemp_2;
						else if (fDistTemp_2 > fDistMax_2) fDistMax_2 = fDistTemp_2;
					}
				}
				if (fDistMax_1 < fDistMin_2 || fDistMin_1 > fDistMax_2) return false; // 분리축이 존재하면 false를 리턴
				*/

				fCenterDist = fabs(D3DXVec3Dot(&vSA, &vCenterInterval));
				fRadius_1 =
					fabs(vEdgeHalfLength_1[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[0])) +
					fabs(vEdgeHalfLength_1[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[1])) +
					fabs(vEdgeHalfLength_1[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_1[2]));

				fRadius_2 =
					fabs(vEdgeHalfLength_2[0] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[0])) +
					fabs(vEdgeHalfLength_2[1] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[1])) +
					fabs(vEdgeHalfLength_2[2] * D3DXVec3Dot(&vSA, &vPolygonNorm_2[2]));

				if (fCenterDist > fRadius_1 + fRadius_2) return false;
			}
		}


		return true;
	}
}



///////////


bool onSegment(D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}
int orientation(D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r)
{
	int val = static_cast<int>((q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y));

	if (val == 0) return 0;
	return (val > 0) ? 1 : 2;
}
bool doIntersect(D3DXVECTOR2 p1, D3DXVECTOR2 q1, D3DXVECTOR2 p2, D3DXVECTOR2 q2)
{
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	if (o1 != o2 && o3 != o4)
		return true;

	if (o1 == 0 && onSegment(p1, p2, q1)) return true;
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false;
}