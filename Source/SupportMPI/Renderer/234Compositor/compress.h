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

/*************************************************************************
* RLE_Compress() - Compress a block of data using an RLE coder.
*  in     - Input (uncompressed) buffer.
*  out    - Output (compressed) buffer. This buffer must be 0.4% larger
*           than the input buffer, plus one byte.
*  insize - Number of input bytes.
* The function returns the size of the compressed data.
*************************************************************************/
int RLE_Compress( BYTE*, BYTE*, unsigned int );

/*************************************************************************
* RLE_Uncompress() - Uncompress a block of data using an RLE decoder.
*  in      - Input (compressed) buffer.
*  out     - Output (uncompressed) buffer. This buffer must be large
*            enough to hold the uncompressed data.
*  insize  - Number of input bytes.
*  The function returns the size of the deflated data.
*************************************************************************/
int RLE_Uncompress( BYTE*, BYTE*, unsigned int );
