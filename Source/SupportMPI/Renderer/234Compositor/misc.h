/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   misc.h
// @brief  Miscelaneous routines for 234compositor
// @author Jorji Nonaka (jorji@riken.jp)

#pragma once
#include "typedef.h"

// ======================================
//   Function Prototypes
// ======================================
float clamp_float( const float, const float, const float ); // Clamp float values 
BYTE  clamp_BYTE ( const BYTE,  const BYTE, const BYTE ); // Clamp BYTE values 

BYTE  saturate_add( BYTE, BYTE ); // Saturate Addition of BYTE values

bool check_pow2 ( unsigned int ); // Check whether or not a value (number of nodes ) is a power-of-two or not.
unsigned int get_nearest_pow2 (unsigned int ); // Get nearest power-of-two from an input value

unsigned int bitrevorder ( unsigned int, unsigned int ); // Returns the input data in bit-reversed order 

// Memory Allocation
BYTE  *allocate_byte_memory_region ( unsigned int );  // BYTE data
float *allocate_float_memory_region ( unsigned int ); // float data
unsigned int  *allocate_int_memory_region ( unsigned int ); // unsigned int data
