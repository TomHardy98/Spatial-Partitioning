/// @file Hash.cpp
/// @brief Controls the hash function that determines what bucket a particle belongs in

#include <iostream>
#include <string>
#include <vector>

#include "Hash.h"

hashClass::hashClass()
{
	
}

int hashClass::HashFunction( int _x, int _y )
{
	int bucket;
	int xGrid = _x / m_rows;   // How big a bucket is (10x10)
	int yGrid = _y / m_rows;

	int hash = yGrid * m_rows + xGrid;   // Hash function

	bucket = hash;   // Dividing by size of the table and using remainder as index

	return bucket;
}