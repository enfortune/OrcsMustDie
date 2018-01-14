#include "stdafx.h"
#include "D3DX_UTIL.h"

bool onSegment(D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r);
int orientation(D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r);
bool doIntersect(D3DXVECTOR2 p1, D3DXVECTOR2 q1, D3DXVECTOR2 p2, D3DXVECTOR2 q2);

namespace D3DX_UTIL
{
#define INF 10000


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