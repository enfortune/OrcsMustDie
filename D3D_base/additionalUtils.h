#pragma once

//define
#ifndef PI
#define PI 3.141592654f
#endif
#ifndef PI2
#define PI2 PI * 2
#endif
#ifndef DEGREE
#define DEGREE *PI/180  // �ﰢ�Լ� ��� �� ���� �ڿ� �ٿ��� ���. ex) 180 DEGREE ��� �ϸ� PI�� ��.
#endif
#ifndef BOOL
typedef int BOOL;
#endif


//��� �����
namespace THETA_UTIL
{
	//enum
	typedef enum class DIRECTION_8
	{
		L,	// left
		R,	// right
		T,	// top
		B,	// bottom
		LT,  // left top
		RT,	// right top
		LB,	// left bottom
		RB,	// right bottom
		END
	}DI8; // 8����
	typedef enum class DIRECTION_4
	{
		LEFT,	// left
		RIGHT,	// right
		TOP,		// top
		BOTTOM,	// bottom
		END
	}DI4; // 4����

	//func

	//struct
	typedef struct tagFloatPoint // float�� POINT
	{
		float x, y;
		tagFloatPoint(float x = 0.0f, float y = 0.0f)
			: x(x), y(y)
		{}
		void set(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		void move(float moveX, float moveY)
		{
			x += moveX;
			y += moveY;
		}
		POINT toPoint()
		{
			POINT temp;
			temp = { (int)x, (int)y };
			return temp;
		}
	}FloatPoint, fPOINT, *LPFloatPoint, *LPfPOINT;
	typedef struct tagFloatRect // float�� RECT
	{
		float left, top, right, bottom;
		tagFloatRect(float left = 0.0f, float top = 0.0f, float right = 0.0f, float bottom = 0.0f)
			:left(left), top(top), right(right), bottom(bottom)
		{}
		void set(float left, float top, float right, float bottom)
		{
			this->left		=	left;
			this->top		=	top;
			this->right		=	right;
			this->bottom	=	bottom;
		}
		void move(float moveX, float moveY)
		{
			this->left		+=	moveX;
			this->top		+=	moveY;
			this->right		+=	moveX;
			this->bottom	+=	moveY;
		}
		void setCenter(float centerX, float centerY)
		{
			float width		=	right - left;
			float height	=	bottom - top;
			
			this->left		=	centerX - width / 2;
			this->top		=	centerY - height / 2;
			this->right		=	centerX + width / 2;
			this->bottom	=	centerY + height / 2;
		}
		void draw(HDC hdc, float cameraX = 0.0f, float cameraY = 0.0f)
		{
			Rectangle(hdc, 
				(int)left - (int)cameraX, (int)top - (int)cameraY,
				(int)right - (int)cameraX, (int)bottom - (int)cameraY);
		}
		RECT toRect()
		{
			RECT temp;
			temp = { (int)left, (int)top, (int)right, (int)bottom };
			return temp;
		}
		float width()
		{
			return right - left;
		}
		float height()
		{
			return bottom - top;
		}
		fPOINT center()
		{
			fPOINT temp;
			temp.x = (left+right)/2;
			temp.y = (top+bottom)/2;
			return temp;
		}
	}FloatRect, fRECT, *LPFloatRect, *LPfRECT;
	typedef struct tagCircle
	{
		float centerX, centerY, radius;
		tagCircle(float centerX = 0.0f, float centerY = 0.0f, float radius = 0.0f)
			:centerX(centerX), centerY(centerY), radius(radius)
		{}
		void set(float centerX, float centerY, float radius)
		{
			this->centerX = centerX;
			this->centerY = centerY;
			this->radius = radius;
		}
		void setCenter(float centerX, float centerY)
		{
			this->centerX = centerX;
			this->centerY = centerY;
		}
		void draw(HDC hdc, float cameraX = 0.0f, float cameraY = 0.0f)
		{
			Ellipse(hdc, 
				static_cast<int>(centerX - radius - cameraX), static_cast<int>(centerY - radius - cameraY),
				static_cast<int>(centerX + radius - cameraX), static_cast<int>(centerY + radius - cameraY));
		}
		void move(float moveX, float moveY)
		{
			centerX += moveX;
			centerY += moveY;
		}
	}CIRCLE, *LPCIRCLE;
	typedef struct tagRhombus
	{
		float centerX, centerY, width, height;
		tagRhombus(float centerX = 0, float centerY = 0, float width = 0, float height = 0)
			: centerX(centerX), centerY(centerY), width(width), height(height)
		{}
		void set(float centerX, float centerY, float width, float height)
		{
			this->centerX = centerX;
			this->centerY = centerY;
			this->width = width;
			this->height = height;
		}
		void setCenter(float centerX, float centerY)
		{
			this->centerX = centerX;
			this->centerY = centerY;
		}
		void setSize(float width, float height)
		{
			this->width = width;
			this->height = height;
		}
		float leftX() { return centerX - width / 2; }
		float topY() { return centerY - height / 2; }
		float rightX() { return centerX + width / 2; }
		float bottomY() { return centerY + height / 2; }
		void draw(HDC hdc, float cameraX = 0.0f, float cameraY = 0.0f)
		{
			POINT pt[4];
			pt[0] = PointMake(leftX() - cameraX, centerY - cameraY);
			pt[1] = PointMake(centerX - cameraX, topY() - cameraY);
			pt[2] = PointMake(rightX() - cameraX, centerY - cameraY);
			pt[3] = PointMake(centerX - cameraX, bottomY() - cameraY);
			Polygon(hdc, pt, 4);
		}
		void drawLine(HDC hdc, float cameraX = 0.0f, float cameraY = 0.0f)
		{
			POINT pt[4];
			pt[0] = PointMake(leftX()	 - cameraX, centerY		- cameraY);
			pt[1] = PointMake(centerX	 - cameraX, topY()		- cameraY);
			pt[2] = PointMake(rightX()	 - cameraX, centerY		- cameraY);
			pt[3] = PointMake(centerX	 - cameraX, bottomY()	- cameraY);
			LineMake(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
			LineMake(hdc, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
			LineMake(hdc, pt[2].x, pt[2].y, pt[3].x, pt[3].y);
			LineMake(hdc, pt[3].x, pt[3].y, pt[0].x, pt[0].y);
		}
	}RHOMBUS, tagIsometric, ISOMETRIC, *LPRHOMBUS, *LPISOMETRIC;

	/*Ű�Ŵ����� isOnceKeyDown�Լ� ���Ͽ�*/
	typedef struct tagKeyChecker
	{
		bool isReady; // Ű�� �غ�Ǿ�����
		bool isAction; // Ű�� �����Ϳ� ���� �׼��� �����ؾ� �ϴ���

		tagKeyChecker() :isReady(true), isAction(false)
		{}

		void ready() { isReady = true; isAction = false; }

	}KEYCHECKER, *LPKEYCHECKER;

	//[fRECT, fPOINT] struct func
	fPOINT PtTofPt(POINT pt);
	POINT fPtToPt(fPOINT fpt);
	fPOINT fPointMake(float x, float y);
	fRECT RectTofRect(RECT rc);
	RECT fRectToRect(fRECT frc);
	fRECT fRectMake(float left, float top, float width, float height);
	fRECT fRectMakeCenter(float centerX, float centerY, float width, float height);
	//[RECT] struct func
	LONG GetRectWidth(RECT rc);
	LONG GetRectHeight(RECT rc);
	POINT GetRectCenter(RECT rc);
	float GetRectWidth(fRECT frc);
	float GetRectHeight(fRECT frc);
	fPOINT GetRectCenter(fRECT frc);
	//draw
	void RectangleMakeRect(HDC hdc, RECT rc);	// RECT����ü �޾Ƽ� �簢�� �׷���
	void EllipseMakeRect(HDC hdc, RECT rc);		// RECT����ü �޾Ƽ� �� �׷���
	void LineMakeRect(HDC hdc, RECT rc);		// RECT����ü �޾Ƽ� �簢��(��) �׷���
	void LineMakeRect(HDC hdc, int left, int top, int right, int bottom); // �簢��(��) �׷���
	void RapidStretchBlt(HDC destDC, int destX, int destY, int destW, int destH,
		HDC sourDC, int sourX, int sourY, int sourW, int sourH); // ���� StretchBlt
	void ExactStretchBlt(HDC destDC, int destX, int destY, int destW, int destH,
		HDC sourDC, int sourX, int sourY, int sourW, int sourH); // ������ StretchBlt
	void ExtendBlt(HDC destDC, int destX, int destY, int destW, int destH,
		HDC sourDC, int sourX, int sourY, int sourW, int sourH,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255)); // ������ �� �ִ� TransparentBlt
	//degree
	float DegToRad(float degree); // degree�� radian���� ��ȯ����
	float RadToDeg(float radian); // radian�� degree�� ��ȯ����
	float diToAngle(DIRECTION_8 di); // ġ���� 0<= ~ <2PI
	float diToAngle(DIRECTION_4 di); // ġ���� 0<= ~ <2PI
	int SetRotateImageFrame(float startAngle, int nFrame, bool isClockWise, float currentAngle); // �ޱ��� ���������� ��ȯ

	//intersect & contain
	bool CheckRectIntersectRect(LPfRECT inter,	LPRECT rect1	, LPRECT rect2	);
	bool CheckRectIntersectRect(LPfRECT inter,	LPfRECT fRect1	, LPfRECT fRect2);
	bool CheckRectIntersectRect(LPfRECT inter,	LPRECT rect		, LPfRECT fRect	);
	bool CheckRectIntersectRect(LPfRECT inter,	LPfRECT fRect	, LPRECT rect	);

	bool CheckRectContainRect(LPRECT rectBig	, LPRECT rectSmall	);
	bool CheckRectContainRect(LPfRECT fRectBig	, LPfRECT fRectSmall);
	bool CheckRectContainRect(LPRECT rectBig	, LPfRECT fRectSmall);
	bool CheckRectContainRect(LPfRECT fRectBig	, LPRECT rectSmall);

	bool CheckRectContainPoint(LPRECT rect		, LPPOINT pt	);
	bool CheckRectContainPoint(LPRECT rect		, LPfPOINT fpt	);
	bool CheckRectContainPoint(LPfRECT fRect	, LPPOINT pt	);
	bool CheckRectContainPoint(LPfRECT fRect	, LPfPOINT fpt	);

	bool CheckIsoContainPoint(LPISOMETRIC iso	, LPPOINT pt	);

	//collision reaction
	bool isCollisionReaction(const RECT& rcHold, RECT& rcMove);
	bool isCollisionReaction(const fRECT& mrcHold, fRECT& mrcMove);
	bool isCollisionReaction(const CIRCLE& cirHold, CIRCLE& cirMove);
}