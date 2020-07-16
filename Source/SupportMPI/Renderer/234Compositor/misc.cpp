/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   misc.c
// @brief  Miscelaneous routines for 234compositor
// @author Jorji Nonaka (jorji@riken.jp)


#include "misc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*========================================================*/
/**
 *  @brief Clamp a floating value using the min-max range
 *
 *  @param  value, min value, max value
 */
/*========================================================*/
float clamp_float( const float val, const float min_f, const float max_f )
{
    return ( val > max_f ? max_f : val < min_f ? min_f : val );
}

/*========================================================*/
/**
 *  @brief Clamp a BYTE value using the min-max range
 *
 *  @param  value, min value, max value
 */
/*========================================================*/
BYTE clamp_BYTE( const BYTE val, const BYTE min_f, const BYTE max_f )
{
    return ( val > max_f ? max_f : val < min_f ? min_f : val );
}

/*========================================================*/
/**
 *  @brief Saturate addition of BYTE values
 *
 *  @param  value_a, value_b
 *  @return  value_a + value_b < 255 (Max Value)
 */
/*========================================================*/
BYTE saturate_add( BYTE val_a, BYTE val_b )
{ 
    return ( val_a > 0xFF - val_b ) ? 0xFF : val_a + val_b;
}

/*========================================================*/
/**
 *  @brief Check whether the number of nodes is a 
 *         power-of-two or not.
 *         Based on Kentaro Oku's "LSVCompTest" code 
 *
 *  @param  nnodes [in] Numer of Nodes
 *  @return true or false
 **/
/*========================================================*/
//_Bool check_pow2 ( unsigned int nnodes )
bool check_pow2 ( unsigned int nnodes )
{
    if ( nnodes == 0 )
    	return false; // ZERO is not a power-of-two
  
    if( !( nnodes & ( nnodes - 1)) ) 
    {
        return true; // Power of 2
    }
    else
    {
        return false;
    }
}

/*========================================================*/
/**
 *  @brief Return the nearest power-of-two smaller than
 *         the input value 
 *         Based on Kentaro Oku's LSVCompTest code 
 *
 *  @param  number of nodes
 */
/*========================================================*/
unsigned int get_nearest_pow2 (unsigned int nnodes )
{
    int pow2 = 1;

    while ( pow2 < nnodes )
    {
        pow2 <<= 1;
    }
  
    return ( pow2 >> 1 );
}

/*========================================================*/
/**
 *  @brief Return the Rank in Bit-reversed order
 *	       Derived from Van der Corput Sequence Generator 
 *         Code at Rosetta Code (rosettacode.org) 
 *        
 *  @param  rank   [in] Rank 
 *  @param  nnodes [in] Number of Nodes 
 *  @return Rank in bit reversed order
 */
/*========================================================*/
unsigned int bitrevorder ( unsigned int rank, unsigned int nnodes )
{
    unsigned int bitrev_num, bitrev_den;
    unsigned int bitrev_p, bitrev_q;
    unsigned int bitrev_my_rank;

    // =======================================  
    bitrev_my_rank = rank;
    bitrev_p = 0;
    bitrev_q = 1;

    while ( bitrev_my_rank ) {
    	bitrev_p = bitrev_p * 2 + ( bitrev_my_rank % 2 );
       	bitrev_q *= 2;
        bitrev_my_rank *= 0.5; // DIV 2
    }
	 
    bitrev_num = bitrev_p;
    bitrev_den = bitrev_q;

    while ( bitrev_p ) { 
        bitrev_my_rank = bitrev_p; 
        bitrev_p = bitrev_q % bitrev_p; 
        bitrev_q = bitrev_my_rank; 
    }

    bitrev_num /= bitrev_q;
    bitrev_den /= bitrev_q;

    bitrev_my_rank = (unsigned int)(bitrev_num * nnodes / bitrev_den);

    return ( bitrev_my_rank );
}

/*========================================================*/
/**
 *  @brief Allocate memory region (BYTE data) 
 *
 *  @param  mem_size [in] Memory size
 *  @return Allocated memory region 
 */
/*========================================================*/
BYTE *allocate_byte_memory_region ( unsigned int mem_size ) 
{
    BYTE *mem_region;

    if (( mem_region = (BYTE *)malloc( sizeof(BYTE) * mem_size )) == NULL) 
    { 
       	printf( "<<< ERROR >> Cannot allocate memory for BYTE Data \n" );
        return NULL;
    }
	
    memset( mem_region, 0x00, sizeof(BYTE) * mem_size );

    return (BYTE *)mem_region;
}

/*========================================================*/
/**
 *  @brief Allocate memory region (FLOAT data). 
 *
 *  @param  mem_size [in] Memory size
 *  @return Allocated memory region 
 */
/*========================================================*/
float *allocate_float_memory_region ( unsigned int mem_size ) 
{
    float *mem_region;

    if (( mem_region = (float *)malloc( sizeof(float) * mem_size )) == NULL) 
    { 
       	printf( "<<< ERROR >> Cannot allocate memory for FLOAT Data \n" );
        return NULL;
    }
	
    memset( mem_region, 0x00, sizeof(float) * mem_size );

    return (float *)mem_region;
}


/*========================================================*/
/**
 *  @brief Allocate memory region (UNSIGNED INT data). 
 *
 *  @param  mem_size [in] Memory size
 *  @return Allocated memory region 
 */
/*========================================================*/
unsigned int *allocate_int_memory_region ( unsigned int mem_size ) 
{
    unsigned int *mem_region;

    if (( mem_region = (unsigned int *)malloc( sizeof(unsigned int) * mem_size )) == NULL) 
    { 
       	printf( "<<< ERROR >> Cannot allocate memory for INT Data \n" );
        return NULL;
    }
	
    memset( mem_region, 0x00, sizeof(unsigned int) * mem_size );

    return (unsigned int *)mem_region;
}
