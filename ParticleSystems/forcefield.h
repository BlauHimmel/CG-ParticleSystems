#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include "vectors.h"

class ForceField
{
public:
	ForceField()
	{

	}
	virtual ~ForceField()
	{

	}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class GravityForceField : public ForceField
{
public:
	GravityForceField(Vec3f gravity)
	{
		this->gravity = new Vec3f(gravity.x(), gravity.y(), gravity.z());
	}
	virtual ~GravityForceField()
	{
		delete[] gravity;
	}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const
	{
		Vec3f vec = Vec3f(gravity->x(), gravity->y(), gravity->z());
		vec = vec * (1.0f / mass);
		return vec;
	}
protected:
	Vec3f* gravity;
};

class ConstantForceField : public ForceField
{
public:
	ConstantForceField(Vec3f force)
	{
		this->force = new Vec3f(force.x(), force.y(), force.z());
	}
	virtual ~ConstantForceField()
	{
		delete[] force;
	}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const
	{
		Vec3f vec = Vec3f(force->x(), force->y(), force->z());
		vec = vec * (1.0f / mass);
		return vec;
	}
protected:
	Vec3f* force;
};

class  RadialForceField : public ForceField
{
public:
	RadialForceField(float magnitude)
	{
		this->magnitude = magnitude;
	}
	virtual ~RadialForceField()
	{

	}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const
	{
		Vec3f vec = Vec3f(position.x(), position.y(), position.z());
		vec = vec * -1.0f * magnitude;
		return vec;
	}
protected:
	float magnitude;
};

class  VerticalForceField : public ForceField
{
public:
	VerticalForceField(float magnitude)
	{
		this->magnitude = magnitude;
	}
	virtual ~VerticalForceField()
	{

	}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const
	{
		Vec3f vec = Vec3f(0, -position.y(), 0);
		vec = vec * -1.0f * magnitude;
		return vec;
	}
protected:
	float magnitude;
};

class  WindForceField : public ForceField
{
public:
	WindForceField(float magnitude)
	{
		this->magnitude = magnitude;
	}
	virtual ~WindForceField()
	{

	}
	virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const
	{
		Vec3f vec = Vec3f(PerlinNoise_2D(mass, t) /* mass*/ * 4.0f, 0.0f, 0.0f);;
		return vec;
	}
protected:
	float magnitude;
	float Noise1(int x, int y) const
	{
		int n = x + y * 57;
		n = (n << 13) ^ n;
		return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}
	float SmoothNoise_1(int x, int y) const
	{
		float corners = (Noise1(x - 1, y - 1) + Noise1(x + 1, y - 1) + Noise1(x - 1, y + 1) + Noise1(x + 1, y + 1)) / 16.0f;
		float sides = (Noise1(x - 1, y) + Noise1(x + 1, y) + Noise1(x, y - 1) + Noise1(x, y + 1)) / 8.0f;
		float center = Noise1(x, y) / 4.0f;
		return corners + sides + center;
	}
	float Cosine_Interpolate(float a, float b, float x) const
	{
		double ft = x * 3.1415927;
		double f = (1 - cos(ft)) * 0.5f;

		return  (float)(a*(1 - f) + b*f);

	}
	float InterpolatedNoise_1(float x, float y) const
	{
		int integer_X = (int)floor(x + 0.5);
		float fractional_X = x - integer_X;

		int integer_Y = (int)floor(y + 0.5);
		float fractional_Y = y - integer_Y;

		float v1 = SmoothNoise_1(integer_X, integer_Y);
		float v2 = SmoothNoise_1(integer_X + 1, integer_Y);
		float v3 = SmoothNoise_1(integer_X, integer_Y + 1);
		float v4 = SmoothNoise_1(integer_X + 1, integer_Y + 1);

		float i1 = Cosine_Interpolate(v1, v2, fractional_X);
		float i2 = Cosine_Interpolate(v3, v4, fractional_X);

		return Cosine_Interpolate(i1, i2, fractional_Y);
	}
	float PerlinNoise_2D(float x, float y) const
	{
		float persistence = 0.55f;
		int Number_Of_Octaves = 3;
		float total = 0.0f;
		float p = persistence;
		int n = Number_Of_Octaves - 1;
		int ii;

		for (ii = 0; ii <= n; ii++)
		{
			float frequency = (float)pow((float)2, ii);
			float amplitude = (float)pow(p, ii);

			total = total + InterpolatedNoise_1(x * frequency, y * frequency) * amplitude;
		}

		return total;
	}
};