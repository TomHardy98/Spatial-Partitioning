/// \file Bucket.h
/// \brief Controls all the buckets what they contain
/// \author Thomas Hardy

#pragma once
#include <vector>
#include <memory>
#include <iostream>

#include "Particle.h"

#ifndef _BUCKET_H
#define _BUCKET_H

class Bucket
{
public:

	Bucket();

	Bucket(int _x, int _y, int _w, int _h);   // Constructor

	~Bucket();   // Destructor

	virtual void Draw();   // Draw function

	int m_bucketID;   // Bucket ID

	int const GetX() { return m_positionX; }   // Get X value function
	int const GetY() { return m_positionY; }   // Get Y value function

	void DeleteParticle(int _particleIndex);   // Delete particle function

	void CheckCollisions();   // Check collisions function

	std::vector <std::shared_ptr<Particle>>& const GetParticles() { return m_Particles; }   // Get particles function

protected:

	std::vector <std::shared_ptr<Particle>> m_Particles;   // Particles inside bucket

	int m_positionX;   // Buckets position X
	int m_positionY;   // Buckets position Y
};
#endif