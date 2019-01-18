/// \file Particle.h
/// \brief Controls all particles and sets their respective buckets and positions
/// \author Thomas Hardy

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef _PARTICLE_H
#define _PARTICLE_H

class Particle
{
public:

	Particle();

	Particle(int _x, int _y);   // Constructor

	~Particle();   // Destructor

	virtual void Draw();   // Draw function

	virtual void Update(float _deltaTime);   // Update function

	int m_particleID;   // Particle ID

	int const GetX() { return m_position.x; }   // Get X function
	int const GetY() { return m_position.y; }   // Get Y function

	int const GetBucket() { return m_bucket; }   // Get bucket function
	void SetBucket(int _value) { m_bucket = _value; }   // Set bucket function

	int const GetPrevBucket() { return m_prevBucket; }   // Get previous bucket function
	void SetPrevBucket(int _bucket) { m_prevBucket = _bucket; }   // Set the previous bucket function

	void SetVelocity();   // Set velocity function

protected:

	int m_bucket;   // Bucket

	int m_prevBucket;   // Previous bucket

	glm::vec2 m_position;   // Position.x and Position.y

	glm::vec2 m_velocity;   // Velocity.x and Velocity.y
};
#endif