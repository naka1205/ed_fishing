#ifndef __SDL_Point_H__
#define __SDL_Point_H__

#include <cmath>
#include <string>

#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "SG_Engine/SEPlatformMarcos.h"

NS_SDL_BEGIN
class Point
{
public:
	float x;
	float y;

public:
	static const Point ZERO;

public:
	Point(float x = 0.f, float y = 0.f) : x(x), y(y) {}
	Point(const Point &point);
	Point(const SDL_Point &point);

	~Point() {}

	float getX() const;
	void setX(float x);
	float getY() const;
	void setY(float y);

	void set(float x, float y);

	float length() const
	{
		return sqrt(x * x + y * y);
	}

	void normalize()
	{
		float l = length();
		if (l > 0)
		{
			(*this) *= 1 / l;
		}
	}

	Point operator+(const Point &v2) const
	{
		return Point(x + v2.x, y + v2.y);
	}

	Point operator*(float scalar)
	{
		return Point(x * scalar, y * scalar);
	}

	Point &operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Point operator-(const Point &v2) const
	{
		return Point(x - v2.x, y - v2.y);
	}

	Point operator/(float scalar)
	{
		return Point(x / scalar, y / scalar);
	}

	Point &operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	friend Point &operator+=(Point &v1, const Point &v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;

		return v1;
	}

	friend Point &operator-=(Point &v1, const Point &v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;

		return v1;
	}

	friend bool operator!=(const Point &p1, const Point &p2)
	{
		return !p1.equals(p2);
	}

	friend bool operator==(const Point &p1, const Point &p2)
	{
		return p1.equals(p2);
	}

	inline Point rotate(const Point &other) const
	{
		return Point(x * other.x - y * other.y, x * other.y + y * other.x);
	}

	Point rotateByAngle(const Point &pivot, float angle) const;

	bool equals(const Point &point) const;

	inline Point lerp(const Point &other, float alpha) const
	{
		return Point(x * (1.f - alpha) + other.x * alpha, y * (1.f - alpha) + other.y * alpha);
	}

	static inline Point forAngle(const float a)
	{
		return Point(cosf(a), sinf(a));
	}

	static bool isLineIntersect(const Point &A, const Point &B, const Point &C, const Point &D, float *S = nullptr, float *T = nullptr);

	SDL_Point getSDL_Point() const;
};
NS_SDL_END
#endif
