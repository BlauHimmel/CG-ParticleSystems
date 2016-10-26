#pragma once
#include "particle.h"
#include "forcefield.h"

class Integrator
{
public:
	Integrator()
	{

	}
	virtual ~Integrator()
	{

	}
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() = 0;
};

class EulerIntegrator : public Integrator
{
public:
	EulerIntegrator()
	{

	}
	virtual ~EulerIntegrator()
	{

	}
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt)
	{
		Vec3f position_current = particle->getPosition();
		Vec3f velocity_current = particle->getVelocity();
		Vec3f position_next = position_current + velocity_current * dt;
		Vec3f velocity_next = velocity_current + forcefield->getAcceleration(position_current, particle->getMass(), t) * dt;
		particle->increaseAge(dt);
		particle->setPosition(position_next);
		particle->setVelocity(velocity_next);
	}
	virtual Vec3f getColor()
	{
		Vec3f vec = Vec3f(0.8f, 0.8f, 0.3f);
		return vec;
	}
};

class MidpointIntegrator : public Integrator
{
public:
	MidpointIntegrator()
	{

	}
	virtual ~MidpointIntegrator()
	{

	}
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt)
	{
		Vec3f position_current = particle->getPosition();
		Vec3f velocity_current = particle->getVelocity();
		Vec3f position_mid = position_current + velocity_current * (dt * 0.5f);
		Vec3f velocity_mid = velocity_current + forcefield->getAcceleration(position_current, particle->getMass(), t) * (dt * 0.5f);
		Vec3f position_next = position_current + velocity_mid * dt;
		Vec3f velocity_next = velocity_current + forcefield->getAcceleration(position_mid, particle->getMass(), t + 0.5f * dt) * dt;
		particle->increaseAge(dt);
		particle->setPosition(position_next);
		particle->setVelocity(velocity_next);
	}
	virtual Vec3f getColor()
	{
		Vec3f vec = Vec3f(0.3f, 0.8f, 0.8f);
		return vec;
	}
};

class RungeKuttaIntegrator : public Integrator
{
public:
	RungeKuttaIntegrator()
	{

	}
	virtual ~RungeKuttaIntegrator()
	{

	}
	virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt)
	{
		Vec3f position_current = particle->getPosition();
		Vec3f velocity_current = particle->getVelocity();
		Vec3f position_mid = position_current + velocity_current * (dt * 0.5f);
		Vec3f velocity_mid = velocity_current + forcefield->getAcceleration(position_current, particle->getMass(), t) * (dt * 0.5f);
		Vec3f position_next = position_current + velocity_mid * dt;
		Vec3f velocity_next = velocity_current + forcefield->getAcceleration(position_mid, particle->getMass(), t + 0.5f * dt) * dt;
		particle->increaseAge(dt);
		particle->setPosition(position_next);
		particle->setVelocity(velocity_next);
	}
	virtual Vec3f getColor()
	{
		Vec3f vec = Vec3f(0.8f, 0.3f, 0.8f);
		return vec;
	}
};
