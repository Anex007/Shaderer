#pragma once

struct Vec2f
{
	Vec2f(float _v)
		:x(_v), y(_v)
	{}

	Vec2f(float _x, float  _y)
		: x(_x), y(_y)
	{}

	float x, y;
};

struct Vec3f
{
	Vec3f(float _v)
		: x(_v), y(_v), z(_v)
	{}

	Vec3f(float _x, float  _y, float _z)
		: x(_x), y(_y), z(_z)
	{}

	float x, y, z;
};

struct Vec4f
{
	Vec4f(float _v)
		: x(_v), y(_v), z(_v), w(_v)
	{}

	Vec4f(float _x, float  _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w)
	{}

	float x, y, z, w;
};