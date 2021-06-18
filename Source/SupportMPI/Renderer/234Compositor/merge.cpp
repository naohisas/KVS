/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   merge.c
// @brief  Image data merging routines for 234Compositor
// @author Jorji Nonaka (jorji@riken.jp)



//#ifndef COMPOSITOR234_H_INCLUDE
//	#include "234compositor.h"
//	#define COMPOSITOR234_H_INCLUDE
//#endif

//#ifndef COMPOSITOR234_MISC_H_INCLUDE
//	#include "misc.h"
//	#define COMPOSITOR234_MISC_H_INCLUDE
//#endif

#include "merge.h"
#include "misc.h"
#include "define.h"
#include "extern_global.h"
#include <stdlib.h>

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBA32 Pixels)
 *         Back-to-Front Order
 * 
 *  Consider Pre-multiplied (Weighted) images
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/

#ifdef C99
 int composite_alpha_rgba32 \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgba32 \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
	
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* blend_image_ptr;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b;
	BYTE under_a;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;
	BYTE over_a;

	BYTE blend_r;
	BYTE blend_g;
	BYTE blend_b;
	BYTE blend_a;

	float one_minus_alpha;

	blend_image_ptr = (BYTE *)blend_image;

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBA32;

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				 over_r,  over_g,  over_b,  over_a,   \
				 under_r, under_g, under_b, under_a,  \
				 blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBA ) // SKIP 4 elements
	{
		over_a = (BYTE)over_image[ i + 3 ];
		under_a = (BYTE)under_image[ i + 3 ];

		if ( over_a == 0 ) {
			blend_a = under_a;

			blend_r = (BYTE)under_image[ i     ];
			blend_g = (BYTE)under_image[ i + 1 ];
			blend_b = (BYTE)under_image[ i + 2 ];
		}
		else if ( over_a == 255 ) {
			blend_a = over_a;

			blend_r = (BYTE)over_image[ i     ];
			blend_g = (BYTE)over_image[ i + 1 ];
			blend_b = (BYTE)over_image[ i + 2 ];
		}
		else {
		
			// Separate R, G, B and A values of both 
			// the foreground and background colors	
			over_r = (BYTE)over_image[ i     ];
			over_g = (BYTE)over_image[ i + 1 ];
			over_b = (BYTE)over_image[ i + 2 ];

			under_r = (BYTE)under_image[ i     ];
			under_g = (BYTE)under_image[ i + 1 ];
			under_b = (BYTE)under_image[ i + 2 ];
	
			// Pre-calculate 1 - Src_A
			one_minus_alpha = (float)( 1.0f - ( over_a  / 255.0f ));

			// =======================================================
			blend_a = saturate_add( over_a, (BYTE)( under_a * one_minus_alpha ));

			blend_r = saturate_add( over_r, (BYTE)( under_r * one_minus_alpha ));
			blend_g = saturate_add( over_g, (BYTE)( under_g * one_minus_alpha ));
			blend_b = saturate_add( over_b, (BYTE)( under_b * one_minus_alpha ));
			// =======================================================
		}	
		
		blend_image_ptr[ i     ] = (BYTE)( blend_r ); 
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g ); 
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b ); 
		blend_image_ptr[ i + 3 ] = (BYTE)( blend_a ); 

	}
	return EXIT_SUCCESS;
}

#ifdef C99
 int composite_alpha_rgbaz64 \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgbaz64 \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* over_image_ptr;
	BYTE* under_image_ptr;
	BYTE* blend_image_ptr;

	float* over_image_f_ptr;
	float* under_image_f_ptr;
	float* blend_image_f_ptr;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;
	BYTE over_a;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b;
	BYTE under_a;

	float over_z_f;
	float under_z_f;

	BYTE  blend_r;
	BYTE  blend_g;
	BYTE  blend_b;
	BYTE  blend_a;
	float blend_z;

	blend_image_ptr = (BYTE *)blend_image;
	over_image_ptr  = (BYTE *)over_image;
	under_image_ptr = (BYTE *)under_image;

	blend_image_f_ptr = (float *)blend_image;
	over_image_f_ptr  = (float *)over_image;
	under_image_f_ptr = (float *)under_image;

	full_image_size = image_size * RGBAZ64; // 8 BYTES

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBAZ64; // 8 BYTES

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, \
				  over_r,  over_g,  over_b,  over_a, over_z_f,  \
				  under_r, under_g, under_b, under_a, under_z_f,\
				  blend_r, blend_g, blend_b, blend_a, blend_z ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBAZ64 ) // SKIP 8 BYTES
	{

		over_r = (BYTE)over_image_ptr[ i     ];
		over_g = (BYTE)over_image_ptr[ i + 1 ];
		over_b = (BYTE)over_image_ptr[ i + 2 ];
		over_a = (BYTE)over_image_ptr[ i + 3 ];
		over_image_f_ptr = (float *)&over_image_ptr[ i + 4 ];
		over_z_f = (float)over_image_f_ptr[ 0 ];

		under_r = (BYTE)under_image_ptr[ i     ];
		under_g = (BYTE)under_image_ptr[ i + 1 ];
		under_b = (BYTE)under_image_ptr[ i + 2 ];
		under_a = (BYTE)under_image_ptr[ i + 3 ];
		under_image_f_ptr = (float *)&under_image_ptr[ i + 4 ];
		under_z_f = (float)under_image_f_ptr[ 0 ];

		// Depth sorting if necessary
		if ( over_z_f > under_z_f ) 
		{
			blend_r = under_r;
			blend_g = under_g;
			blend_b = under_b;
			blend_a = under_a;
			blend_z = under_z_f;
		}
		else 
		{
			blend_r = over_r;
			blend_g = over_g;
			blend_b = over_b;
			blend_a = over_a;
			blend_z = over_z_f;
		}



		blend_image_ptr[ i     ] = (BYTE)( blend_r );
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g );
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b );
		blend_image_ptr[ i + 3 ] = (BYTE)( blend_a );
		blend_image_f_ptr = (float *)&blend_image_ptr[ i + 4 ];
		blend_image_f_ptr[ 0 ] = (float)blend_z;
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBA32 Pixels)
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/
#ifdef C99
 int composite_alpha_rgba32f \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
int composite_alpha_rgba32f \
  ( BYTE* over_image, \
    BYTE* under_image, \
    BYTE* blend_image, \
    unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* blend_image_ptr;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b;
	BYTE under_a;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;
	BYTE over_a;

	float over_r_f;
	float over_g_f;
	float over_b_f;
	float over_a_f;

	float under_r_f;
	float under_g_f;
	float under_b_f;
	float under_a_f;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;

	float one_minus_alpha;

	blend_image_ptr = (BYTE *)blend_image;

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBA32;

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				 over_r,  over_g,  over_b,  over_a,   \
				 under_r, under_g, under_b, under_a,  \
				 blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBA ) // SKIP 4 elements
	{
		// Separate R, G, B and A values of both 
		// the foreground and background colors	

		over_r = (BYTE)over_image[ i     ];
		over_g = (BYTE)over_image[ i + 1 ];
		over_b = (BYTE)over_image[ i + 2 ];
		over_a = (BYTE)over_image[ i + 3 ];

		under_r = (BYTE)under_image[ i     ];
		under_g = (BYTE)under_image[ i + 1 ];
		under_b = (BYTE)under_image[ i + 2 ];
		under_a = (BYTE)under_image[ i + 3 ];

		// Convert BYTE to Float (Range: 0.0 - 1.0)
		over_r_f = (float)( over_r / 255.0f );
		over_g_f = (float)( over_g / 255.0f );
		over_b_f = (float)( over_b / 255.0f );
		over_a_f = (float)( over_a / 255.0f );

		under_r_f = (float)( under_r / 255.0f );
		under_g_f = (float)( under_g / 255.0f );
		under_b_f = (float)( under_b / 255.0f );
		under_a_f = (float)( under_a / 255.0f );

		// Pre-calculate 1 - Src_A
		one_minus_alpha = (float)( 1.0f - over_a_f );

		// =======================================================
		blend_a = (float)( over_a_f + ( under_a_f * one_minus_alpha ));

		blend_r = (float)( over_r_f + ( under_r_f * one_minus_alpha )); 
		blend_g = (float)( over_g_f + ( under_g_f * one_minus_alpha )); 
		blend_b = (float)( over_b_f + ( under_b_f * one_minus_alpha )); 

		// Clamp RGB component values if necessary
		blend_r = clamp_float( blend_r, 0.0f, 1.0f );
		blend_g = clamp_float( blend_g, 0.0f, 1.0f );
		blend_b = clamp_float( blend_b, 0.0f, 1.0f );
		blend_a = clamp_float( blend_a, 0.0f, 1.0f );

		blend_image_ptr[ i     ] = (BYTE)( blend_r * 255 ); 
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g * 255 ); 
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b * 255 ); 
		blend_image_ptr[ i + 3 ] = (BYTE)( blend_a * 255 ); 
	}
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBAZ64 Pixels)
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/

#ifdef C99
 int composite_alpha_rgbaz64f \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgbaz64f \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* over_image_ptr;
	BYTE* under_image_ptr;
	BYTE* blend_image_ptr;

	float* over_image_f_ptr;
	float* under_image_f_ptr;
	float* blend_image_f_ptr;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;
	BYTE over_a;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b;
	BYTE under_a;

	float over_r_f;
	float over_g_f;
	float over_b_f;
	float over_a_f;
	float over_z_f;

	float under_r_f;
	float under_g_f;
	float under_b_f;
	float under_a_f;
	float under_z_f;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;
	float blend_z;

	float one_minus_alpha;

	blend_image_ptr = (BYTE *)blend_image;
	over_image_ptr  = (BYTE *)over_image;
	under_image_ptr = (BYTE *)under_image;

	blend_image_f_ptr = (float *)blend_image;
	over_image_f_ptr  = (float *)over_image;
	under_image_f_ptr = (float *)under_image;

	full_image_size = image_size * RGBAZ64; // 8 BYTES

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBAZ64; // 8 BYTES

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
					 over_r,  over_g,  over_b,  over_a, over_z_f,  \
				     under_r, under_g, under_b, under_a, under_z_f,\
					 blend_r, blend_g, blend_b, blend_a, blend_z ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBAZ64 ) // SKIP 8 BYTES
	{

		over_r = (BYTE)over_image_ptr[ i     ];
		over_g = (BYTE)over_image_ptr[ i + 1 ];
		over_b = (BYTE)over_image_ptr[ i + 2 ];
		over_a = (BYTE)over_image_ptr[ i + 3 ];
		over_image_f_ptr = (float *)&over_image_ptr[ i + 4 ];
		over_z_f = (float)over_image_f_ptr[ 0 ];

		under_r = (BYTE)under_image_ptr[ i     ];
		under_g = (BYTE)under_image_ptr[ i + 1 ];
		under_b = (BYTE)under_image_ptr[ i + 2 ];
		under_a = (BYTE)under_image_ptr[ i + 3 ];
		under_image_f_ptr = (float *)&under_image_ptr[ i + 4 ];
		under_z_f = (float)under_image_f_ptr[ 0 ];

		// Depth sorting if necessary
		if ( over_z_f > under_z_f ) 
		{
			blend_r = over_r;
			blend_g = over_g;
			blend_b = over_b;
			blend_a = over_a;
			blend_z = over_z_f;

			over_r = under_r;
			over_g = under_g;
			over_b = under_b;
			over_a = under_a;
			over_z_f = under_z_f;

			under_r = blend_r;
			under_g = blend_g;
			under_b = blend_b;
			under_a = blend_a;
			under_z_f = blend_z;
		}

		// Convert BYTE to Float (Range: 0.0 - 1.0)
		over_r_f = (float)( over_r / 255.0f );
		over_g_f = (float)( over_g / 255.0f );
		over_b_f = (float)( over_b / 255.0f );
		over_a_f = (float)( over_a / 255.0f) ;

		under_r_f = (float)( under_r / 255.0f );
		under_g_f = (float)( under_g / 255.0f );
		under_b_f = (float)( under_b / 255.0f );
		under_a_f = (float)( under_a / 255.0f );

		// Pre-calculate 1 - Src_A
		one_minus_alpha = 1.0f - over_a_f;

		// Calculate Final alpha value
		blend_a = (float)( over_a_f + ( under_a_f * one_minus_alpha ));

		blend_r = (float)( over_r_f + ( under_r_f * one_minus_alpha )); 
		blend_g = (float)( over_g_f + ( under_g_f * one_minus_alpha )); 
		blend_b = (float)( over_b_f + ( under_b_f * one_minus_alpha )); 

		// Clamp RGB component values if necessary
		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image_ptr[ i     ] = (BYTE)( blend_r * 255 );
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g * 255 );
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b * 255 );
		blend_image_ptr[ i + 3 ] = (BYTE)( blend_a * 255 );
		blend_image_f_ptr = (float *)&blend_image_ptr[ i + 4 ];
		blend_image_f_ptr[ 0 ] = (float)over_z_f;
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBA56 Pixels)
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/
#ifdef C99
 int composite_alpha_rgba56 \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgba56 \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif

{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* blend_image_ptr;

	float* over_image_f_ptr;
	float* under_image_f_ptr;
	float* blend_image_f_ptr;

	BYTE  under_r;
	BYTE  under_g;
	BYTE  under_b;
	float under_a;

	BYTE  over_r;
	BYTE  over_g;
	BYTE  over_b;
	float over_a;

	float over_r_f;
	float over_g_f;
	float over_b_f;
	float over_a_f;

	float under_r_f;
	float under_g_f;
	float under_b_f;
	float under_a_f;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;

	float one_minus_alpha;

	blend_image_ptr = (BYTE *)blend_image;

	blend_image_f_ptr = (float *)blend_image;
	over_image_f_ptr  = (float *)over_image;
	under_image_f_ptr = (float *)under_image;

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBA56;

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				 over_r,  over_g,  over_b,  over_a,   \
				 under_r, under_g, under_b, under_a,  \
				 blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBA56 ) // SKIP 7 elements
	{
		// Separate R, G, B and A values of both 
		// the foreground and background colors	
		over_r = (BYTE)over_image[ i     ];
		over_g = (BYTE)over_image[ i + 1 ];
		over_b = (BYTE)over_image[ i + 2 ];

		over_image_f_ptr = (float *)&over_image[ i + 3 ];
		over_a = (float)*over_image_f_ptr;

		under_r = (BYTE)under_image[ i     ];
		under_g = (BYTE)under_image[ i + 1 ];
		under_b = (BYTE)under_image[ i + 2 ];

		under_image_f_ptr = (float *)&under_image[ i + 3 ];
		under_a = (float)*under_image_f_ptr;

		// Convert BYTE to Float (Range: 0.0 - 1.0)
		over_r_f = (float)( over_r / 255.0f );
		over_g_f = (float)( over_g / 255.0f );
		over_b_f = (float)( over_b / 255.0f );
		over_a_f = (float)  over_a ;

		under_r_f = (float)( under_r / 255.0f );
		under_g_f = (float)( under_g / 255.0f );
		under_b_f = (float)( under_b / 255.0f );
		under_a_f = (float)  under_a ;

		// Pre-calculate 1 - Src_A
		one_minus_alpha = 1.0f - over_a_f;

		// Calculate Final alpha value
		blend_a = (float)( over_a_f + ( under_a_f * one_minus_alpha ));

		blend_r = (float)( over_r_f + ( under_r_f * one_minus_alpha )); 
		blend_g = (float)( over_g_f + ( under_g_f * one_minus_alpha )); 
		blend_b = (float)( over_b_f + ( under_b_f * one_minus_alpha )); 
		// =======================================================

		// Clamp RGB component values if necessary
		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image_ptr[ i     ] = (BYTE)( blend_r * 255 ); 
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g * 255 ); 
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b * 255 ); 

		blend_image_f_ptr  = (float *)&blend_image_ptr[ i + 3 ];
		*blend_image_f_ptr = (float) blend_a; 
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBA64 Pixels)
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/

#ifdef C99
 int composite_alpha_rgba64 \
 	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgba64 \
 	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* blend_image_ptr;

	float* over_image_f_ptr;
	float* under_image_f_ptr;
	float* blend_image_f_ptr;

	BYTE  under_r;
	BYTE  under_g;
	BYTE  under_b;
	float under_a;

	BYTE  over_r;
	BYTE  over_g;
	BYTE  over_b;
	float over_a;

	float over_r_f;
	float over_g_f;
	float over_b_f;
	float over_a_f;

	float under_r_f;
	float under_g_f;
	float under_b_f;
	float under_a_f;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;

	float one_minus_alpha;

	blend_image_ptr = (BYTE *)blend_image;

	blend_image_f_ptr = (float *)blend_image;
	over_image_f_ptr  = (float *)over_image;
	under_image_f_ptr = (float *)under_image;

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBA64;

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				 over_r,  over_g,  over_b,  over_a,   \
				 under_r, under_g, under_b, under_a,  \
				 blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBA64 ) // SKIP 8 elements
	{
		// Separate R, G, B and A values of both 
		// the foreground and background colors	
		over_r = (BYTE)over_image[ i     ];
		over_g = (BYTE)over_image[ i + 1 ];
		over_b = (BYTE)over_image[ i + 2 ];

		over_image_f_ptr = (float *)&over_image[ i + 4 ];
		over_a = (float)*over_image_f_ptr;

		under_r = (BYTE)under_image[ i     ];
		under_g = (BYTE)under_image[ i + 1 ];
		under_b = (BYTE)under_image[ i + 2 ];

		under_image_f_ptr = (float *)&under_image[ i + 4 ];
		under_a = (float)*under_image_f_ptr;

		// Convert BYTE to Float (Range: 0.0 - 1.0)
		over_r_f = (float)( over_r / 255.0f );
		over_g_f = (float)( over_g / 255.0f );
		over_b_f = (float)( over_b / 255.0f );
		over_a_f = (float)  over_a ;

		under_r_f = (float)( under_r / 255.0f );
		under_g_f = (float)( under_g / 255.0f );
		under_b_f = (float)( under_b / 255.0f );
		under_a_f = (float)  under_a ;

		// Pre-calculate 1 - Src_A
		one_minus_alpha = 1.0f - over_a_f;

		// Calculate Final alpha value
		blend_a = (float) ( over_a_f + ( under_a_f * one_minus_alpha ));

		blend_r = (float)( over_r_f + ( under_r_f * one_minus_alpha )); 
		blend_g = (float)( over_g_f + ( under_g_f * one_minus_alpha )); 
		blend_b = (float)( over_b_f + ( under_b_f * one_minus_alpha )); 
		// =======================================================
		
		// Clamp RGB component values if necessary
		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image_ptr[ i     ] = (BYTE)( blend_r * 255 ); // R
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g * 255 ); // G
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b * 255 ); // B
		blend_image_ptr[ i + 3 ] = (BYTE)0;                 // X    

		blend_image_f_ptr  = (float *)&blend_image_ptr[ i + 4 ];  // A
		*blend_image_f_ptr = (float) blend_a; 
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBAZ88 Pixels)
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/

#ifdef C99
 int composite_alpha_rgbaz88 \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgbaz88 \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE* over_image_ptr;
	BYTE* under_image_ptr;
	BYTE* blend_image_ptr;

	float* over_image_f_ptr;
	float* under_image_f_ptr;
	float* blend_image_f_ptr;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b;

	float over_r_f;
	float over_g_f;
	float over_b_f;
	float over_a_f;
	float over_z_f;

	float under_r_f;
	float under_g_f;
	float under_b_f;
	float under_a_f;
	float under_z_f;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;
	float blend_z;

	float one_minus_alpha;

	blend_image_ptr = (BYTE *)blend_image;
	over_image_ptr  = (BYTE *)over_image;
	under_image_ptr = (BYTE *)under_image;

	blend_image_f_ptr = (float *)blend_image;
	over_image_f_ptr  = (float *)over_image;
	under_image_f_ptr = (float *)under_image;

	blend_z = 0;
	full_image_size = image_size * RGBAZ88; // 11 BYTES

	//=====================================
	//  		Depth Sorting
	//=====================================
	over_image_f_ptr = (float *)&over_image_ptr[ 3 ];
	over_z_f  = (float) over_image_f_ptr[ 1 ]; // Z

	under_image_f_ptr = (float *)&under_image_ptr[ 3 ];
	under_z_f = (float)under_image_f_ptr[ 1 ]; // Z

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				 over_r_f,  over_g_f,  over_b_f,  over_a_f, over_z_f,  \
				 under_r, under_g, under_b_f, under_a_f, under_z_f,\
				 blend_r, blend_g, blend_b, blend_a, blend_z ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBAZ88 ) // SKIP 11 BYTES
	{
		over_r = (BYTE)over_image_ptr[ i     ]; // R
		over_g = (BYTE)over_image_ptr[ i + 1 ]; // G
		over_b = (BYTE)over_image_ptr[ i + 2 ]; // B

		over_image_f_ptr = (float *)&over_image_ptr[ i + 3 ];
		over_a_f = (float)over_image_f_ptr[ 0 ]; // A
		over_z_f = (float)over_image_f_ptr[ 1 ]; // Z

		under_r = (BYTE)under_image_ptr[ i     ]; // R
		under_g = (BYTE)under_image_ptr[ i + 1 ]; // G
		under_b = (BYTE)under_image_ptr[ i + 2 ]; // B

		under_image_f_ptr = (float *)&under_image_ptr[ i + 3 ];
		under_a_f = (float)under_image_f_ptr[ 0 ]; // A
		under_z_f = (float)under_image_f_ptr[ 1 ]; // Z

		// Depth sorting if necessary
		if ( over_z_f > under_z_f ) 
		{
			blend_r = over_r;
			blend_g = over_g;
			blend_b = over_b;
			blend_a = over_a_f;
			blend_z = over_z_f;

			over_r   = under_r;
			over_g   = under_g;
			over_b   = under_b;
			over_a_f = under_a_f;
			over_z_f = under_z_f;

			under_r   = blend_r;
			under_g   = blend_g;
			under_b   = blend_b;
			under_a_f = blend_a;
			under_z_f = blend_z;
		}

		// Convert BYTE to Float (Range: 0.0 - 1.0)
		over_r_f = (float)( over_r / 255.0f );
		over_g_f = (float)( over_g / 255.0f );
		over_b_f = (float)( over_b / 255.0f );

		under_r_f = (float)( under_r / 255.0f );
		under_g_f = (float)( under_g / 255.0f );
		under_b_f = (float)( under_b / 255.0f );

		// Pre-calculate 1 - Src_A
		one_minus_alpha = 1.0f - over_a_f;

		// Calculate Final alpha value
		blend_a = (float) ( over_a_f + ( under_a_f * one_minus_alpha ));

		blend_r = (float)( over_r_f + ( under_r_f * one_minus_alpha )); 
		blend_g = (float)( over_g_f + ( under_g_f * one_minus_alpha )); 
		blend_b = (float)( over_b_f + ( under_b_f * one_minus_alpha )); 
		// =======================================================

		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image_ptr[ i     ] = (BYTE)( blend_r * 255 ); // R
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g * 255 ); // G
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b * 255 ); // B

		blend_image_f_ptr = (float *)&blend_image_ptr[ i + 3 ];
		blend_image_f_ptr[ 0 ] = (float)blend_a;  // A
		blend_image_f_ptr[ 1 ] = (float)over_z_f; // Z
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBAZ96 Pixels)
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/

#ifdef C99
 int composite_alpha_rgbaz96 \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgbaz96 \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	float* over_image_f_ptr;
	float* under_image_f_ptr;
	float* blend_image_f_ptr;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b;

	float over_r_f;
	float over_g_f;
	float over_b_f;
	float over_a_f;
	float over_z_f;

	float under_r_f;
	float under_g_f;
	float under_b_f;
	float under_a_f;
	float under_z_f;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;
	float blend_z;

	float one_minus_alpha;

	blend_image_f_ptr = (float *)blend_image;
	over_image_f_ptr  = (float *)over_image;
	under_image_f_ptr = (float *)under_image;

	full_image_size = image_size * RGBAZ96; // 12 BYTES

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
					 over_image_f_ptr, under_image_f_ptr, blend_image_f_ptr, \
					 over_r,  over_g,  over_b,  \
				     under_r, under_g, under_b, \
					 over_r_f,  over_g_f,  over_b_f,  over_a_f, over_z_f,  \
				     under_r_f, under_g_f, under_b_f, under_a_f, under_z_f,  \
					 blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBAZ96 ) // SKIP 12 BYTES
	{

		over_r = (BYTE)over_image[ i     ]; // R
		over_g = (BYTE)over_image[ i + 1 ]; // G
		over_b = (BYTE)over_image[ i + 2 ]; // B

		over_image_f_ptr = (float *)&over_image[ i + 4 ];
		over_a_f = (float)over_image_f_ptr[ 0 ]; // A
		over_z_f = (float)over_image_f_ptr[ 1 ]; // Z

		under_r = (BYTE)under_image[ i     ]; // R
		under_g = (BYTE)under_image[ i + 1 ]; // G
		under_b = (BYTE)under_image[ i + 2 ]; // B

		under_image_f_ptr = (float *)&under_image[ i + 4 ];
		under_a_f = (float)under_image_f_ptr[ 0 ]; // A
		under_z_f = (float)under_image_f_ptr[ 1 ]; // Z

		// Depth sorting if necessary
		if ( over_z_f > under_z_f ) 
		{
			blend_r = over_r;
			blend_g = over_g;
			blend_b = over_b;
			blend_a = over_a_f;
			blend_z = over_z_f;

			over_r   = under_r;
			over_g   = under_g;
			over_b   = under_b;
			over_a_f = under_a_f;
			over_z_f = under_z_f;

			under_r   = blend_r;
			under_g   = blend_g;
			under_b   = blend_b;
			under_a_f = blend_a;
			under_z_f = blend_z;
		}

		// Convert BYTE to Float (Range: 0.0 - 1.0)
		over_r_f = (float)( over_r / 255.0f );
		over_g_f = (float)( over_g / 255.0f );
		over_b_f = (float)( over_b / 255.0f );

		under_r_f = (float)( under_r / 255.0f );
		under_g_f = (float)( under_g / 255.0f );
		under_b_f = (float)( under_b / 255.0f );

		// Pre-calculate 1 - Src_A
		one_minus_alpha = 1.0f - over_a_f;

		// Calculate Final alpha value
		blend_a = (float) ( over_a_f + ( under_a_f * one_minus_alpha ));

		blend_r = (float)( over_r_f + ( under_r_f * one_minus_alpha )); 
		blend_g = (float)( over_g_f + ( under_g_f * one_minus_alpha )); 
		blend_b = (float)( over_b_f + ( under_b_f * one_minus_alpha )); 
		// =======================================================

		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image[ i     ] = (BYTE)( blend_r * 255 ); // R
		blend_image[ i + 1 ] = (BYTE)( blend_g * 255 ); // G
		blend_image[ i + 2 ] = (BYTE)( blend_b * 255 ); // B
		blend_image[ i + 3 ] = (BYTE)0;                 // X

		blend_image_f_ptr = (float *)&blend_image[ i + 4 ];
		blend_image_f_ptr[ 0 ] = (float)blend_a;  // A
		blend_image_f_ptr[ 1 ] = (float)over_z_f; // Z
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBA128 Pixels) 
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/
#ifdef C99
 int composite_alpha_rgba128 \
	( float* restrict over_image, \
	  float* restrict under_image, \
	  float* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgba128 \
	( float* over_image, \
	  float* under_image, \
	  float* blend_image, \
	  unsigned int  image_size )
#endif

{
	unsigned int i;
	unsigned int full_image_size;

	float under_r;
	float under_g;
	float under_b; 
	float under_a;

	float over_r;
	float over_g;
	float over_b;
	float over_a;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;
	
	float one_minus_alpha;

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBA; // 4 elements

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				 over_r,  over_g,  over_b,  over_a,   \
				 under_r, under_g, under_b, under_a,  \
				 blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBA ) // SKIP 4 elements(FLOAT)
	{

		// Separate R, G, B and A values of both 
		// the foreground and background colors		
		over_r = (float)over_image[ i     ];
		over_g = (float)over_image[ i + 1 ];
		over_b = (float)over_image[ i + 2 ];
		over_a = (float)over_image[ i + 3 ];

		under_r = (float)under_image[ i     ];
		under_g = (float)under_image[ i + 1 ];
		under_b = (float)under_image[ i + 2 ];
		under_a = (float)under_image[ i + 3 ];

		// =============================================
		// Eliminate branching for compiler optimization
		// =============================================

		// Pre-calculate 1 - Src_A
		one_minus_alpha = (float)(1.0f - over_a);
			
		// Calculate Final alpha value
		blend_a = (float)( over_a + ( under_a * one_minus_alpha ));

		blend_r = (float)( over_r + ( under_r * one_minus_alpha )); 
		blend_g = (float)( over_g + ( under_g * one_minus_alpha )); 
		blend_b = (float)( over_b + ( under_b * one_minus_alpha )); 
		// =======================================================

		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image[ i     ] = (float)blend_r; 
		blend_image[ i + 1 ] = (float)blend_g; 
		blend_image[ i + 2 ] = (float)blend_b; 
		blend_image[ i + 3 ] = (float)blend_a; 
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Alpha-blend compositing (RGBAZ160 Pixels) 
 *         Back-to-Front Order
 *
 *  @param  over_image  [in] Image to be alpha blended
 *  @param  under_image [in] Image to be alpha blended
 *  @param  blend_image [in] Image to be alpha blended
 *  @param  image_size  [in] Image size
*/
/*========================================================*/

#ifdef C99
 int composite_alpha_rgbaz160 \
	( float* restrict over_image, \
	  float* restrict under_image, \
	  float* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgbaz160 \
	( float* over_image, \
	  float* under_image, \
	  float* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	float under_r;
	float under_g;
	float under_b; 
	float under_a;
	float under_z;

	float over_r;
	float over_g;
	float over_b;
	float over_a;
	float over_z;

	float blend_r;
	float blend_g;
	float blend_b;
	float blend_a;
	float blend_z;

	float one_minus_alpha;

	blend_z = 0.0f;
	full_image_size = image_size * RGBAZ;  // 5 elements

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, one_minus_alpha, \
				  over_r,  over_g,  over_b,  over_a, over_z,  \
				  under_r, under_g, under_b, under_a, under_z,  \
				  blend_r, blend_g, blend_b, blend_a, blend_z ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBAZ ) // SKIP 5 elements(FLOAT)
	{
	
		// Separate R, G, B and A values of both 
		// the foreground and background colors		
		over_r = (float)over_image[ i     ];
		over_g = (float)over_image[ i + 1 ];
		over_b = (float)over_image[ i + 2 ];
		over_a = (float)over_image[ i + 3 ];
		over_z = (float)over_image[ i + 4 ];

		under_r = (float)under_image[ i     ];
		under_g = (float)under_image[ i + 1 ];
		under_b = (float)under_image[ i + 2 ];
		under_a = (float)under_image[ i + 3 ];
		under_z = (float)under_image[ i + 4 ];

		// Depth sorting if necessary
		if ( over_z > under_z ) 
		{
			blend_r = over_r;
			blend_g = over_g;
			blend_b = over_b;
			blend_a = over_a;
			blend_z = over_z;

			over_r = under_r;
			over_g = under_g;
			over_b = under_b;
			over_a = under_a;
			over_z = under_z;

			under_r = blend_r;
			under_g = blend_g;
			under_b = blend_b;
			under_a = blend_a;
			under_z = blend_z;
		}

		// =======================================================
		// Pre-calculate 1 - Src_A
		one_minus_alpha = (float)(1.0f - over_a);
			
		// Calculate Final alpha value
		blend_a = (float)( over_a + ( under_a * one_minus_alpha ));

		blend_r = (float)( over_r + ( under_r * one_minus_alpha )); 
		blend_g = (float)( over_g + ( under_g * one_minus_alpha )); 
		blend_b = (float)( over_b + ( under_b * one_minus_alpha )); 
		// =======================================================

		blend_r = clamp_float( blend_r, 0.0, 1.0 );
		blend_g = clamp_float( blend_g, 0.0, 1.0 );
		blend_b = clamp_float( blend_b, 0.0, 1.0 );
		blend_a = clamp_float( blend_a, 0.0, 1.0 );

		blend_image[ i     ] = (float)blend_r; 
		blend_image[ i + 1 ] = (float)blend_g; 
		blend_image[ i + 2 ] = (float)blend_b; 
		blend_image[ i + 3 ] = (float)blend_a;
		blend_image[ i + 4 ] = (float)over_z;
	}

	return EXIT_SUCCESS;
}

// =================================================================
//				  	ALPHA BLENDING IMAGE COMPOSITION
// =================================================================
// Optimization based on LUT (HP Image Compositing Library)
//
// Parallel Compositing Library 
//   http://sourceforge.net/projects/paracomp 
// =================================================================

/* BEGINPARACOMPCOPYRIGHT

* The Parallel Compositing Library
* Copyright (c) 2007 Hewlett-Packard Development Company, L.P.

* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
* any later version.

* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
* details.

* ENDPARACOMPCOPYRIGHT
*/
	
/*===========================================================================*/
/**
 *  @brief Generate look up tables for alpha blending operation. 
 */
/*===========================================================================*/
void Create_AlphaBlend_LUT ( void )
{
	unsigned int Alpha;
	unsigned int Color;
	unsigned int Saturation;
	unsigned int Color_Val;

	unsigned int Alpha_Div_256;
	unsigned int Alpha_Sub_255;

	// Generate Product Lookup Table
	for ( Alpha = 0; Alpha < 256; Alpha++ ) 
	{
		Alpha_Div_256 = Alpha << 8;
		Alpha_Sub_255 = 255 - Alpha;

		for ( Color = 0; Color < 256; Color++ ) 
		{
			Color_Val =  (unsigned int)(( Alpha_Sub_255 * Color ) + 0x80 ) >> 8;
			LUT_Mult[ Alpha_Div_256 | Color ] = ( BYTE )Color_Val;
		}
	}

	// Generate Saturation Lookup Table
	for ( Saturation = 0; Saturation < 512; Saturation++ ) 
	{
		LUT_Sat[ Saturation ] = (BYTE)(( Saturation > 255 ) ? 255 : Saturation);
	}
}

/*===========================================================================*/
/**
 *  @brief Alpha-blend compositing. 
 *
 *  @param  order      [in] OVER or UNDER
 *  @param  othr_image [in] Image to be alpha blended
 *  @param  recv_image [in] Image to be alpha blended
 *  @param  image_size [in] Image size
 *  @param  blnd_image [out] Alpha blended image
 */
/*===========================================================================*/
#ifdef C99
 int composite_alpha_rgba32_LUT \
	( BYTE* restrict over_image, \
	  BYTE* restrict under_image, \
	  BYTE* restrict blend_image, \
	  unsigned int  image_size )
#else
 int composite_alpha_rgba32_LUT \
	( BYTE* over_image, \
	  BYTE* under_image, \
	  BYTE* blend_image, \
	  unsigned int  image_size )
#endif
{
	unsigned int i;
	unsigned int full_image_size;

	BYTE *blend_image_ptr;

	BYTE under_r;
	BYTE under_g;
	BYTE under_b; 
	BYTE under_a;

	BYTE over_r;
	BYTE over_g;
	BYTE over_b;
	BYTE over_a;

	BYTE blend_r;
	BYTE blend_g;
	BYTE blend_b;	
	BYTE blend_a;

	unsigned int Alpha_Div_256;

	blend_image_ptr = (BYTE *)blend_image;

	//=====================================
	//  	Shared Memory Parallelism
	//=====================================
	full_image_size = image_size * RGBA32;

	#if defined ( _OPENMP ) 
		#pragma omp parallel for \
			private( i, Alpha_Div_256, \
				  over_r,  over_g,  over_b,  over_a,   \
				  under_r, under_g, under_b, under_a,  \
				  blend_r, blend_g, blend_b, blend_a ) 
	#endif

	for ( i = 0; i < full_image_size; i += RGBA ) // SKIP 4 elements
	{
		// Separate R, G, B and A values of both 
		// the foreground and background colors	
		over_r = (BYTE)over_image[ i     ];
		over_g = (BYTE)over_image[ i + 1 ];
		over_b = (BYTE)over_image[ i + 2 ];
		over_a = (BYTE)over_image[ i + 3 ];

		under_r = (BYTE)under_image[ i     ];
		under_g = (BYTE)under_image[ i + 1 ];
		under_b = (BYTE)under_image[ i + 2 ];
		under_a = (BYTE)under_image[ i + 3 ];

		Alpha_Div_256 = ((unsigned int)over_a) << 8;

		// Calculate RGBA component values
		blend_r = (BYTE)LUT_Sat[ ((unsigned int)LUT_Mult[ \
							Alpha_Div_256 | ((unsigned int)under_r  ) ] ) \
							+ ((unsigned int)over_r  ) ];
		blend_g = (BYTE)LUT_Sat[  ((unsigned int)LUT_Mult[ \
							Alpha_Div_256 | ((unsigned int)under_g) ] ) \
							+ ((unsigned int)over_g) ];
		blend_b = (BYTE)LUT_Sat[ ((unsigned int)LUT_Mult[ \
							Alpha_Div_256 | ((unsigned int)under_b ) ] ) \
							+ ((unsigned int)over_b ) ];
		blend_a = (BYTE)LUT_Sat[ ((unsigned int)LUT_Mult[ \
							Alpha_Div_256 | ((unsigned int)under_a) ] ) \
							+ ((unsigned int)over_a) ];

		blend_image_ptr[ i     ] = (BYTE)( blend_r ); 
		blend_image_ptr[ i + 1 ] = (BYTE)( blend_g ); 
		blend_image_ptr[ i + 2 ] = (BYTE)( blend_b ); 
		blend_image_ptr[ i + 3 ] = (BYTE)( blend_a ); 
	}
	
	return EXIT_SUCCESS;
}


