#pragma once

#ifndef PI
#define PI 3.141592654f
#endif

#ifndef PI2
#define PI2 PI * 2
#endif

#define FLOAT_EPSILON 0.001f
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)
#define FLOAT_EQUAL(f1, f2) (fabs(f1, f2) <= FLOAT_EPSILON)

//일주일의 고생은 이것을 위함이여찌
namespace THETA_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY);

	float getAngle(float x1, float y1, float x2, float y2);

}