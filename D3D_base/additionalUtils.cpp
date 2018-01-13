#include "stdafx.h"
#include "additionalUtils.h"


//CPP 구현부 (실제 사용시 이 아래는 .cpp파일로 분리할 것)
namespace THETA_UTIL
{
	fPOINT fPointMake(float x, float y)
	{
		fPOINT fpt;
		fpt.x = x;
		fpt.y = y;
		return fpt;
	}
	fPOINT PtTofPt(POINT pt)
	{
		fPOINT fpt;
		fpt.x = (float)pt.x;
		fpt.y = (float)pt.y;
		return fpt;
	}
	POINT fPtToPt(fPOINT fpt)
	{
		POINT pt;
		pt = { (LONG)fpt.x, (LONG)fpt.y };
		return pt;
	}
	fRECT RectTofRect(RECT rc)
	{
		fRECT frc;
		frc.left = rc.left;
		frc.top = rc.top;
		frc.right = rc.right;
		frc.bottom = rc.bottom;
		return frc;
	}
	RECT fRectToRect(fRECT frc)
	{
		RECT rc;
		rc.left = frc.left;
		rc.top = frc.top;
		rc.right = frc.right;
		rc.bottom = frc.bottom;
		return rc;
	}
	fRECT fRectMake(float left, float top, float width, float height)
	{
		fRECT frc;
		frc.left = left;
		frc.top = top;
		frc.right = left + width;
		frc.bottom = top + height;
		return frc;
	}
	fRECT fRectMakeCenter(float centerX, float centerY, float width, float height)
	{
		fRECT frc;
		frc.left = centerX - width / 2;
		frc.top = centerY - height / 2;
		frc.right = centerX + width / 2;
		frc.bottom = centerY + height / 2;
		return frc;
	}
	LONG GetRectWidth(RECT rc)
	{
		return rc.right - rc.left;
	}
	LONG GetRectHeight(RECT rc)
	{
		return rc.bottom - rc.top;
	}
	POINT GetRectCenter(RECT rc)
	{
		POINT temp;
		temp.x = (rc.left + rc.right) / 2;
		temp.y = (rc.top + rc.bottom) / 2;
		return temp;
	}
	float GetRectWidth(fRECT frc)
	{
		return frc.right - frc.left;
	}
	float GetRectHeight(fRECT frc)
	{
		return frc.bottom - frc.top;
	}
	fPOINT GetRectCenter(fRECT frc)
	{
		fPOINT temp;
		temp.x = (frc.left + frc.right) / 2;
		temp.y = (frc.top + frc.bottom) / 2;
		return temp;
	}
	void RectangleMakeRect(HDC hdc, RECT rc)
	{
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	}
	void EllipseMakeRect(HDC hdc, RECT rc)
	{
		Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
	}
	void LineMakeRect(HDC hdc, RECT rc)
	{
		MoveToEx(hdc, rc.left, rc.top, NULL);
		LineTo(hdc, rc.right, rc.top);
		MoveToEx(hdc, rc.right, rc.top, NULL);
		LineTo(hdc, rc.right, rc.bottom);
		MoveToEx(hdc, rc.right, rc.bottom, NULL);
		LineTo(hdc, rc.left, rc.bottom);
		MoveToEx(hdc, rc.left, rc.bottom, NULL);
		LineTo(hdc, rc.left, rc.top);
	}
	void LineMakeRect(HDC hdc, int left, int top, int right, int bottom)
	{
		MoveToEx(hdc, left, top, NULL);
		LineTo(hdc, right, top);
		MoveToEx(hdc, right, top, NULL);
		LineTo(hdc, right, bottom);
		MoveToEx(hdc, right, bottom, NULL);
		LineTo(hdc, left, bottom);
		MoveToEx(hdc, left, bottom, NULL);
		LineTo(hdc, left, top);
	}
	void RapidStretchBlt(HDC destDC, int destX, int destY, int destW, int destH,
		HDC sourDC, int sourX, int sourY, int sourW, int sourH)
	{
		int old = SetStretchBltMode(destDC, COLORONCOLOR);
		StretchBlt(destDC, destX, destY, destW, destH, sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
		SetStretchBltMode(destDC, old);
	}
	void ExactStretchBlt(HDC destDC, int destX, int destY, int destW, int destH,
		HDC sourDC, int sourX, int sourY, int sourW, int sourH)
	{
		int old = SetStretchBltMode(destDC, HALFTONE);
		StretchBlt(destDC, destX, destY, destW, destH, sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
		SetStretchBltMode(destDC, old);
	}
	void ExtendBlt(HDC destDC, int destX, int destY, int destW, int destH,
		HDC sourDC, int sourX, int sourY, int sourW, int sourH,
		bool isTrans, COLORREF transColor)
	{
		if (isTrans)
		{
			HDC hdcTemp = GetDC(g_hWnd);
			HDC hdcReverse;
			HBITMAP hbitTemp;
			short diWidth = 1;
			short diHeight = 1;
			if (destW < 0) diWidth = -1;
			if (destH < 0) diHeight = -1;
			hdcReverse = CreateCompatibleDC(hdcTemp);
			hbitTemp = CreateCompatibleBitmap(hdcTemp, sourW, sourH);
			SelectObject(hdcReverse, hbitTemp);
			//_imageInfo->hMemDC
			SetStretchBltMode(hdcReverse, COLORONCOLOR);
			switch (diWidth)
			{
			case 1:
				switch (diHeight)
				{
				case 1:
					PatBlt(hdcReverse, 0, 0, sourW, sourH, WHITENESS);
					StretchBlt(hdcReverse, 0, 0, sourW, sourH,
						sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
					GdiTransparentBlt(destDC,
						destX,
						destY,
						diWidth*destW,
						diHeight*destH,
						hdcReverse,
						0, 0,
						sourW,
						sourH,
						transColor);
					break;
				case -1:
					PatBlt(hdcReverse, 0, 0, sourW, sourH, WHITENESS);
					StretchBlt(hdcReverse, 0, sourH, sourW, -sourH - 1,
						sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
					GdiTransparentBlt(destDC,
						destX,
						destY - diHeight*destH,
						diWidth*destW,
						diHeight*destH,
						hdcReverse,
						0, 0,
						sourW,
						sourH,
						transColor);
					break;
				}
				break;
			case -1:
				switch (diHeight)
				{
				case 1:
					PatBlt(hdcReverse, 0, 0, sourW, sourH, WHITENESS);
					StretchBlt(hdcReverse, sourW, 0, -sourW - 1, sourH,
						sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
					GdiTransparentBlt(destDC,
						destX - diWidth*destW,
						destY,
						diWidth*destW,
						diHeight*destH,
						hdcReverse,
						0, 0,
						sourW,
						sourH,
						transColor);
					break;
				case -1:
					PatBlt(hdcReverse, 0, 0, sourW, sourH, WHITENESS);
					StretchBlt(hdcReverse, sourW, sourH, -sourW - 1, -sourH - 1,
						sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
					GdiTransparentBlt(destDC,
						destX - diWidth*destW,
						destY - diHeight*destH,
						diWidth*destW,
						diHeight*destH,
						hdcReverse,
						0, 0,
						sourW,
						sourH,
						transColor);
					break;
				}
				break;
			}
			ReleaseDC(g_hWnd, hdcTemp);
			DeleteDC(hdcTemp);
			DeleteObject(hbitTemp);
			DeleteDC(hdcReverse);
		}
		else
		{
			//백버퍼에 있는 것을 앞으로 고속복사 해준다
			SetStretchBltMode(destDC, COLORONCOLOR);
			StretchBlt(destDC, destX, destY, destW, destH,
				sourDC, sourX, sourY, sourW, sourH, SRCCOPY);
		}
	}
	float DegToRad(float degree)
	{
		return degree / 180 * PI;
	}
	float RadToDeg(float radian)
	{
		return radian / PI * 180;
	}
	float diToAngle(DIRECTION_8 di) // 치역은 0<= ~ <2PI
	{
		switch (di)
		{
			case THETA_UTIL::DIRECTION_8::L:
				return -PI;
			break;
			case THETA_UTIL::DIRECTION_8::R:
				return 0;
			break;
			case THETA_UTIL::DIRECTION_8::T:
				return -PI / 2;
			break;
			case THETA_UTIL::DIRECTION_8::B:
				return -PI*3/ 2;
			break;
			case THETA_UTIL::DIRECTION_8::LT:
				return -PI * 3 / 4;
			break;
			case THETA_UTIL::DIRECTION_8::RT:
				return -PI * 1 / 4;
			break;
			case THETA_UTIL::DIRECTION_8::LB:
				return -PI *5 / 4;
			break;
			case THETA_UTIL::DIRECTION_8::RB:
				return -PI * 7 / 4;
			break;
		}
		return 0;
	}
	float diToAngle(DIRECTION_4 di) // 치역은 0<= ~ <2PI
	{
		switch (di)
		{
			case THETA_UTIL::DIRECTION_4::LEFT:
				return -PI;
			break;
			case THETA_UTIL::DIRECTION_4::RIGHT:
				return 0;
			break;
			case THETA_UTIL::DIRECTION_4::TOP:
				return -PI / 2;
			break;
			case THETA_UTIL::DIRECTION_4::BOTTOM:
				return -PI * 3 / 2;
			break;
		}
		return 0;
	}
	int SetRotateImageFrame(float startAngle, int nFrame, bool isClockWise, float currentAngle)
	{
		float tempSAngle = startAngle;
		float tempCAngle = currentAngle;
		while (tempSAngle < -PI || tempSAngle > PI) // 각보정
		{
			if (tempSAngle < -PI)
			{
				tempSAngle += PI2;
			}
			else if (tempSAngle > PI)
			{
				tempSAngle -= PI2;
			}
		}
		if (isClockWise) // 시계방향
		{
			while (tempCAngle < tempSAngle - PI / nFrame
				|| tempCAngle > tempSAngle + PI2 - PI / nFrame) // 각보정
			{
				if (tempCAngle < tempSAngle - PI / nFrame)
				{
					tempCAngle += PI2;
				}
				else if (tempCAngle >= tempSAngle + PI2 - PI / nFrame)
				{
					tempCAngle -= PI2;
				}
			}
			for (int i = 0; i < nFrame; i++)
			{
				if (tempCAngle - tempSAngle >= -PI / nFrame + i*PI2 / nFrame
					&& tempCAngle - tempSAngle <= -PI / nFrame + (i + 1)*PI2 / nFrame)
					return i;
			}
		}
		else // 반시계방향
		{
			while (tempCAngle < tempSAngle - PI2 + PI / nFrame 
				|| tempCAngle > tempSAngle + PI / nFrame) // 각보정
			{
				if (tempCAngle < tempSAngle - PI2 + PI / nFrame)
				{
					tempCAngle += PI2;
				}
				else if (tempCAngle >= tempSAngle + PI / nFrame)
				{
					tempCAngle -= PI2;
				}
			}
			for (int i = 0; i < nFrame; i++)
			{
				if (tempCAngle - tempSAngle <= +PI / nFrame - i*PI2 / nFrame
					&& tempCAngle - tempSAngle >= +PI / nFrame - (i + 1)*PI2 / nFrame)
					return i;
			}
		}
		return 0; // 예외처리
	}

	//충돌체크
	bool CheckRectIntersectRect(LPfRECT inter, LPRECT rect1, LPRECT rect2)
	{
		RECT temp;
		bool isCollision = false;
		isCollision = IntersectRect(&temp, rect1, rect2);
		inter->left = temp.left;
		inter->top = temp.top;
		inter->right = temp.right;
		inter->bottom = temp.bottom;
		return isCollision;
	}
	bool CheckRectIntersectRect(LPfRECT inter, LPfRECT fRect1, LPfRECT fRect2)
	{
		bool isCollision = false;
		
		if (fRect1->right > fRect2->left
			&& fRect1->left < fRect2->right
			&& fRect1->bottom > fRect2->top
			&& fRect1->top < fRect2->bottom)
		{
			isCollision = true;
			
			(fRect1->left > fRect2->left) ? (inter->left = fRect1->left) : (inter->left = fRect2->left);
			(fRect1->right < fRect2->right) ? (inter->right = fRect1->right) : (inter->right = fRect2->right);
			(fRect1->top > fRect2->top) ? (inter->top = fRect1->top) : (inter->top = fRect2->top);
			(fRect1->bottom < fRect2->bottom) ? (inter->bottom = fRect1->bottom) : (inter->bottom = fRect2->bottom);
		}
		else
		{
			isCollision = false;
			*inter = tagFloatRect();
		}

		return isCollision;
	}
	bool CheckRectIntersectRect(LPfRECT inter, LPRECT rect, LPfRECT fRect)
	{
		bool isCollision = false;

		if (rect->right > fRect->left
			&& rect->left < fRect->right
			&& rect->bottom > fRect->top
			&& rect->top < fRect->bottom)
		{
			isCollision = true;

			(rect->left > fRect->left) ? (inter->left = rect->left) : (inter->left = fRect->left);
			(rect->right < fRect->right) ? (inter->right = rect->right) : (inter->right = fRect->right);
			(rect->top > fRect->top) ? (inter->top = rect->top) : (inter->top = fRect->top);
			(rect->bottom < fRect->bottom) ? (inter->bottom = rect->bottom) : (inter->bottom = fRect->bottom);
		}
		else
		{
			isCollision = false;
			*inter = tagFloatRect();
		}

		return isCollision;
	}
	bool CheckRectIntersectRect(LPfRECT inter, LPfRECT fRect, LPRECT rect)
	{
		bool isCollision = false;

		if (fRect->right > rect->left
			&& fRect->left < rect->right
			&& fRect->bottom > rect->top
			&& fRect->top < rect->bottom)
		{
			isCollision = true;

			(fRect->left > rect->left) ? (inter->left = fRect->left) : (inter->left = rect->left);
			(fRect->right < rect->right) ? (inter->right = fRect->right) : (inter->right = rect->right);
			(fRect->top > rect->top) ? (inter->top = fRect->top) : (inter->top = rect->top);
			(fRect->bottom < rect->bottom) ? (inter->bottom = fRect->bottom) : (inter->bottom = rect->bottom);
		}
		else
		{
			isCollision = false;
			*inter = tagFloatRect();
		}

		return isCollision;
	}

	
	bool CheckRectContainRect(LPRECT rectBig, LPRECT rectSmall)
	{
		if (rectSmall->left >= rectBig->left
			&& rectSmall->right <= rectBig->right
			&& rectSmall->top >= rectBig->top
			&& rectSmall->bottom <= rectBig->bottom)
		{
			return true;
		}
		else return false;
	}
	bool CheckRectContainRect(LPfRECT fRectBig, LPfRECT fRectSmall)
	{
		if (fRectSmall->left >= fRectBig->left
			&& fRectSmall->right <= fRectBig->right
			&& fRectSmall->top >= fRectBig->top
			&& fRectSmall->bottom <= fRectBig->bottom)
		{
			return true;
		}
		else return false;
	}
	bool CheckRectContainRect(LPRECT rectBig, LPfRECT fRectSmall)
	{
		if (fRectSmall->left >= rectBig->left
			&& fRectSmall->right <= rectBig->right
			&& fRectSmall->top >= rectBig->top
			&& fRectSmall->bottom <= rectBig->bottom)
		{
			return true;
		}
		else return false;
	}
	bool CheckRectContainRect(LPfRECT fRectBig, LPRECT rectSmall)
	{
		if (rectSmall->left >= fRectBig->left
			&& rectSmall->right <= fRectBig->right
			&& rectSmall->top >= fRectBig->top
			&& rectSmall->bottom <= fRectBig->bottom)
		{
			return true;
		}
		else return false;
	}

	bool CheckRectContainPoint(LPRECT rect, LPPOINT pt)
	{
		if (pt->x >= rect->left && pt->x <= rect->right
			&& pt->y >= rect->top && pt->y <= rect->bottom)
		{
			return true;
		}
		else return false;
	}
	bool CheckRectContainPoint(LPRECT rect, LPfPOINT fpt)
	{
		if (fpt->x >= rect->left && fpt->x <= rect->right
			&& fpt->y >= rect->top && fpt->y <= rect->bottom)
		{
			return true;
		}
		else return false;
	}
	bool CheckRectContainPoint(LPfRECT fRect, LPPOINT pt)
	{
		if (pt->x >= fRect->left && pt->x <= fRect->right
			&& pt->y >= fRect->top && pt->y <= fRect->bottom)
		{
			return true;
		}
		else return false;
	}
	bool CheckRectContainPoint(LPfRECT fRect, LPfPOINT fpt)
	{
		if (fpt->x >= fRect->left && fpt->x <= fRect->right
			&& fpt->y >= fRect->top && fpt->y <= fRect->bottom)
		{
			return true;
		}
		else return false;
	}

	bool CheckIsoContainPoint(LPISOMETRIC iso, LPPOINT pt)
	{
		//x축 범위 밖
		if (pt->x < iso->leftX() || pt->x >= iso->rightX()) return false;

		//1~2사분면
		if (pt->y > iso->topY() && pt->y <= iso->centerY)
		{
			/*if ((pt->y - iso->centerY) >= -((iso->height) / (iso->width)) * abs(pt->x - iso->centerX) + iso->height / 2)
			{
				return true;
			}*/
			if ((pt->y - iso->centerY) >= ((iso->height) / (iso->width)) * abs(pt->x - iso->centerX) - iso->height / 2)
			{
				return true;
			}
				
			else return false;
		}
		//3~4사분면
		else if (pt->y > iso->centerY && pt->y < iso->bottomY())
		{
			/*if ((pt->y - iso->centerY) <= ((iso->height) / (iso->width)) * abs(pt->x - iso->centerX) - iso->height / 2)
			{
				return true;
			}*/
			if (-(pt->y - iso->centerY) > ((iso->height) / (iso->width)) * abs(pt->x - iso->centerX) - iso->height / 2)
			{
				return true;
			}
				
			else return false;
		}
		else return false;
	}

	//충돌 반응
	bool isCollisionReaction(const RECT& rcHold, RECT& rcMove)
	{
		RECT rcInter;

		if (!IntersectRect(&rcInter, &rcHold, &rcMove)) return false;

		int interW = rcInter.right - rcInter.left;
		int interH = rcInter.bottom - rcInter.top;

		if (interW > interH)
		{
			if (rcInter.top == rcHold.top)
			{
				OffsetRect(&rcMove, 0, -interH);
			}
			else if (rcInter.bottom == rcHold.bottom)
			{
				OffsetRect(&rcMove, 0, interH);
			}
		}
		else
		{
			if (rcInter.left == rcHold.left)
			{
				OffsetRect(&rcMove, -interW, 0);
			}
			else if (rcInter.right == rcHold.right)
			{
				OffsetRect(&rcMove, interW, 0);
			}
		}

		return true;
	}
	bool isCollisionReaction(const fRECT& mrcHold, fRECT& mrcMove)
	{
		RECT rcHold;
		rcHold.left = FLOAT_TO_INT(mrcHold.left);
		rcHold.top = FLOAT_TO_INT(mrcHold.top);
		rcHold.right = FLOAT_TO_INT(mrcHold.right);
		rcHold.bottom = FLOAT_TO_INT(mrcHold.bottom);

		RECT rcMove;
		rcMove.left = FLOAT_TO_INT(mrcMove.left);
		rcMove.top = FLOAT_TO_INT(mrcMove.top);
		rcMove.right = FLOAT_TO_INT(mrcMove.right);
		rcMove.bottom = FLOAT_TO_INT(mrcMove.bottom);

		RECT rcInter;
		if (!IntersectRect(&rcInter, &rcHold, &rcMove)) return false;

		int interW = rcInter.right - rcInter.left;
		int interH = rcInter.bottom - rcInter.top;

		if (interW > interH)
		{
			if (rcInter.top == rcHold.top)
			{
				OffsetRect(&rcMove, 0, -interH);
			}
			else if (rcInter.bottom == rcHold.bottom)
			{
				OffsetRect(&rcMove, 0, interH);
			}
		}
		else
		{
			if (rcInter.left == rcHold.left)
			{
				OffsetRect(&rcMove, -interW, 0);
			}
			else if (rcInter.right == rcHold.right)
			{
				OffsetRect(&rcMove, interW, 0);
			}
		}

		mrcMove.left = static_cast<float>(rcMove.left);
		mrcMove.top = static_cast<float>(rcMove.top);
		mrcMove.right = static_cast<float>(rcMove.right);
		mrcMove.bottom = static_cast<float>(rcMove.bottom);

		return true;
	}
	bool isCollisionReaction(const CIRCLE& cirHold, CIRCLE& cirMove)
	{
		float deltaX = cirMove.centerX - cirHold.centerX;
		float deltaY = cirMove.centerY - cirHold.centerY;
		float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

		float radius = cirHold.radius + cirMove.radius;

		if (distance < radius)
		{
			float angle = getAngle(cirHold.centerX, cirHold.centerY, cirMove.centerX, cirMove.centerY);

			float moveDelta = radius - distance;
			float moveX = cosf(angle) * moveDelta;
			float moveY = -sinf(angle) * moveDelta;

			cirMove.move(moveX, moveY);

			return true;
		}

		return false;
	}

}


