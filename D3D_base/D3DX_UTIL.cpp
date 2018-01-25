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

			if (D3DXVec3Dot(&vN, &(v1 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(v2 - vC)) < 0) // 원의 일부조차 겹치지 않을 경우
				return false;
		}

		return true;
	}

	bool CheckSphereIntersectSphere(ST_SPHERE* pSphere1, ST_SPHERE* pSphere2)
	{
		if (D3DXVec3Length(&(pSphere1->vCenter - pSphere2->vCenter)) < pSphere1->fRadius + pSphere2->fRadius)
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

			if (D3DXVec3Dot(&vN, &(pFrustum1->vNear_00 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vNear_01 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vNear_10 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vNear_11 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_00 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_01 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_10 - vC)) < 0 &&
				D3DXVec3Dot(&vN, &(pFrustum1->vFar_11 - vC)) < 0) // 절두체의 일부조차 겹치지 않을 경우
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