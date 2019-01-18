/// @file Particle.cpp
/// @brief Controls each individual particle, drawing it, assigning its velocity and position

#include <Windows.h>
#include <GLFW/glfw3.h>

#include "Particle.h"

Particle::Particle()
{

}

Particle::Particle(int _x, int _y)
{
	m_position.x = _x;   // Set particles X position
	m_position.y = _y;   // Set particles Y position

	float lowestVel = -0.4f;   // Initialise lowest velocity value
	float highestVel = 0.4f;   // Initialise highest velocity value
	float randFloat = lowestVel + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highestVel - lowestVel)));   // Get a random value between lowest and highest velocity
	float randFloat2 = lowestVel + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highestVel - lowestVel)));

	m_velocity = glm::vec2(randFloat, randFloat2);   // Set velocity to 2 random values

	m_prevBucket = 0;   // Set previous bucket to 0
}

Particle::~Particle()
{
	
}

void Particle::Update(float _deltaTime)
{
	if ( m_position.x + m_velocity.x >= 100 )   // If the particle is out of bounds
	{
		m_velocity.x *= -1.0f;   // Reverse velocity
	}
	else if ( m_position.x + m_velocity.x <= 0 )   // If the particle is out of bounds
	{
		m_velocity.x *= -1.0f;   // Reverse velocity
	}

	if ( m_position.y + m_velocity.y >= 100 )   // If the particle is out of bounds
	{
		m_velocity.y *= -1.0f;   // Reverse velocity
	}
	else if ( m_position.y + m_velocity.y <= 0 )   // If the particle is out of bounds
	{
		m_velocity.y *= -1.0f;   // Reverse velocity
	}

	( m_position += m_velocity ) *_deltaTime;   // Move pixel by position, velocity and deltaTime
}

void Particle::Draw()
{
	glPointSize( 3.0f );   // Set point size to 3x3 pixels
	glBegin( GL_POINTS );   // Start drawing points

	glColor3f( 1, 1, 0 );   // Set particles colour to green

    glVertex2i( m_position.x, m_position.y );   // Set the pixel draw position to the particles position
	glEnd();   // Stop drawing points
}

void Particle::SetVelocity()
{
	m_velocity.y *= -1.0f;   // Reverse velocity in Y axis
	m_velocity.x *= -1.0f;   // Reverse velocity in X axis
}