/// @file Main.cpp
/// @brief Controls the entire programs functionality, calling all the functions and the main software loop

#include <GLFW/glfw3.h>   // Allows for the use of GLFW windows and drawing
#include <time.h>   // Allows for the use of srand
#include <iostream>   // Allows for console input and output
#include <string>   // Allows for passing strings
#include <vector>   // Allows for the use of vectors
#include <memory>   // Allows for sorting vectors in order
#include <algorithm> 
#include <functional>

#include "Hash.h"   // Including hash header file
#include "Bucket.h"   // Including bucket header file
#include "Particle.h"   // Including particle header file

void LoadBuckets( std::vector<Bucket*>& _Buckets, int const _gridHeight, int const _gridWidth );
void CreateParticles( std::vector<std::shared_ptr<Particle>> & _Particles, int _amountofParticles, int const _gridHeight, int const _gridWidth, std::vector<Bucket*>& _Buckets );
void AddParticles( std::vector<std::shared_ptr<Particle>> & _Particles, int _amountOfParticles, int _amountOfParticlesAdded, std::vector<Bucket*>& _Buckets );
void RemoveParticles( std::vector<std::shared_ptr<Particle>> & _Particles, int _amountOfParticles, int _amountOfParticlesRemoved, std::vector<Bucket*>& _Buckets );

int main()
{
	GLFWwindow* window;
	int _gridWidth = 100;
	int _gridHeight = 100;
	int _amountOfParticles = 200;
	int _amountOfParticlesAdded = 10;
	int _amountOfParticlesRemoved = 10;
	int frames = 0;
	int bucket = -1;
	double t0 = glfwGetTime();
	float currentFrame = 0;
	float lastFrame = 0;
	float deltaTime = 0;
	float fps = 0;

	srand( time( NULL ) );   // Intialising srand

	hashClass hashing;   // Intialising the hash class to call the hashing function

	std::vector<Bucket*> _Buckets;   // Create a vector of buckets
	std::vector<std::shared_ptr<Particle>> _Particles;   // Create a shared pointer of particles so they delete automatically

	if ( !glfwInit() )   // Initialize the library
	{
		return -1;
	}

	window = glfwCreateWindow( 640, 480, "2D Spatial Hashing Program", NULL, NULL );   // Create a windowed mode window and its OpenGL context

	if ( !window )
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent( window );   // Make the window's context current

	LoadBuckets( _Buckets, _gridHeight, _gridWidth );   // Call the load buckets function

	CreateParticles( _Particles, _amountOfParticles, _gridHeight, _gridWidth, _Buckets );   // Call the create particles function

	glOrtho( 0, 100, 100, 0, 1, -1 );   // Set the windows orthographic view

	while ( !glfwWindowShouldClose( window ) )   // Loop until the user closes the window
	{
		// Initialising delta time
		currentFrame = glfwGetTime();
		lastFrame = currentFrame;
		deltaTime = currentFrame - lastFrame;

		if ( glfwGetKey( window, GLFW_KEY_UP ) )   // If the up arrow has been pressed
		{
			if ( _Particles.size() < 10000 )   // If the particle size is less than 10000
			{
				_amountOfParticles += _amountOfParticlesAdded;   // Add to the amount of particles
				AddParticles( _Particles, _amountOfParticles, _amountOfParticlesAdded, _Buckets );   // Call the add particles function
			}
		}

		if ( glfwGetKey( window, GLFW_KEY_DOWN ) )   // If the down arrow has been pressed
		{
			if ( _Particles.size() > 10 )   // If the particle size is greater than 10
			{
				_amountOfParticles -= _amountOfParticlesRemoved;   // Remove from the amount of particles
				RemoveParticles(_Particles, _amountOfParticles, _amountOfParticlesRemoved, _Buckets);   // Call the remove particles function
			}
		}

		glClear( GL_COLOR_BUFFER_BIT );   // Rendering below here

		for ( int i = 0; i < _Buckets.size(); ++i )   // Loop through all the buckets
		{
			_Buckets[i]->Draw();   // Draw the buckets
			_Buckets[i]->CheckCollisions();   // Check the collisions inside the buckets
		}

		for ( int i = 0; i < _Particles.size(); ++i )   // Loop through all particles
		{
			_Particles[i]->Draw();   // Draw the particles
			_Particles[i]->Update(deltaTime);   // Update the particle positions using deltaTime
		}

		std::vector<int> indexToRemove;   // Create an int vector called indexToRemove

		for ( int i = 0; i < _Buckets.size(); ++i )   // Loop through all the buckets
		{
			for ( int x = 0; x < _Buckets[i]->GetParticles().size(); ++x )   // Loop through all the particles inside the bucket
			{
				std::shared_ptr<Particle> currentParticle = _Buckets[i]->GetParticles()[x];   // Set the current particle to each particle in the bucket

				bucket = hashing.HashFunction( currentParticle->GetX(), currentParticle->GetY() );   // Set the particles bucket to its current position

				if ( currentParticle->GetBucket() != bucket )   // If the current particles bucket is no longer the same bucket as it used to be
				{
					currentParticle->SetPrevBucket( currentParticle->GetBucket() );   // Set its previous bucket
					currentParticle->SetBucket( bucket );   // Set its new bucket

					indexToRemove.push_back(x);   // Push it into the index to remove vector

					_Buckets[bucket]->GetParticles().emplace_back( currentParticle );   // Push the particle into the new buckets vector
				}
			}

			std::sort( indexToRemove.begin(), indexToRemove.end(), std::greater<int>() );   // Reorder the indexToRemove vector to go from small to large

			for ( int y = 0; y < indexToRemove.size(); ++y )   // Loop through the indexToRemove vector
			{
				_Buckets[i]->DeleteParticle( indexToRemove[y] );   // Delete particle in the buckets vector
			}

			indexToRemove.clear();   // Clear the indexToRemove vector
		}

		// FPS Counter
		if ( ( currentFrame - t0 ) > 1.0 || frames == 0)
		{
			fps = ( double )frames / ( currentFrame - t0 );
			std::cout << "FPS: " << frames << std::endl;
			t0 = currentFrame;
			frames = 0;
		}

		frames++;

		glfwSwapBuffers( window );   // Swap front and back buffers

		glfwPollEvents();   // Poll for and process events
	}

	glfwTerminate();   // Terminate window

	return 0;
}

void LoadBuckets( std::vector<Bucket*>& _Buckets, int const _gridHeight, int const _gridWidth )
{
	for ( int y = 0; y < _gridHeight; y += 10 )   // Loop through the grid height 10 times
	{
		for ( int x = 0; x < _gridWidth; x += 10 )   // Loop through the grid width 10 times
		{
			Bucket* bucketLoaded = new Bucket();   // Create a new bucket called bucketLoaded

			bucketLoaded = new Bucket( x, y, 10, 10 );   // Create a new bucket at width and height of 10

			_Buckets.push_back( bucketLoaded );   // Push back the loaded bucket into the buckets vector
		}
	}

	for ( int i = 0; i < _Buckets.size(); ++i )   // Loop through the buckets vector
	{
		_Buckets[i]->m_bucketID = i;   // Give each bucket an ID from 0-99
	}
}

void CreateParticles( std::vector<std::shared_ptr<Particle>> & _Particles, int _amountOfParticles, int const _gridHeight, int const _gridWidth, std::vector<Bucket*>& _Buckets )
{
	int bucket;   // Create a local bucket
	hashClass hashing;   // Create a local hashClass for a hashing function

	for ( int i = 0; i < _amountOfParticles; ++i )   // Loop through the amount of particles
	{
		std::shared_ptr<Particle> particlesLoaded = std::make_shared<Particle>( rand()%100, rand()%100 );   // Create a new particle at a random position called particlesLoaded

		_Particles.emplace_back( particlesLoaded );   // Put the loaded particle into the particles vector

		for ( int i = 0; i < _Particles.size(); ++i )   // Loop through the particles vector
		{
			_Particles[i]->m_particleID = i;   // Give each particle an ID depending on where it is in the vector
		}

		for ( int x = 0; x < _Buckets.size(); ++x )   // Loop through the buckets vector
		{
			bucket = hashing.HashFunction( _Particles[i]->GetX(), _Particles[i]->GetY() );   // Assign a bucket using the particles x and y position 

			_Particles[i]->SetBucket( bucket );   // Set the particles bucket

			if ( _Particles[i]->GetBucket() == _Buckets[x]->m_bucketID )   // If the particles bucket is equal to one of the buckets
			{
				_Buckets[x]->GetParticles().emplace_back( particlesLoaded );   // Push that particle onto that buckets vector
			}
		}
	}
}

void AddParticles( std::vector<std::shared_ptr<Particle>> & _Particles, int _amountOfParticles, int _amountOfParticlesAdded, std::vector<Bucket*>& _Buckets )
{
	int bucket2;
	hashClass hashFunction;

	for ( int i = 0; i < _amountOfParticlesAdded; ++i )   // Loop through amount of particles added
	{
		std::shared_ptr<Particle> particlesLoaded = std::make_shared<Particle>( rand() % 100, rand() % 100 );   // Create a new particle with random x and y position

		_Particles.emplace_back( particlesLoaded );   // Put the new particle into the particles vector

		for ( int i = 0; i < _Particles.size(); ++i )   // Loop through the particles size
		{
			_Particles[i]->m_particleID = i;   // Assign the new particle ID's
		}

		for ( int x = 0; x < _Buckets.size(); ++x )   // Adding the particle to the bucket's particles contained vector at spawn (Doesn't remove and add to another bucket yet)
		{
			bucket2 = hashFunction.HashFunction( _Particles[i]->GetX(), _Particles[i]->GetY() );   // Assign a bucket to the new particles using x and y positions

			_Particles[i]->SetBucket( bucket2 );   // Set the particles bucket

			if ( _Particles[i]->GetBucket() == _Buckets[x]->m_bucketID )   // If the particles bucket matches one of the buckets
			{
				_Buckets[x]->GetParticles().emplace_back( particlesLoaded );   // Put the particle into the buckets vector
			}
		}
	}

	std::cout << "Amount of Particles: " << _amountOfParticles << std::endl;
}

void RemoveParticles( std::vector<std::shared_ptr<Particle>> & _Particles, int _amountOfParticles, int _amountOfParticlesRemoved, std::vector<Bucket*>& _Buckets )
{
	std::vector<int> indexToRemove;
	std::shared_ptr<Particle> currentParticle;
	int currentBucket;

	for ( int t = 0; t < _amountOfParticlesRemoved; ++t )   // Loop through the particles removed size
	{
		currentBucket = _Particles[t]->GetBucket();   // Get the current bucket of the current particle

		for ( int x = 0; x < _Buckets[currentBucket]->GetParticles().size(); ++x )   // Loop through the amount of particles in that particles bucket
		{
			currentParticle = _Buckets[currentBucket]->GetParticles()[x];   // Set the current particle 

			indexToRemove.push_back(x);   // Push back the current particle to the remove vector
		}

		std::sort( indexToRemove.begin(), indexToRemove.end(), std::greater<int>() );   // Order the index to remove vector from small to big

		for ( int y = 0; y < indexToRemove.size(); ++y )   // Loop through the index to remove vector
		{
			_Buckets[currentBucket]->DeleteParticle( indexToRemove[y] );   // Delete the particle from the buckets vector
		}

		indexToRemove.clear();   // Clear the index to remove vector

		_Particles.pop_back();   // Pop the back end of the particles vector off
	}

	std::cout << "Amount of Particles: " << _amountOfParticles << std::endl;
}