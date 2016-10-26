#pragma once
#include "vectors.h"
#include "particle.h"
#include "random.h"
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>

class Generator
{
public:
	Generator()
	{
		color = new Vec3f();
		dead_color = new Vec3f();
		color_randomness = 0;
		mass = 0;
		mass_randomness = 0;
		lifespan = 0;
		lifespan_randomness = 0;
	    desired_num_particles = 0;
		this->random = new Random(100);
	}
	virtual ~Generator()
	{
		delete color;
		delete dead_color;
		delete random;
	}
	// 初始化
	void SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
	{
		this->color->Set(color.x(), color.y(), color.z());
		this->dead_color->Set(dead_color.x(), dead_color.y(), dead_color.z());
		this->color_randomness = color_randomness;
	}
	void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
	{
		this->lifespan = lifespan;
		this->lifespan_randomness = lifespan_randomness;
		this->desired_num_particles = desired_num_particles;
	}
	void SetMass(float mass, float mass_randomness)
	{
		this->mass = mass;
		this->mass_randomness = mass_randomness;
	}
	// 每个时间步，产生一些质点
	virtual int numNewParticles(float current_time, float dt) const = 0;
	virtual Particle* Generate(float current_time, int i) = 0;
	// for the gui
	virtual void Paint() const = 0;
	virtual void Restart()
	{
		delete this->random;
		this->random = new Random(100);
	}
protected:
	Vec3f* color;
	Vec3f* dead_color;
	float color_randomness;

	float mass;
	float mass_randomness;

	float lifespan;
	float lifespan_randomness;

	float desired_num_particles;

	Random* random;
};

class HoseGenerator : public Generator
{
public:
	HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness)
	{
		this->position = new Vec3f(position.x(), position.y(), position.z());
		this->velocity = new Vec3f(velocity.x(), velocity.y(), velocity.z());
		this->position_randomness = position_randomness;
		this->velocity_randomness = velocity_randomness;
	}
	virtual ~HoseGenerator()
	{
		delete position;
		delete velocity;
	}
	virtual int numNewParticles(float current_time, float dt) const
	{
		float num = dt * desired_num_particles / lifespan;		
		return num;
	}
	virtual Particle* Generate(float current_time, int i)
	{
		Vec3f position_random = *position + Vec3f(0.707f * (2 * random->next() - 1.0f) *  position_randomness, 0.707f * (2 * random->next() - 1.0f) *  position_randomness, 0.707f * (2 * random->next() - 1.0f) *  velocity_randomness);
		Vec3f velocity_random = *velocity + Vec3f(0.707f * (2 * random->next() - 1.0f) *  velocity_randomness, 0.707f * (2 * random->next() - 1.0f) *  velocity_randomness, 0.707f * (2 * random->next() - 1.0f) *  velocity_randomness);
		float mass_random = mass + (2 * random->next() - 1.0f) * mass_randomness;
		float lifespan_random = lifespan + (2 * random->next() - 1.0f) * lifespan_randomness;
		Particle* particles = new Particle(position_random, velocity_random, *color, *dead_color, mass_random, lifespan_random);
		return particles;
	}
	virtual void Paint() const
	{

	}
protected:
	Vec3f* position;
	float position_randomness;

	Vec3f* velocity;
	float velocity_randomness;
};

class  RingGenerator : public Generator
{
public:
	RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness)
	{
		this->velocity = new Vec3f(velocity.x(), velocity.y(), velocity.z());
		this->position_randomness = position_randomness;
		this->velocity_randomness = velocity_randomness;
	}
	virtual ~RingGenerator()
	{
		delete velocity;
	}
	virtual int numNewParticles(float current_time, float dt) const
	{
		float num = dt * desired_num_particles / lifespan + current_time;
		return num;
	}
	virtual Particle* Generate(float current_time, int i)
	{
		float radius = 3.5f + (1.0f * random->next() - 0.5f);
		float theta = random->next() * 2.0f * 3.14159f;
		Vec3f position_random = Vec3f(radius * sinf(theta), 0.0f, radius * cosf(theta));
		Vec3f velocity_random = *velocity + Vec3f(0.707f * (2 * random->next() - 1.0f) *  velocity_randomness, 0.707f * (2 * random->next() - 1.0f) *  velocity_randomness, 0.0f);
		float mass_random = mass + (2 * random->next() - 1.0f) * mass_randomness;
		float lifespan_random = lifespan + (2 * random->next() - 1.0f) * lifespan_randomness;
		Particle* particles = new Particle(position_random, velocity_random, *color, *dead_color, mass_random, lifespan_random);
		return particles;
	}
	virtual void Paint() const
	{
		glBegin(GL_QUADS);
		glVertex3f(-5.0, 0.0, 5.0);
		glVertex3f(5.0, 0.0, 5.0);
		glVertex3f(5.0, 0.0, -5.0);
		glVertex3f(-5.0, 0.0, -5.0);
		glEnd();
	}
	virtual void Restart()
	{

	}
protected:
	float position_randomness;

	Vec3f* velocity;
	float velocity_randomness;
};
