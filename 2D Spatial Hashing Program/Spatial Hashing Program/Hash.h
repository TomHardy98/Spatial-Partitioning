/// \file Hash.h
/// \brief Controls the has function to determine particles bucket
/// \author Thomas Hardy

#pragma once
#include <iostream>
#include <string>

#include "Bucket.h"

#ifndef _HASH_H
#define _HASH_H

class hashClass
{
public:

	hashClass();

	int HashFunction(int _x, int _y);   // Hash function

private:

	static const int m_sizeofTable = 100;   // Constant int for the table size
	static const int m_rows = 10;   // Rows in the table
};
#endif