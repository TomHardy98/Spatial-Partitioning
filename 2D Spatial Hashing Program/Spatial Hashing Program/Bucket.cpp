/// @file Bucket.cpp
/// @brief Controls the buckets position, what it contains and how its drawn

#include <Windows.h>
#include <GLFW/glfw3.h>

#include "Bucket.h"

Bucket::Bucket()
{

}

Bucket::Bucket(int _x, int _y, int _w, int _h)
{
	m_positionX = _x;   // Set the x position of the bucket
	m_positionY = _y;   // Set the y position of the bucket
}

Bucket::~Bucket()
{

}

void Bucket::Draw()
{
	glPointSize( 2.0f );
	glBegin( GL_POINTS );   // Start drawing in points (pixels)
	glColor3f( 0.2f, 0.2f, 0.2f );   // Set the colour of the points drawn

	for ( int i = 0; i <= 10; ++i )   // Loop through buckets
	{
		glVertex2i( m_positionX + i, m_positionY );   // Draw along the buckets Y position
		glVertex2i( m_positionX, m_positionY + i );   // Draw along the buckets X position
	}

	glEnd();   // Stop drawing points
}

void Bucket::DeleteParticle( int _particleIndex )
{
	m_Particles.erase( m_Particles.begin() + _particleIndex );   // Erase the particle at the particles index
}

void Bucket::CheckCollisions()
{
	int offset = 3;

	if ( m_Particles.size() > 1 )   // If more than one particle is in a bucket
	{
		for ( int x = 0; x < m_Particles.size(); ++x )   // Loop through all the particles in the bucket
		{
			int positionCompareX = m_Particles[x]->GetX() + offset;   // Set the positions to compare
			int positionCompareY = m_Particles[x]->GetY() + offset;   // Set the positions to compare

			for ( int i = 0; i < m_Particles.size(); ++i )   // Loop through the particles again
			{
				if ( x == i )   // If its the compared one then break
				{
					continue;
				}

				if (m_Particles[i]->GetX() + offset == positionCompareX )   // If the compared one has the same X and Y as the other one
				{
					if (m_Particles[i]->GetY() + offset == positionCompareY )
					{
						m_Particles[i]->SetVelocity();   // Change the velocity
					}
				}
			}
		}
	}
}