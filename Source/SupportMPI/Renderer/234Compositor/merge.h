/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   merge.h
// @brief  Miscelaneous routines for 234compositor
// @author Jorji Nonaka (jorji@riken.jp)

#pragma once
#include "typedef.h"

// ======================================
//   Function Prototypes
// ======================================
int composite_alpha_rgba32    ( BYTE* , BYTE* , BYTE* , unsigned int ); // Alpha-blend compositing (RGBA32 Pixels)
int composite_alpha_rgba32f   ( BYTE* , BYTE* , BYTE* , unsigned int ); // Alpha-blend compositing (RGBA32 Pixels)
int composite_alpha_rgbaz64   ( BYTE* , BYTE* , BYTE* , unsigned int ); // Alpha-blend compositing (RGBA64 Pixels)
int composite_alpha_rgba128   ( float*, float*, float*, unsigned int ); // Alpha-blend compositing (RGBA128 Pixels)
int composite_alpha_rgbaz160  ( float*, float*, float*, unsigned int ); // Alpha-blend compositing (RGBAZ160 Pixels)

void Create_AlphaBlend_LUT ( void ); // Generate Alpha Blending Look Up Table
int composite_alpha_rgba32_LUT ( BYTE* , BYTE* , BYTE* , unsigned int ); // Alpha-blend compositing (RGBA32 Pixels)

#ifdef C99
 int composite_alpha_rgba56   ( BYTE* restrict, BYTE* restrict, BYTE* restrict, unsigned int  ); // Alpha-blend compositing (RGBA56 Pixels)
 int composite_alpha_rgba64   ( BYTE* restrict, BYTE* restrict, BYTE* restrict, unsigned int  ); // Alpha-blend compositing (RGBA64 Pixels)
 int composite_alpha_rgbaz88  ( BYTE* restrict, BYTE* restrict, BYTE* restrict, unsigned int  ); // Alpha-blend compositing (RGBAZ88 Pixels)
 int composite_alpha_rgbaz96  ( BYTE* restrict, BYTE* restrict, BYTE* restrict, unsigned int  ); // Alpha-blend compositing (RGBAZ96 Pixels)
#else
 int composite_alpha_rgba56   ( BYTE*, BYTE*, BYTE*, unsigned int ); // Alpha-blend compositing (RGBA56 Pixels)
 int composite_alpha_rgba64   ( BYTE*, BYTE*, BYTE*, unsigned int ); // Alpha-blend compositing (RGBA64 Pixels)
 int composite_alpha_rgbaz88  ( BYTE*, BYTE*, BYTE*, unsigned int ); // Alpha-blend compositing (RGBA88 Pixels)
 int composite_alpha_rgbaz96  ( BYTE*, BYTE*, BYTE*, unsigned int ); // Alpha-blend compositing (RGBA96 Pixels)
#endif
