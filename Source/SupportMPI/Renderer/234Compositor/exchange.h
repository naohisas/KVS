/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   exchange.h
// @brief  Data exchange routines for 234Compositor
// @author Jorji Nonaka (jorji@riken.jp)

#pragma once
#include "typedef.h"
#include <mpi.h>

// ======================================
//   Function Prototypes
// ======================================
// Binary-Swap (RGBA32 and RGBA56 Pixels)
int bswap_rgba_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE*,  BYTE**,  unsigned int*, unsigned int*, MPI_Comm );
// Binary-Swap (RGBAZ64 and RGBAZ88 Pixels)
int bswap_rgbaz_BYTE ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE*,  BYTE**,  unsigned int*, unsigned int*, MPI_Comm );
// Binary-Swap (RGBA128 Pixels)
int bswap_rgba128    ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float**, unsigned int*, unsigned int*, MPI_Comm );
// Binary-Swap (RGBAZ160 Pixels)
int bswap_rgbaz160   ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float**, unsigned int*, unsigned int*, MPI_Comm );

// Binary-Swap from Stage 2 (RGBA32 and RGBA56)
int stage2_bswap_rgba_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE**, unsigned int*, unsigned int*, MPI_Comm );
// Binary-Swap from Stage 2 (RGBAZ64 and RGBAZ88)
int stage2_bswap_rgbaz_BYTE ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE**, unsigned int*, unsigned int*, MPI_Comm );
// Binary-Swap from Stage 2 (RGBA128 Pixels)
int stage2_bswap_rgba128    ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float**, unsigned int*, unsigned int*, MPI_Comm );
// Binary-Swap from Stage 2 (RGBAZ160 Pixels)
int stage2_bswap_rgbaz160   ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float**, unsigned int*, unsigned int*, MPI_Comm );

// 2 Node Stage 1 Binary-Swap (RGBA32 and RGBA56 Pixels)
int partial_bswap2_rgba_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 2 Node Stage 1 Binary-Swap (RGBAZ64 and RGBAZ88 Pixels)
int partial_bswap2_rgbaz_BYTE ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 2 Node Stage 1 Binary-Swap (RGBA128 Pixels)
int partial_bswap2_rgba128    ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );
// 2 Node Stage 1 Binary-Swap (RGBAZ160 Pixels)
int partial_bswap2_rgbaz160   ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );

// 3 Node Stage 1 Binary-Swap (RGBA32 and RGBA56 Pixels)
int partial_bswap3_rgba_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 3 Node Stage 1 Binary-Swap (RGBAZ64 and RGBAZ88 Pixels)
int partial_bswap3_rgbaz_BYTE ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 3 Node Stage 1 Binary-Swap (RGBA128 Pixels)
int partial_bswap3_rgba128    ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );
// 3 Node Stage 1 Binary-Swap (RGBAZ160 Pixels)
int partial_bswap3_rgbaz160   ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );

// 4 Node Stage 1 Binary-Swap (RGBA32 and RGBA56 Pixels)
int partial_bswap4_rgba_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 4 Node Stage 1 Binary-Swap (RGBAZ64 and RGBAZ88 Pixels)
int partial_bswap4_rgbaz_BYTE ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 4 Node Stage 1 Binary-Swap (RGBA128 Pixels)
int partial_bswap4_rgba128    ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );
// 4 Node Stage 1 Binary-Swap (RGBAZ160 Pixels)
int partial_bswap4_rgbaz160   ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );

// 3 Node Direct-Send (RGBA32 Pixels)
int dsend3_rgba_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 3 Node Direct-Send (RGBAZ64 Pixels)
int dsend3_rgbaz_BYTE ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE* , BYTE* , MPI_Comm );
// 3 Node Direct-Send (RGBA128 Pixels)
int dsend3_rgba128    ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );
// 3 Node Direct-Send (RGBAZ160 Pixels)
int dsend3_rgbaz160   ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, float*, MPI_Comm );
