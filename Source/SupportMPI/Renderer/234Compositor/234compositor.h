/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   234compositor.h
// @brief  Main routines for 234Compositor
//          2-3-4 Decomposition + Binary-Swap
//          MPI_Gather using MPI Rank in bit-reversed order
// @author Jorji Nonaka (jorji@riken.jp)

#pragma once

// ======================================
//   HEADER FILES
// ======================================

#include <stdio.h>   // printf
#include <string.h>  // malloc, memset, strcpy, strcat, strrchr
#include <math.h>    // ceil, M_LOG2E
#include <stdio.h>   // printf, fprintf, sprintf, fwrite, FILE
#include <stdlib.h>  // atoi, free 
#ifndef HAVE_STDBOOL_H
#include <stdbool.h> // true, false
#endif

// C99 Compiler
#if (__STDC_VERSION__ >= 199901L)
	#define C99
#endif

// MPI Library
#ifndef _MPI_INCLUDE
	#include <mpi.h>
#endif

// OpenMP Library
#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "typedef.h"
#include "define.h"

// ======================================
//   K_234Composition API
// ======================================
int Init_234Composition  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int );
// my_rank, nnodes, width, height, pixel_ID

// Do image composition
int  Do_234Composition  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void*, MPI_Comm );
// my_rank, nnodes, width, height, pixel_ID, merge_ID, *my_image_byte, MPI_COMM

int  Do_234ZComposition  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void*, const float*, MPI_Comm );
// my_rank, nnodes, width, height, pixel_ID, merge_ID, *my_image_byte, *my_image_depth, MPI_COMM 

void* Do_234Composition_Ptr ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void*, MPI_Comm );
// my_rank, nnodes, width, height, pixel_ID, merge_ID, *my_image_byte, MPI_COMM

int Destroy_234Composition ( unsigned int );
// pixel_ID

// Initialize variables and image buffer for 234 Image Compositing 
int Init_234Composition_FLOAT ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int );
// my_rank, nnodes, width, height, pixel_ID

//=====================================
// Initialize variables and image buffer for 234 Image Compositing 
int Init_234Composition_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int );
// my_rank, nnodes, width, height, pixel_ID 

// Do image composition
int  Do_234Composition_Core_BYTE  ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, BYTE*,  MPI_Comm );
// my_rank, nnodes, width, height, pixel_ID, *my_image_byte,  MPI_COMM

int Destroy_234Composition_BYTE ( unsigned int );
// pixel_ID

// Initialize variables and image buffer for 234 Image Compositing 
int Init_234Composition_FLOAT ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int );
// my_rank, nnodes, width, height, pixel_ID

// Do image composition
int  Do_234Composition_Core_FLOAT ( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float*, MPI_Comm );
// my_rank, nnodes, width, height, pixel_ID, *my_image_float,  MPI_COMM

int Destroy_234Composition_FLOAT ( unsigned int );
//=====================================
