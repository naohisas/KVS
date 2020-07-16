/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/
/**********************************************************/

// @file   exchange.c
// @brief  Data exchange routines for 234Compositor
// @author Jorji Nonaka (jorji@riken.jp)


//#ifndef COMPOSITOR234_H_INCLUDE
//	#include "234compositor.h"
//	#define COMPOSITOR234_H_INCLUDE
//#endif

//#ifndef COMPOSITOR234_MERGE_H_INCLUDE
//	#include "merge.h"
//	#define COMPOSITOR234_MERGE_H_INCLUDE
//#endif

#include "exchange.h"
#include "merge.h"
#include "define.h"
#include "extern_global.h"
#include <math.h>
#include <stdlib.h>


/*========================================================*/
/**
 *  @brief Traditional Direct-Send Image Exchange 
 *         between 3 NODES 
 *         (RGBA32, RGBA56 and RGBA64 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  MPI_COMM_DSEND [in] MPI Communicator
*/
/*========================================================*/
int dsend3_rgba_BYTE ( unsigned int my_rank, unsigned int nnodes, \
			   unsigned int width, unsigned int height, unsigned int image_type, \
			   BYTE *my_image, BYTE *temp_image, \
			   MPI_Comm MPI_COMM_DSEND )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	int ds_pair;
	unsigned int ds_image_size, ds_last_image_size, image_size_remainder;
	unsigned int image_size;

	BYTE* ds_send_image_ptr;
	BYTE* ds_recv_image_ptr;
	BYTE* ds_blnd_image_ptr;

	// ====================================================================
	// 			COMPOSITE IMAGES ( DIRECT-SEND )
	// ====================================================================
	image_size = width * height;

	image_size_remainder = image_size % 3;
	ds_image_size = ceil( image_size / 3 ); 

	if (( image_size_remainder ) == 0 ) 
	{
		ds_last_image_size = ds_image_size;
	} 
	else 
	{
		ds_last_image_size = ds_image_size + image_size_remainder;
	}

	if ( my_rank == 0 )
	{
		//=====================================
		//  		Node 0
		//=====================================
		ds_pair = 1;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ds_image_size * image_type;

		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr += ds_image_size * image_type;

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (2->1->0)
		//=====================================
		ds_blnd_image_ptr = temp_image;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_size * image_type;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) // RGBA32
		{
			composite_alpha_rgba32 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		} 
		else if ( image_type == RGBA56 ) // RGBA56
		{
			composite_alpha_rgba56 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBA64 ) // RGBA64
		{
			composite_alpha_rgba64 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		#endif

		ds_blnd_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) // RGBA32
		{
			composite_alpha_rgba32 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		} 
		else if ( image_type == RGBA56 ) // RGBA56
		{
			composite_alpha_rgba56 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBA64 ) // RGBA64
		{
			composite_alpha_rgba64 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		#endif

	}
	else if ( my_rank == 1 )
	{
		//=====================================
		//  		Node 1
		//=====================================
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
		
		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ds_image_size * image_type * 2;
		ds_recv_image_ptr += ds_image_size * image_type;

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (2->1->0)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_size * image_type;


		if ( image_type == RGBA32 ) // RGBA32
		{
			composite_alpha_rgba32 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBA56 ) // RGBA56
		{
			composite_alpha_rgba56 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBA64 ) // RGBA64
		{
			composite_alpha_rgba64 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}

		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr = temp_image;

		if ( image_type == RGBA32 ) // RGBA32
		{
			composite_alpha_rgba32 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBA56 ) // RGBA56
		{
			composite_alpha_rgba56 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBA64 ) // RGBA64
		{
			composite_alpha_rgba64 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
	}
	else //  my_rank == 2
	{
		//=====================================
		//  		Node 2
		//=====================================
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
	
		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		ds_pair = 1;

		ds_send_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr += ds_last_image_size * image_type;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (2->1->0)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type * 2;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_last_image_size * image_type;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) // RGBA32
		{
			composite_alpha_rgba32 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBA56 ) // RGBA56
		{
			composite_alpha_rgba56 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBA64 ) // RGBA64
		{
			composite_alpha_rgba64 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		#endif

		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type * 2;
		ds_recv_image_ptr = temp_image;
		
		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) // RGBA32
		{
			composite_alpha_rgba32 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBA56 ) // RGBA56
		{
			composite_alpha_rgba56 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBA64 ) // RGBA64
		{
			composite_alpha_rgba64 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		#endif
	}

	//=====================================
	// 	Final Image Gathering
	//=====================================
	if ( my_rank == 0 )
	{
		ds_pair = 1;

		ds_recv_image_ptr = my_image;
		ds_recv_image_ptr += ( ds_image_size * image_type );

		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_recv_image_ptr += (ds_image_size * image_type );

		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );
	}
	else if ( my_rank == 1 )
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_size * image_type );

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}
	else
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_size * image_type * 2 );

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Direct-Send Image Exchange 
 *         between 3 NODES 
 *         (RGBAZ64, RGBAZ88 and RGBAZ96 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int dsend3_rgbaz_BYTE ( unsigned int my_rank, unsigned int nnodes, \
			    unsigned int width, unsigned int height, unsigned int image_type, \
			    BYTE *my_image, BYTE *temp_image, \
			    MPI_Comm MPI_COMM_DSEND )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	int ds_pair;
	unsigned int ds_image_size, ds_last_image_size, image_size_remainder;
	unsigned int image_size;

	BYTE* ds_send_image_ptr;
	BYTE* ds_recv_image_ptr;
	BYTE* ds_blnd_image_ptr;
	
	// ====================================================================
	// 			COMPOSITE IMAGES ( DIRECT-SEND )
	// ====================================================================
	image_size = width * height;

	image_size_remainder = image_size % 3;
	ds_image_size = ceil( image_size / 3 ); 

	if (( image_size_remainder ) == 0 ) 
	{
		ds_last_image_size = ds_image_size;
	} 
	else 
	{
		ds_last_image_size = ds_image_size + image_size_remainder;
	}

	if ( my_rank == 0 )
	{
		//=====================================
		//  		Node 0
		//=====================================
		ds_pair = 1;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_size * image_type );

		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ( ds_image_size * image_type );
		ds_recv_image_ptr += ( ds_image_size * image_type );

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (2->1->0)
		//  2 and 1: temp_image
		//  0: my_image
		//=====================================
		ds_blnd_image_ptr = temp_image;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ( ds_image_size * image_type );

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) // RGBAZ64
		{
			composite_alpha_rgbaz64 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ88 ) // RGBAZ88
		{
			composite_alpha_rgbaz88 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ96 ) // RGBAZ96
		{
			composite_alpha_rgbaz96 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		#endif

		ds_blnd_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) // RGBAZ64
		{
			composite_alpha_rgbaz64 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ88 ) // RGBAZ88
		{
			composite_alpha_rgbaz88 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ96 ) // RGBAZ96
		{
			composite_alpha_rgbaz96 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		#endif
	}
	else if ( my_rank == 1 )
	{
		//=====================================
		//  		Node 1
		//=====================================
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
		
		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ds_image_size * image_type * 2;
		ds_recv_image_ptr += ds_image_size * image_type;

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (2->1->0)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_size * image_type;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) // RGBAZ64
		{
			composite_alpha_rgbaz64 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ88 ) // RGBAZ88
		{
			composite_alpha_rgbaz88 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ96 ) // RGBAZ96
		{
			composite_alpha_rgbaz96 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		#endif

		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) // RGBAZ64
		{
			composite_alpha_rgbaz64 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ88 ) // RGBAZ88
		{
			composite_alpha_rgbaz88 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		else if ( image_type == RGBAZ96 ) // RGBAZ96
		{
			composite_alpha_rgbaz96 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		}
		#endif
	}
	else //  my_rank == 2
	{
		//=====================================
		//  		Node 2
		//=====================================
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
	
		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		ds_pair = 1;

		ds_send_image_ptr += ds_image_size * image_type;
		ds_recv_image_ptr += ds_last_image_size * image_type;

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (2->1->0)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type * 2;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_last_image_size * image_type;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) // RGBAZ64
		{
			composite_alpha_rgbaz64 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBAZ88 ) // RGBAZ88
		{
			composite_alpha_rgbaz88 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBAZ96 ) // RGBAZ96
		{
			composite_alpha_rgbaz96 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		#endif

		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_size * image_type * 2;
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) // RGBAZ64
		{
			composite_alpha_rgbaz64 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBAZ88 ) // RGBAZ88
		{
			composite_alpha_rgbaz88 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		else if ( image_type == RGBAZ96 ) // RGBAZ96
		{
			composite_alpha_rgbaz96 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		}
		#endif
	}

	//=====================================
	// 	Final Image Gathering
	//=====================================
	if ( my_rank == 0 )
	{
		ds_pair = 1;

		ds_recv_image_ptr = my_image;
		ds_recv_image_ptr += ds_image_size * image_type;

		MPI_Irecv( ds_recv_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;
		ds_recv_image_ptr += ds_image_size * image_type;

		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );
	}
	else if ( my_rank == 1 )
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_size * image_type );

		MPI_Isend( ds_send_image_ptr, ds_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}
	else // my_rank = 2
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_size * image_type * 2 );

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * image_type, MPI_BYTE, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}

	return EXIT_SUCCESS;
}



/*========================================================*/
/**
 *  @brief Traditional Direct-Send Image Exchange 
 *         between 3 NODES 
 *         (RGBA128 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  MPI_COMM_DSEND [in] MPI Communicator
*/
/*========================================================*/
int dsend3_rgba128 ( unsigned int my_rank, unsigned int nnodes, \
					 unsigned int width, unsigned int height, unsigned int image_type, \
					 float *my_image, float *temp_image, \
					 MPI_Comm MPI_COMM_DSEND )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	int ds_pair;
	unsigned int ds_image_size, ds_last_image_size, image_size_remainder;
	unsigned int ds_image_offset, ds_last_image_offset;
	unsigned int image_size; 


	float* ds_send_image_ptr;
	float* ds_recv_image_ptr;
	float* ds_blnd_image_ptr;

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( DIRECT-SEND )
	// ====================================================================
	image_size = width * height;

	image_size_remainder = image_size % 3;
	ds_image_size = ceil( image_size / 3 ); 

	if (( image_size_remainder ) == 0 ) 
	{
		ds_last_image_size = ds_image_size;
	} 
	else 
	{
		ds_last_image_size = ds_image_size + image_size_remainder;
	}

	ds_image_offset = ds_image_size * RGBA; // RGBA = Four components
	ds_last_image_offset = ds_last_image_size * RGBA; // RGBA = Four components

	if ( my_rank == 0 )
	{
		ds_pair = 1;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ds_image_offset;

		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * RGBA, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * RGBA, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ds_image_offset;
		ds_recv_image_ptr += ds_image_offset;
	
		MPI_Isend( ds_send_image_ptr, ds_last_image_size * RGBA, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size      * RGBA, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		ds_blnd_image_ptr = temp_image;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_offset;

		composite_alpha_rgba128 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );

		ds_blnd_image_ptr = temp_image;
		ds_recv_image_ptr = temp_image;
		composite_alpha_rgba128 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
	}
	else if ( my_rank == 1 )
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * RGBA, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * RGBA, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
		
		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ( ds_image_offset * 2 );
		ds_recv_image_ptr += ds_image_offset;

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * RGBA, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size      * RGBA, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_offset;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_offset;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgba128 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		#endif

		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_offset;
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgba128 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		#endif
	}
	else //  my_rank == 2
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size      * RGBA, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * RGBA, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
	
		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		ds_pair = 1;

		ds_send_image_ptr += ds_image_offset;
		ds_recv_image_ptr += ds_last_image_offset;

		MPI_Isend( ds_send_image_ptr, ds_image_size      * RGBA, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * RGBA, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ( ds_image_offset * 2 );

		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_last_image_offset;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgba128 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		#endif

		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgba128 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		#endif
	}

	//=====================================
	//  Image Gathering
	//=====================================
	if ( my_rank == 0 )
	{
		ds_pair = 1;

		ds_recv_image_ptr = my_image;
		ds_recv_image_ptr += ds_image_offset;

		MPI_Irecv( ds_recv_image_ptr, ds_image_size      * RGBA, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;
		ds_recv_image_ptr += ds_image_offset;

		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * RGBA, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );
	}
	else if ( my_rank == 1 )
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ds_image_offset;

		MPI_Isend( ds_send_image_ptr, ds_image_size     * RGBA, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}
	else
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_offset * 2 );

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * RGBA, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}

	return EXIT_SUCCESS;

}

/*========================================================*/
/**
 *  @brief Traditional Direct-Send Image Exchange 
 *         between 3 NODES 
 *         (RGBAZ160 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  MPI_COMM_DSEND [in] MPI Communicator
*/
/*========================================================*/
int dsend3_rgbaz160 ( unsigned int my_rank, unsigned int nnodes, \
					  unsigned int width, unsigned int height, unsigned int image_type, \
					  float *my_image, float *temp_image, \
					  MPI_Comm MPI_COMM_DSEND )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	int ds_pair;
	unsigned int ds_image_size, ds_last_image_size, image_size_remainder;
	unsigned int ds_image_offset, ds_last_image_offset;
	unsigned int image_size; 


	float* ds_send_image_ptr;
	float* ds_recv_image_ptr;
	float* ds_blnd_image_ptr;

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( DIRECT-SEND )
	// ====================================================================
	image_size = width * height;

	image_size_remainder = image_size % 3;
	ds_image_size = ceil( image_size / 3 ); 

	if (( image_size_remainder ) == 0 ) 
	{
		ds_last_image_size = ds_image_size;
	} 
	else 
	{
		ds_last_image_size = ds_image_size + image_size_remainder;
	}

	ds_image_offset = ds_image_size * RGBAZ; // RGBAZ = Five components
	ds_last_image_offset = ds_last_image_size * RGBAZ; // RGBAZ = Five components

	if ( my_rank == 0 )
	{
		ds_pair = 1;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ds_image_offset;

		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * RGBAZ, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * RGBAZ, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ds_image_offset;
		ds_recv_image_ptr += ds_image_offset;
	
		MPI_Isend( ds_send_image_ptr, ds_last_image_size * RGBAZ, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size      * RGBAZ, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		ds_blnd_image_ptr = temp_image;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_offset;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( temp_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		#endif

		ds_blnd_image_ptr = temp_image; /// SKMT
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( my_image, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		#endif
	}
	else if ( my_rank == 1 )
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size * RGBAZ, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size * RGBAZ, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
		
		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;

		ds_send_image_ptr += ( ds_image_offset * 2 );
		ds_recv_image_ptr += ds_image_offset;

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * RGBAZ, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_image_size      * RGBAZ, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_offset;
		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_offset;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( ds_blnd_image_ptr, ds_recv_image_ptr, ds_blnd_image_ptr, ds_image_size );
		#endif

		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ds_image_offset;
		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_image_size );
		#endif
	}
	else //  my_rank == 2
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_recv_image_ptr = temp_image;

		MPI_Isend( ds_send_image_ptr, ds_image_size      * RGBAZ, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * RGBAZ, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );
	
		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		ds_pair = 1;

		ds_send_image_ptr += ds_image_offset;
		ds_recv_image_ptr += ds_last_image_offset;

		MPI_Isend( ds_send_image_ptr, ds_image_size      * RGBAZ, MPI_FLOAT, ds_pair, RECV_TAG, MPI_COMM_DSEND, &isend );
		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * RGBAZ, MPI_FLOAT, ds_pair, SEND_TAG, MPI_COMM_DSEND, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		ds_blnd_image_ptr = my_image;
		ds_blnd_image_ptr += ( ds_image_offset * 2 );

		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_last_image_offset;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( ds_recv_image_ptr, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		#endif

		ds_recv_image_ptr = temp_image;

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( temp_image, ds_blnd_image_ptr, ds_blnd_image_ptr, ds_last_image_size );
		#endif
	}

	//=====================================
	//  Image Gathering
	//=====================================
	if ( my_rank == 0 )
	{
		ds_pair = 1;

		ds_recv_image_ptr = temp_image;
		ds_recv_image_ptr += ds_image_offset;

		MPI_Irecv( ds_recv_image_ptr, ds_image_size      * RGBAZ, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );

		ds_pair = 2;
		ds_recv_image_ptr += ds_image_offset;

		MPI_Irecv( ds_recv_image_ptr, ds_last_image_size * RGBAZ, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &irecv );
		MPI_Wait( &irecv, &status );
	}
	else if ( my_rank == 1 )
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ds_image_offset;

		MPI_Isend( ds_send_image_ptr, ds_image_size     * RGBAZ, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}
	else
	{
		ds_pair = 0;

		ds_send_image_ptr = my_image;
		ds_send_image_ptr += ( ds_image_offset * 2 );

		MPI_Isend( ds_send_image_ptr, ds_last_image_size * RGBAZ, MPI_FLOAT, ds_pair, PAIR_TAG, MPI_COMM_DSEND, &isend );
		MPI_Wait( &isend, &status );
	}

	return EXIT_SUCCESS;

}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *        (RGBA32 and RGBA56 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_ID       [in] Image Type ID
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int bswap_rgba_BYTE ( unsigned int my_rank, unsigned int nnodes, \
				      unsigned int width, unsigned int height, unsigned int image_ID, \
				      BYTE *my_image, BYTE **comp_image, \
				      unsigned int *bs_offset, unsigned int *bs_counts, \
				      MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int image_size;
	unsigned int bs_send_image_size;
	unsigned int bs_recv_image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_pair_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 		COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	
	 // Power of two number of pixels
	#ifdef _GATHERV	
		// << MPI_GATHERV >>
		image_size = width * height;
	#else 
		// << MPI_GATHER >>
		// Add remaining pixels (Blank Pixels) in order to make
		// image size (number of pixels) divisible by "nnodes"
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif
	
	switch ( image_ID ) {
		case ID_RGBA32: temp_image_byte_ptr = temp_image_rgba32;
                    break;
		case ID_RGBA56: temp_image_byte_ptr = temp_image_rgba56;
                    break;
		case ID_RGBA64: temp_image_byte_ptr = temp_image_rgba64;
                    break;
		case ID_RGBAZ64: temp_image_byte_ptr = temp_image_rgbaz64;
                     break;
		case ID_RGBAZ88: temp_image_byte_ptr = temp_image_rgbaz88;
                     break;
		case ID_RGBAZ96: temp_image_byte_ptr = temp_image_rgbaz96;
                     break;
 	}

	bs_blnd_image_ptr  = my_image;
	bs_send_image_size = image_size; // width * height ( + global_add_pixels )

	*bs_offset = (unsigned int)0;
	*bs_counts = (unsigned int)0;

	bs_recv_image_size = 0;

	for ( bs_stage = 0; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 ) // LEFT NODE
		{
			//=====================================
			//  		LEFT NODE of pair
			//=====================================
			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;
				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_send_image_size -= global_add_pixels;
				}
			#endif

			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * global_image_type );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			// Assuming bs_pair_image_ptr (OVER) and bs_recv_image_ptr (UNDER)
			if ( image_ID == ID_RGBA32 ) 
			{
				#ifdef _LUTBLEND
					composite_alpha_rgba32 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#elif defined _BLENDF
					composite_alpha_rgba32f ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#else
					composite_alpha_rgba32_LUT ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#endif
			}
			else if ( image_ID == ID_RGBA56 ) 
			{
				composite_alpha_rgba56 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBA64 ) 
			{
				composite_alpha_rgba64 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else // RIGHT NODE
		{		
			//=====================================
			//  		RIGHT NODE of pair
			//=====================================
			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size *= 0.5;
					bs_recv_image_size  = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;

				//=====================================
				//  Offset for Image Gathering
				//=====================================
				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_recv_image_size -= global_add_pixels;
				}
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;
			
			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * global_image_type );

			if ( bs_stage == 0 ) {
				bs_blnd_image_ptr  = temp_image_byte_ptr;
				bs_blnd_image_ptr += ( bs_send_image_size * global_image_type );
			}
			else
			{
				bs_blnd_image_ptr  = bs_pair_image_ptr;
			}

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			// Assuming bs_recv_image_ptr (OVER) and bs_pair_image_ptr (UNDER)
			if ( image_ID == ID_RGBA32 ) 
			{
				#ifdef _LUTBLEND
					composite_alpha_rgba32 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#elif defined _BLENDF
					composite_alpha_rgba32f ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#else
					composite_alpha_rgba32_LUT ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#endif
			}
			else if ( image_ID == ID_RGBA56 ) 
			{
				composite_alpha_rgba56 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBA64 ) 
			{
				composite_alpha_rgba64 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif // #ifdef _NOBLEND
		}
	}

	*bs_counts = bs_recv_image_size;
	*comp_image = (BYTE *)bs_blnd_image_ptr;

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *        (RGBAZ64, RGBAZ88 and RGBAZ96 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_ID       [in] Image Type ID
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int bswap_rgbaz_BYTE ( unsigned int my_rank, unsigned int nnodes, \
				       unsigned int width, unsigned int height, unsigned int image_ID, \
				       BYTE *my_image, BYTE **comp_image, \
				       unsigned int *bs_offset, unsigned int *bs_counts, \
				       MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int image_size;
	unsigned int bs_send_image_size;
	unsigned int bs_recv_image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_pair_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

	// =======================================  
	//     COMPOSITE IMAGES ( BINARY SWAP )
	// =======================================  
	 // Power of two number of pixels
	#ifdef _GATHERV	
		// << MPI_GATHERV >>
		image_size = width * height;
	#else 
		// << MPI_GATHER >>
		// Add remaining pixels (Blank Pixels) in order to make
		// image size (number of pixels) divisible by "nnodes"
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	switch ( image_ID ) {
		case ID_RGBA32:  temp_image_byte_ptr = temp_image_rgba32;
				   global_image_type   = RGBA32; 	
		                 break;
		case ID_RGBA56:  temp_image_byte_ptr = temp_image_rgba56;
				   global_image_type   = RGBA56; 	
                    		   break;
		case ID_RGBA64:  temp_image_byte_ptr = temp_image_rgba64;
				   global_image_type   = RGBA64; 	
                   		   break;
		case ID_RGBAZ64: temp_image_byte_ptr = temp_image_rgbaz64;
		   		   global_image_type   = RGBAZ64; 	
                     	   break;
		case ID_RGBAZ88: temp_image_byte_ptr = temp_image_rgbaz88;
				   global_image_type   = RGBAZ88; 	
                     	   break;
		case ID_RGBAZ96: temp_image_byte_ptr = temp_image_rgbaz96;
				   global_image_type   = RGBAZ96; 	
		                 break;
 	}

	bs_blnd_image_ptr  = my_image;
	bs_send_image_size = image_size;

	*bs_offset = (unsigned int)0;
	*bs_counts = (unsigned int)0;

	bs_recv_image_size = 0;

	for ( bs_stage = 0; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 ) // LEFT NODE
		{
			//=====================================
			//  		LEFT NODE of pair
			//=====================================
			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;
				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_send_image_size -= global_add_pixels;
				}
			#endif

			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * global_image_type );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			// Assuming bs_pair_image_ptr (OVER) and bs_recv_image_ptr (UNDER)
			if ( image_ID == ID_RGBAZ64 ) 
			{
				composite_alpha_rgbaz64 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ88 ) 
			{
				composite_alpha_rgbaz88 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ96 ) 
			{
				composite_alpha_rgbaz96 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else // RIGHT NODE
		{		
			//=====================================
			//  		RIGHT NODE of pair
			//=====================================
			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;

				//=====================================
				//  Offset for Image Gathering
				//=====================================
				*bs_offset += bs_send_image_size;

				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_recv_image_size -= global_add_pixels;
				}
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;
			
			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * global_image_type );

			if ( bs_stage == 0 ) {
				bs_blnd_image_ptr  = temp_image_byte_ptr;
				bs_blnd_image_ptr += ( bs_send_image_size * global_image_type );
			}
			else
			{
				bs_blnd_image_ptr  = bs_pair_image_ptr;
			}

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			// Assuming bs_recv_image_ptr (OVER) and bs_pair_image_ptr (UNDER)
			if ( image_ID == ID_RGBAZ64 ) 
			{
				composite_alpha_rgbaz64 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ88 ) 
			{
				composite_alpha_rgbaz88 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ96 ) 
			{
				composite_alpha_rgbaz96 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif
		}
	}

	*bs_counts  = bs_recv_image_size;
	*comp_image = (BYTE *)bs_blnd_image_ptr;

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *        (RGBA128 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int bswap_rgba128 ( unsigned int my_rank, unsigned int nnodes, \
				    unsigned int width, unsigned int height, unsigned int image_type, \
				    float *my_image, float **comp_image, \
					unsigned int *bs_offset, unsigned int *bs_counts, \
					MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int image_size;
	unsigned int bs_send_image_size;
	unsigned int bs_recv_image_size;

	float* bs_send_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	// Power of two number of pixels
	#ifdef _GATHERV	
		// << MPI_GATHERV >>
		image_size = width * height;
	#else 
		// << MPI_GATHER >>
		// Add remaining pixels (Blank Pixels) in order to make
		// image size (number of pixels) divisible by "nnodes"
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	bs_blnd_image_ptr  = my_image;
	bs_send_image_size = image_size; // width * height ( + global_add_pixels )

	*bs_counts = (unsigned int)0;
	*bs_offset = (unsigned int)0;

	bs_recv_image_size = 0;

	for ( bs_stage = 0; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 ) // LEFT NODE
		{
			//=====================================
			//  		LEFT NODE of pair
			//=====================================
			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;
				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_send_image_size -= global_add_pixels;
				}
			#endif

			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_recv_image_ptr  = temp_image_rgba128;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * RGBA );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			//=====================================
			//  Do nothing (for benchmarking)
			//=====================================
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgba128 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );

			#endif // #ifdef _NOBLEND

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else // RIGHT NODE 
		{		
			//=====================================
			//  		RIGHT NODE of pair
			//=====================================
			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;

				//=====================================
				//  Offset for Image Gathering
				//=====================================
				*bs_offset += bs_send_image_size;

				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_recv_image_size -= global_add_pixels;
				}
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;

			bs_recv_image_ptr  = temp_image_rgba128;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );


			if ( bs_stage == 0 ) {
				bs_blnd_image_ptr  = temp_image_rgba128;
				bs_blnd_image_ptr += ( bs_send_image_size * RGBA );

			}
			else
			{
				bs_blnd_image_ptr  = bs_pair_image_ptr;
			}

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			//=====================================
			//  Do nothing (for benchmarking)
			//=====================================
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );

			#endif // #ifdef _NOBLEND
	
			#ifdef _GATHERV
				bs_send_image_size = bs_recv_image_size;
			#endif
		}
	}

	*bs_counts = bs_recv_image_size;
	*comp_image = (float *)bs_blnd_image_ptr;	

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *        (RGBAZ160 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int bswap_rgbaz160 ( unsigned int my_rank, unsigned int nnodes, \
				     unsigned int width, unsigned int height, unsigned int image_type, \
				     float *my_image, float **comp_image, \
					 unsigned int *bs_offset, unsigned int *bs_counts, \
					 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int image_size;
	unsigned int bs_send_image_size;
	unsigned int bs_recv_image_size;

	float* bs_send_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	 // Power of two number of pixels
	#ifdef _GATHERV	
		// << MPI_GATHERV >>
		image_size = width * height;
	#else 
		// << MPI_GATHER >>
		// Add remaining pixels (Blank Pixels) in order to make
		// image size (number of pixels) divisible by "nnodes"
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	bs_blnd_image_ptr  = my_image;
	bs_send_image_size = image_size; // width * height ( + global_add_pixels )

	*bs_counts = (unsigned int)0;
	*bs_offset = (unsigned int)0;

	bs_recv_image_size = 0;

	for ( bs_stage = 0; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 ) // LEFT NODE
		{
			//=====================================
			//  		LEFT NODE of pair
			//=====================================
			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;
				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_send_image_size -= global_add_pixels;
				}
			#endif

			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_recv_image_ptr  = temp_image_rgbaz160;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * RGBAZ );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			//=====================================
			//  Do nothing (for benchmarking)
			//=====================================
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgbaz160 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );

			#endif // #ifdef _NOBLEND

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else // RIGHT NODE 
		{		
			//=====================================
			//  		RIGHT NODE of pair
			//=====================================
			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				// << MPI_GATHERV >>
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// << MPI_GATHER >>
				// Always EVEN number of pixels
				bs_recv_image_size = bs_send_image_size * 0.5;
				bs_send_image_size = bs_recv_image_size;

				//=====================================
				//  Offset for Image Gathering
				//=====================================
				*bs_offset += bs_send_image_size;

				// In the first stage there is a need to send correct 
				// amount of remaining pixels
				if ( bs_stage == 0 ) {
					bs_recv_image_size -= global_add_pixels;
				}
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;

			bs_recv_image_ptr  = temp_image_rgbaz160;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * RGBAZ );

			if ( bs_stage == 0 ) {
				bs_blnd_image_ptr  = temp_image_rgbaz160;
				bs_blnd_image_ptr += ( bs_send_image_size * RGBAZ );
			}
			else
			{
				bs_blnd_image_ptr  = bs_pair_image_ptr;
			}

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			//=====================================
			//  Do nothing (for benchmarking)
			//=====================================
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgbaz160 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );

			#endif // #ifdef _NOBLEND
	
			#ifdef _GATHERV
				bs_send_image_size = bs_recv_image_size;
			#endif
		}
	}

	*bs_counts = bs_recv_image_size;
	*comp_image = (float *)bs_blnd_image_ptr;	

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         From STAGE 2, After 234Composition
 *         (RGBA32, RGBA56 and RGBA64 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int stage2_bswap_rgba_BYTE ( unsigned int my_rank, unsigned int nnodes, \
							 unsigned int width, unsigned int height, unsigned int image_ID, \
							 BYTE *my_image, BYTE **comp_image, \
							 unsigned int *bs_offset, unsigned int *bs_counts, \
							 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size;
	unsigned int image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_pair_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;
    
	unsigned int bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

//	_Bool bs_left_node;
        bool bs_left_node;

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================

	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	switch ( image_ID ) {
		case ID_RGBA32: temp_image_byte_ptr = temp_image_rgba32;
                    break;
		case ID_RGBA56: temp_image_byte_ptr = temp_image_rgba56;
                    break;
		case ID_RGBA64: temp_image_byte_ptr = temp_image_rgba64;
                    break;
		case ID_RGBAZ64: temp_image_byte_ptr = temp_image_rgbaz64;
                     break;
		case ID_RGBAZ88: temp_image_byte_ptr = temp_image_rgbaz88;
                     break;
		case ID_RGBAZ96: temp_image_byte_ptr = temp_image_rgbaz96;
                     break;
 	}

	bs_send_image_size = image_size >> 1; // width * height / 2
	*bs_counts = (unsigned int)0;

	bs_recv_image_size = 0;

	// STAGE 1
	// Get pointers and info for continung Binary-Swap
	// from Stage 2
	if ((( my_rank % 4 ) == 0 )  || (( my_rank % 4 ) == 2 ))
	{
		*bs_offset = (unsigned int)0;
		bs_blnd_image_ptr = my_image;
	}
	else 
	{
		*bs_offset = (unsigned int)bs_send_image_size;
		bs_blnd_image_ptr = my_image + *bs_offset * global_image_type;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif
	}

	// STAGE 2
	for ( bs_stage = 1; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		bs_left_node = false;
		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 )
		{
			bs_left_node = true;
		}
	
		if ( bs_left_node == true )
		{		
			//=====================================
			//  LEFT NODE of pair
			//=====================================

			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * global_image_type );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			if ( image_ID == ID_RGBA32 ) 
			{
				#ifdef _LUTBLEND
					composite_alpha_rgba32 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#elif defined _BLENDF
					composite_alpha_rgba32f ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#else
					composite_alpha_rgba32_LUT ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#endif
			}
			else if ( image_ID == ID_RGBA56 ) 
			{
				composite_alpha_rgba56 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBA64 ) 
			{
				composite_alpha_rgba64 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else 
		{		
			//=====================================
			//  RIGHT NODE of pair
			//=====================================

			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;
			
			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * global_image_type );
			bs_blnd_image_ptr  = bs_pair_image_ptr;

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			if ( image_ID == ID_RGBA32 ) 
			{
				#ifdef _LUTBLEND
					composite_alpha_rgba32 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#elif defined _BLENDF
					composite_alpha_rgba32f ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#else
					composite_alpha_rgba32_LUT ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
				#endif

			}
			else if ( image_ID == ID_RGBA56 ) 
			{
				composite_alpha_rgba56 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBA64 ) 
			{
				composite_alpha_rgba64 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
	}

	*bs_counts  = bs_recv_image_size;
	*comp_image = (BYTE *)bs_blnd_image_ptr;

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 2 NODES (2-3-4 Decomposition)
 *         (RGBA32, RGBA56 and RGBA64 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap2_rgba_BYTE ( unsigned int my_rank, unsigned int nnodes, \
							   unsigned int width, unsigned int height, unsigned int image_type, \
							   BYTE *my_image, BYTE *temp_image, \
							   MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_left_node;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_half_image_size = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	bs_recv_image_size = 0;

	if ( my_rank == ROOT_NODE ) {
		bs_pair_node = 1; 
		bs_left_node = true;
	} else {
		bs_pair_node = ROOT_NODE; 
		bs_left_node = false;
	}

	if ( bs_left_node == true )
	{		
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( my_image, temp_image, my_image, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( my_image, temp_image, my_image, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( my_image, temp_image, my_image, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else 
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}
	
	return EXIT_SUCCESS;
}


/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 3 NODES (2-3-4 Decomposition)
 *         (RGBA32, RGBA56 and RGBA64 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap3_rgba_BYTE ( unsigned int my_rank, unsigned int nnodes, \
							   unsigned int width, unsigned int height, unsigned int image_type, \
							   BYTE *my_image, BYTE *temp_image, \
  							   MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend, isend_to_0, isend_to_1;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================
	
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_half_image_size = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	bs_recv_image_size = 0;

	if ( my_rank == ROOT_NODE ) {
		bs_pair_node = 1; 
	} else {
		bs_pair_node = ROOT_NODE; 
	}

	if ( my_rank == ROOT_NODE ) // bs_left_node = TRUE;
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( my_image, temp_image, my_image, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( my_image, temp_image, my_image, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( my_image, temp_image, my_image, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( my_image, temp_image, my_image, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( my_image, temp_image, my_image, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( my_image, temp_image, my_image, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 1 ) // bs_left_node = FALSE;
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 2 ) {
	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 , 1 with 2 ( BINARY SWAP )
	// ====================================================================

		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_ptr = my_image;

		bs_send_image_size = bs_half_image_size;
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend_to_0 );

		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &isend_to_1 );
		MPI_Wait( &isend_to_0, &status );
		MPI_Wait( &isend_to_1, &status );
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 4 NODES (2-3-4 Decomposition)
 *         (RGBA32, RGBA56 and RGBA64 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap4_rgba_BYTE ( unsigned int my_rank, unsigned int nnodes, \
						       unsigned int width, unsigned int height, unsigned int image_type, \
						       BYTE *my_image, BYTE *temp_image, \
						       MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;	

	BYTE* bs_send_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================

	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

 	// Variable initialization
	bs_pair_node       = 0;
	bs_send_image_size = 0;
	bs_blnd_image_ptr  = NULL;

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else if ( my_rank == 1 ){
		bs_pair_node = 0; 
	} else if ( my_rank == 2 ){
		bs_pair_node = 3; 
	} else if ( my_rank == 3 ){
		bs_pair_node = 2; 
	}
	
	// =========================================================
	// 					BINARY-SWAP STAGE 1
	// =========================================================

	if (( my_rank == 0 ) || ( my_rank == 2 )) // LEFT NODE
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( my_image, temp_image, my_image, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( my_image, temp_image, my_image, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( my_image, temp_image, my_image, bs_recv_image_size );
			#endif

		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else if (( my_rank == 1 ) || ( my_rank == 3 )) // RIGHT NODE
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}

	// =========================================================
	//		BINARY-SWAP STAGE 2
	// =========================================================

	if ( my_rank == 0 ) // LEFT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( my_image, temp_image, my_image, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( my_image, temp_image, my_image, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( my_image, temp_image, my_image, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 1 ) // RIGHT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 3; 

		bs_recv_image_size = bs_half_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBA32 ) 
		{
			#ifdef _LUTBLEND
				composite_alpha_rgba32 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
			#elif defined _BLENDF
				composite_alpha_rgba32f ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
			#else
				composite_alpha_rgba32_LUT ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
		else if ( image_type == RGBA56 ) 
		{
			composite_alpha_rgba56 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBA64 ) 
		{
			composite_alpha_rgba64 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 2 )
	{
		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_ptr = my_image;

		bs_send_image_size = bs_half_image_size;
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );

	}
	else if ( my_rank == 3 )
	{
		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 
		bs_send_image_ptr  = my_image;

		bs_send_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         From STAGE 2, After 234Composition
 *         (RGBAZ64, RGBAZ88 and RGBAZ96 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int stage2_bswap_rgbaz_BYTE ( unsigned int my_rank, unsigned int nnodes, \
						 	  unsigned int width, unsigned int height, unsigned int image_ID, \
							  BYTE *my_image, BYTE **comp_image, \
							  unsigned int *bs_offset, unsigned int *bs_counts, \
							  MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size;
	unsigned int image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_pair_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

//	_Bool bs_left_node;
        bool bs_left_node;

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	switch ( image_ID ) {
		case ID_RGBA32: temp_image_byte_ptr = temp_image_rgba32;
                    break;
		case ID_RGBA56: temp_image_byte_ptr = temp_image_rgba56;
                    break;
		case ID_RGBA64: temp_image_byte_ptr = temp_image_rgba64;
                    break;
		case ID_RGBAZ64: temp_image_byte_ptr = temp_image_rgbaz64;
                     break;
		case ID_RGBAZ88: temp_image_byte_ptr = temp_image_rgbaz88;
                     break;
		case ID_RGBAZ96: temp_image_byte_ptr = temp_image_rgbaz96;
                     break;
 	}

	bs_send_image_size = image_size * 0.5; // width * height / 2
	*bs_counts = (unsigned int)0;

	bs_recv_image_size = 0;

	// STAGE 1
	if ((( my_rank % 4 ) == 0 )  || (( my_rank % 4 ) == 2 ))
	{
		*bs_offset = (unsigned int)0;
		bs_blnd_image_ptr = my_image;
	}
	else 
	{
		*bs_offset = (unsigned int)bs_send_image_size;
		bs_blnd_image_ptr = my_image + *bs_offset * global_image_type;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif
	}

	// STAGE 2
	for ( bs_stage = 1; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		bs_left_node = false;
		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 )
		{
			bs_left_node = true;
		}
	
		if ( bs_left_node == true )
		{		
			//=====================================
			//  LEFT NODE of pair
			//=====================================

			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * global_image_type );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			if ( image_ID == ID_RGBAZ64 ) 
			{
				composite_alpha_rgbaz64 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ88 ) 
			{
				composite_alpha_rgbaz88 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ96 ) 
			{
				composite_alpha_rgbaz96 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else 
		{		
			//=====================================
			//  RIGHT NODE of pair
			//=====================================

			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;
			
			bs_recv_image_ptr  = temp_image_byte_ptr;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * global_image_type );
			bs_blnd_image_ptr  = bs_pair_image_ptr;

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * global_image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * global_image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			if ( image_ID == ID_RGBAZ64 ) 
			{
				composite_alpha_rgbaz64 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ88 ) 
			{
				composite_alpha_rgbaz88 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			else if ( image_ID == ID_RGBAZ96 ) 
			{
				composite_alpha_rgbaz96 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			}
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
	}

	*bs_counts  = bs_recv_image_size;
	*comp_image = (BYTE *)bs_blnd_image_ptr;

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 2 NODES (2-3-4 Decomposition)
 *         (RGBAZ64, RGBAZ88 and RGBAZ96 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap2_rgbaz_BYTE ( unsigned int my_rank, unsigned int nnodes, \
						    	unsigned int width, unsigned int height, unsigned int image_type, \
						   		BYTE *my_image, BYTE *temp_image, \
								MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_left_node;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
		bs_left_node = true;
	} else {
		bs_pair_node = 0; 
		bs_left_node = false;
	}

	if ( bs_left_node == true )
	{		
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type ); 

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else 
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * image_type ); 

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 3 NODES (2-3-4 Decomposition)
 *         (RGBAZ64, RGBAZ88 and RGBAZ96 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap3_rgbaz_BYTE ( unsigned int my_rank, unsigned int nnodes, \
						  	 	unsigned int width, unsigned int height, unsigned int image_type, \
						   	 	BYTE *my_image, BYTE *temp_image, \
						  	 	MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend, isend_to_0, isend_to_1;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

//	_Bool even_image_size;
        bool even_image_size;

	BYTE* bs_send_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================
	
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	even_image_size = false;
	if (( image_size % 2 ) == 0 ) 
	{
		even_image_size = true;
	}

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else {
		bs_pair_node = 0; 
	}

	if ( my_rank == 0 ) // bs_left_node = TRUE;
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 1 ) // bs_left_node = FALSE;
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		if ( even_image_size == false ) {
			bs_recv_image_size++; 
		}

		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 2 ) {
	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 , 1 with 2 ( BINARY SWAP )
	// ====================================================================

		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_ptr = my_image;

		bs_send_image_size = bs_half_image_size;
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend_to_0 );

		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &isend_to_1 );
		MPI_Wait( &isend_to_0, &status );
		MPI_Wait( &isend_to_1, &status );
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 4 NODES (2-3-4 Decomposition)
 *         (RGBAZ64, RGBAZ88 and RGBAZ96 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap4_rgbaz_BYTE ( unsigned int my_rank, unsigned int nnodes, \
							    unsigned int width, unsigned int height, unsigned int image_type, \
							    BYTE *my_image, BYTE *temp_image, \
							    MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;	

	BYTE* bs_send_image_ptr;
	BYTE* bs_recv_image_ptr;
	BYTE* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================
	
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif	

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	// Variable Initialization
	bs_pair_node = 0;
	bs_send_image_size = 0;
	bs_blnd_image_ptr = NULL;

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else if ( my_rank == 1 ){
		bs_pair_node = 0; 
	} else if ( my_rank == 2 ){
		bs_pair_node = 3; 
	} else if ( my_rank == 3 ){
		bs_pair_node = 2; 
	}
	
	// =========================================================
	// 					BINARY-SWAP STAGE 1
	// =========================================================

	if (( my_rank == 0 ) || ( my_rank == 2 )) // LEFT NODE
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * image_type );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else if (( my_rank == 1 ) || ( my_rank == 3 )) // RIGHT NODE
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _NOBLEND
		#else
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}

	// =========================================================
	// 					BINARY-SWAP STAGE 2
	// =========================================================

	if ( my_rank == 0 ) // LEFT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( my_image, temp_image, my_image, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 1 ) // RIGHT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 3; 

		bs_recv_image_size = bs_half_image_size;

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		MPI_Irecv( temp_image, bs_recv_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		if ( image_type == RGBAZ64 ) 
		{
			composite_alpha_rgbaz64 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ88 ) 
		{
			composite_alpha_rgbaz88 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		else if ( image_type == RGBAZ96 ) 
		{
			composite_alpha_rgbaz96 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		}
		#endif
	}
	else if ( my_rank == 2 )
	{
		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_ptr = my_image;

		bs_send_image_size = bs_half_image_size;
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );

	}
	else if ( my_rank == 3 )
	{
		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 
		bs_send_image_ptr  = my_image;

		bs_send_image_ptr += ( bs_half_image_size * image_type );

		#ifdef _GATHERV	
			if ( image_size % 2 != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * image_type, MPI_BYTE, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         From STAGE 2, After 234Composition
 *         (RGBA128 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int stage2_bswap_rgba128 (  unsigned int my_rank, unsigned int nnodes, \
							unsigned int width, unsigned int height, unsigned int image_type, \
							float *my_image, float **comp_image, \
							unsigned int *bs_offset, unsigned int *bs_counts, \
							MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size;
	unsigned int image_size; 

	float* bs_send_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_left_node, bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	 // Power of two number of pixels
	#ifdef _GATHERV	
		// << MPI_GATHERV >>
		image_size = width * height;
		bs_send_image_size = (unsigned int)floor( image_size * 0.5 ); // width * height / 2
		bs_recv_image_size = bs_send_image_size;
	#else 
		// << MPI_GATHER >>
		// Add remaining pixels (Blank Pixels) in order to make
		// image size (number of pixels) divisible by "nnodes"

		image_size = width * height + global_add_pixels; 
		bs_send_image_size = image_size * 0.5; // width * height / 2
		bs_recv_image_size = bs_send_image_size;
	#endif

	*bs_counts = (unsigned int)0;

	//=====================================
	//  			STAGE 1
	//=====================================
	// Note: bs_send_image_size and bs_recv_image_sizes
	// can be different between MPI_GATHERV and MPI_GATHER
	if ((( my_rank % 4 ) == 0 )  || (( my_rank % 4 ) == 2 )) // LEFT NODE
	{
		*bs_offset = (unsigned int)0;
		bs_blnd_image_ptr = my_image;
	}
	else // RIGHT NODE 
	{
		*bs_offset = (unsigned int)bs_send_image_size;

		bs_blnd_image_ptr  = temp_image_rgba128;  
		bs_blnd_image_ptr += ( *bs_offset * RGBA ); 

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size++;
			}
		#endif
	}

	//=====================================
	//  			STAGE 2
	//=====================================
	for ( bs_stage = 1; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		bs_left_node = false;
		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 )
		{
			bs_left_node = true;
		}
	
		if ( bs_left_node == true )
		{		
			//=====================================
			//  LEFT NODE of pair
			//=====================================
			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_recv_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size *= 0.5;
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_recv_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			bs_recv_image_ptr  = temp_image_rgba128;
			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * RGBA );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgba128 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
		else 
		{		
			//=====================================
			//  RIGHT NODE of pair
			//=====================================
			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_recv_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size *= 0.5;
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_recv_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;
			
			bs_recv_image_ptr  = temp_image_rgba128;

			bs_pair_image_ptr  = bs_blnd_image_ptr ;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );
			bs_blnd_image_ptr  = bs_pair_image_ptr;

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================

			composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif
		}
	}

	*bs_counts  = bs_recv_image_size;
	*comp_image = (float *)bs_blnd_image_ptr;

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 2 NODES (2-3-4 Decomposition)
 *         (RGBA128 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap2_rgba128 ( unsigned int my_rank, unsigned int nnodes, \
							 unsigned int width, unsigned int height, unsigned int image_type, \
							 float *my_image, float *temp_image, \
							 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

	float* bs_send_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 
//	_Bool bs_left_node;
        bool bs_left_node;

	// ====================================================================
	// 			 	COMPOSITE TWO IMAGES ( BINARY SWAP )
	// ====================================================================
	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
		bs_left_node = true;
	} else {
		bs_pair_node = 0; 
		bs_left_node = false;
	}

	if ( bs_left_node == true )
	{		
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;

			if (( image_size % 2 ) == 0 ) // EVEN number of pixels
			{
				bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );
				bs_recv_image_size = bs_half_image_size;
				bs_send_image_size = bs_recv_image_size;
			}
			else // ODD number of pixels
			{
				bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );
				bs_recv_image_size = bs_half_image_size;
				bs_send_image_size = bs_recv_image_size + 1;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;
			bs_half_image_size = image_size * 0.5;

			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;
			// For avoiding memory access problem
			bs_send_image_size -= global_add_pixels; 
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBA );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_recv_image_ptr = my_image;
		bs_pair_image_ptr = temp_image;
		bs_blnd_image_ptr = my_image;
		
		composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else 
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;

			if (( image_size % 2 ) == 0 ) // EVEN number of pixels
			{
				bs_send_image_size = ( unsigned int )floor( image_size * 0.5 );
				bs_recv_image_size = bs_send_image_size;
			}
			else // ODD number of pixels
			{
				bs_send_image_size = ( unsigned int )floor( image_size * 0.5 );
				bs_recv_image_size = bs_send_image_size + 1;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;

			bs_half_image_size = image_size * 0.5;
			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;

			// For avoiding memory access problem
			bs_recv_image_size -= global_add_pixels;
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		#ifdef _GATHERV
			bs_pair_image_ptr  = my_image;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );

			bs_blnd_image_ptr  = temp_image;
			bs_blnd_image_ptr += ( bs_send_image_size * RGBA );
		#else
			bs_pair_image_ptr  = my_image;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );

			bs_blnd_image_ptr  = temp_image;
			bs_blnd_image_ptr += ( bs_send_image_size * RGBA );
		#endif

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	
	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 3 NODES (2-3-4 Decomposition)
 *         (RGBA128 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap3_rgba128 ( unsigned int my_rank, unsigned int nnodes, \
							 unsigned int width, unsigned int height, unsigned int image_type, \
							 float *my_image, float *temp_image, \
							 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend, isend_to_0, isend_to_1;
	MPI_Request irecv, irecv_from_2;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size; 

	float* bs_send_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================
	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else {
		bs_pair_node = 0; 
	}

	if ( my_rank == 0 ) // LEFT NODE
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;
			bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );

			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;

			if (( image_size % 2 ) != 0 ) // ODD number of pixels
			{
				bs_send_image_size++;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;

			bs_half_image_size = image_size * 0.5;
			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;
			// For avoiding memory access problem
			bs_send_image_size -= global_add_pixels; 
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBA );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_recv_image_ptr = my_image;
		bs_pair_image_ptr = temp_image;
		bs_blnd_image_ptr = my_image;
		
		composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		MPI_Irecv( temp_image, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv_from_2 );
		MPI_Wait( &irecv_from_2, &status );

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_recv_image_ptr = my_image;
		bs_pair_image_ptr = temp_image;
		bs_blnd_image_ptr = my_image;
		
		composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else if ( my_rank == 1 ) // RIGHT NODE
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;
			bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );

			bs_send_image_size = bs_half_image_size;
			bs_recv_image_size = bs_send_image_size;

			if (( image_size % 2 ) != 0 ) // ODD number of pixels
			{
				bs_recv_image_size++;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;

			bs_half_image_size = image_size * 0.5;
			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			// For avoiding memory access problem
			bs_recv_image_size -= global_add_pixels;
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		#ifdef _GATHERV
			bs_pair_image_ptr  = my_image;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );

			bs_blnd_image_ptr  = temp_image;
			bs_blnd_image_ptr += ( bs_send_image_size * RGBA );
		#else
			bs_pair_image_ptr  = my_image;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );

			bs_blnd_image_ptr  = temp_image;
			bs_blnd_image_ptr += ( bs_send_image_size * RGBA );
		#endif


		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		MPI_Irecv( temp_image, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &irecv_from_2 );
		MPI_Wait( &irecv_from_2, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgba128 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else if ( my_rank == 2 ) {

		// ====================================================================
		// 			 	COMPOSITE IMAGES 0 , 1 with 2 ( BINARY SWAP )
		// ====================================================================
		//=====================================
		//  	Image Sending to LEFT NODE
		//=====================================
		bs_pair_node = 0; 

		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;

			bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );
			bs_send_image_size = bs_half_image_size;
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;

			bs_half_image_size = image_size * 0.5;
			bs_send_image_size = bs_half_image_size;
		#endif

		bs_send_image_ptr = my_image;

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend_to_0 );

		//=====================================
		//  	Image Sending to RIGHT NODE
		//=====================================
		bs_pair_node = 1; 

		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			if (( image_size % 2 ) != 0 ) // ODD number of pixels
			{
				bs_send_image_size = bs_half_image_size + 1;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			bs_send_image_size = bs_half_image_size - global_add_pixels;
		#endif

		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBA );

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &isend_to_1 );

		MPI_Wait( &isend_to_0, &status );
		MPI_Wait( &isend_to_1, &status );
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 4 NODES (2-3-4 Decomposition)
 *         (RGBA128 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap4_rgba128 ( unsigned int my_rank, unsigned int nnodes, \
							 unsigned int width, unsigned int height, unsigned int image_type, \
							 float *my_image, float *temp_image, \
							 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

	float* bs_send_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================
	bs_pair_node = 0;

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else if ( my_rank == 1 ){
		bs_pair_node = 0; 
	} else if ( my_rank == 2 ){
		bs_pair_node = 3; 
	} else if ( my_rank == 3 ){
		bs_pair_node = 2; 
	}

	// Variable Initialization
	bs_half_image_size = 0;
	bs_send_image_size = 0;
	bs_recv_image_size = 0;
	
	// =========================================================
	// 					BINARY-SWAP STAGE 1
	// =========================================================
	if (( my_rank == 0 ) || ( my_rank == 2 )) // LEFT NODE
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;

			bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );
			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;

			if (( image_size % 2 ) != 0 ) // ODD number of pixels
			{
				bs_send_image_size++;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;
			bs_half_image_size = image_size * 0.5;

			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;
			// For avoiding memory access problem
			bs_send_image_size -= global_add_pixels; 
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBA );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_pair_image_ptr = my_image;
		bs_blnd_image_ptr = my_image;

		composite_alpha_rgba128 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else if (( my_rank == 1 ) || ( my_rank == 3 )) // RIGHT NODE
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		#ifdef _GATHERV	
			// << MPI_GATHERV >>
			image_size = width * height;

			bs_half_image_size = ( unsigned int )floor( image_size * 0.5 );
			bs_send_image_size = bs_half_image_size;
			bs_recv_image_size = bs_send_image_size;

			if (( image_size % 2 ) != 0 ) // ODD number of pixels
			{
				bs_recv_image_size++;
			}
		#else
			// << MPI_GATHER >>
			// Always EVEN number of pixels
			image_size = width * height + global_add_pixels;

			bs_half_image_size = image_size * 0.5;
			bs_recv_image_size = bs_half_image_size;
			bs_send_image_size = bs_recv_image_size;

			// For avoiding memory access problem
			bs_recv_image_size -= global_add_pixels;
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		#ifdef _GATHERV
			bs_pair_image_ptr  = my_image;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );

			bs_blnd_image_ptr  = temp_image;
			bs_blnd_image_ptr += ( bs_send_image_size * RGBA );
		#else
			bs_pair_image_ptr  = my_image;
			bs_pair_image_ptr += ( bs_send_image_size * RGBA );

			bs_blnd_image_ptr  = temp_image;
			bs_blnd_image_ptr += ( bs_send_image_size * RGBA );
		#endif

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		composite_alpha_rgba128 ( temp_image, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	// =========================================================
	// 					BINARY-SWAP STAGE 2
	// =========================================================
	if ( my_rank == 0 ) // LEFT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		MPI_Irecv( temp_image, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_recv_image_ptr = my_image;
		bs_pair_image_ptr = temp_image;
		bs_blnd_image_ptr = my_image;

		composite_alpha_rgba128 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else if ( my_rank == 1 ) // RIGHT NODE
	{
		//=====================================
		// 		Image Receiving from 3
		//=====================================
		bs_pair_node = 3; 

		#ifdef _GATHERV
			image_size = width * height;
			bs_half_image_size = (int)floor( image_size * 0.5 );

			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size = bs_half_image_size + 1;
			}
		#else
			image_size = width * height + global_add_pixels;
			bs_half_image_size = image_size * 0.5;
			bs_recv_image_size = bs_half_image_size - global_add_pixels;
		#endif

		bs_recv_image_ptr  = temp_image;

		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		//=====================================
		// No Image Composition (For Test)
		//=====================================
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_pair_image_ptr  = temp_image;
		bs_pair_image_ptr += ( bs_send_image_size * RGBA );
		bs_blnd_image_ptr  = bs_pair_image_ptr;

		composite_alpha_rgba128 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else if ( my_rank == 2 )
	{
		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_size = bs_half_image_size;

		bs_send_image_ptr = my_image;

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );
	}
	else if ( my_rank == 3 )
	{
		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 

		#ifdef _GATHERV
			bs_send_image_size = bs_recv_image_size;
		#else
			bs_send_image_size = bs_half_image_size - global_add_pixels;
		#endif

		bs_send_image_ptr  = temp_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBA );

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBA, MPI_FLOAT, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         From STAGE 2, After 234Composition
 *         (RGBAZ160 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int stage2_bswap_rgbaz160 (  unsigned int my_rank, unsigned int nnodes, \
							unsigned int width, unsigned int height, unsigned int image_type, \
							float *my_image, float **comp_image, \
							unsigned int *bs_offset, unsigned int *bs_counts, \
							MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size;
	unsigned int image_size; 

	float* bs_send_image_ptr;
	float* bs_pair_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_left_node, bs_pair_offset;
	unsigned int bs_stage, bs_max_stage;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================
	
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	#ifdef C99
		bs_max_stage = (unsigned int)( log2( (double) nnodes ));
	#else
		bs_max_stage = (unsigned int)( (float)log( nnodes ) * M_LOG2E );
	#endif

	bs_recv_image_size = 0;

	bs_send_image_size = image_size >> 1; // width * height / 2
	*bs_counts = (unsigned int)0;

	// STAGE 1
	if ((( my_rank % 4 ) == 0 )  || (( my_rank % 4 ) == 2 ))
	{
		*bs_offset = (unsigned int)0;
		bs_blnd_image_ptr = my_image;
	}
	else 
	{
		*bs_offset = (unsigned int)bs_send_image_size;
		bs_blnd_image_ptr = my_image + *bs_offset * RGBAZ; 

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_send_image_size++;
			}
		#endif
	}

	// STAGE 2
	for ( bs_stage = 1; bs_stage < bs_max_stage; bs_stage++ )
	{
		bs_pair_offset = (int) pow( (double)2, (double)bs_stage ); 

		bs_left_node = false;
		if ((( my_rank / bs_pair_offset ) % 2 ) == 0 )
		{
			bs_left_node = true;
		}
	
		if ( bs_left_node == true )
		{		
			//=====================================
			//  LEFT NODE of pair
			//=====================================

			bs_pair_node = my_rank + bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size;
				}
				else // ODD number of pixels
				{
					bs_recv_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_send_image_size = bs_recv_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			bs_recv_image_ptr  = temp_image_rgbaz160;
			bs_pair_image_ptr  = bs_blnd_image_ptr;
			bs_send_image_ptr  = bs_pair_image_ptr ;
			bs_send_image_ptr += ( bs_recv_image_size * RGBAZ );

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

			MPI_Wait( &isend, &status );
			MPI_Wait( &irecv, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgbaz160 ( bs_pair_image_ptr, bs_recv_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
		else 
		{		
			//=====================================
			//  RIGHT NODE of pair
			//=====================================

			bs_pair_node = my_rank - bs_pair_offset;

			#ifdef _GATHERV	
				if (( bs_send_image_size % 2 ) == 0 ) // EVEN number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size;
				}
				else // ODD number of pixels
				{
					bs_send_image_size = ( unsigned int )floor( bs_send_image_size * 0.5 );
					bs_recv_image_size = bs_send_image_size + 1;
				}
			#else
				// Always EVEN number of pixels
				bs_send_image_size *= 0.5; // DIV 2
				bs_recv_image_size = bs_send_image_size;
			#endif

			//=====================================
			//  Offset for Image Gathering
			//=====================================
			*bs_offset += bs_send_image_size;
			
			bs_recv_image_ptr  = temp_image_rgbaz160;
			bs_send_image_ptr  = bs_blnd_image_ptr;
			bs_pair_image_ptr  = bs_send_image_ptr ;
			bs_pair_image_ptr += ( bs_send_image_size * RGBAZ );
			bs_blnd_image_ptr  = bs_pair_image_ptr;

			//=====================================
			//  Image Exchange between pairs
			//=====================================
			MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
			MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

			MPI_Wait( &irecv, &status );
			MPI_Wait( &isend, &status );

			#ifdef _NOBLEND
			#else
			//=====================================
			//  Image Compositing (Alpha or Depth)
			//=====================================
			composite_alpha_rgbaz160 ( bs_recv_image_ptr, bs_pair_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
			#endif

			bs_blnd_image_ptr  = bs_pair_image_ptr;
			bs_send_image_size = bs_recv_image_size;
		}
	}

	*bs_counts  = bs_recv_image_size;
	*comp_image = (float *)bs_blnd_image_ptr;

	return EXIT_SUCCESS;
}


/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 2 NODES (2-3-4 Decomposition)
 *         (RGBAZ160 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap2_rgbaz160 ( unsigned int my_rank, unsigned int nnodes, \
						 	  unsigned int width, unsigned int height, unsigned int image_type, \
							  float *my_image, float *temp_image, \
							  MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size; 

	float* bs_send_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_left_node;
	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES ( BINARY SWAP )
	// ====================================================================

	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
		bs_left_node = true;
	} else {
		bs_pair_node = 0; 
		bs_left_node = false;
	}

	if ( bs_left_node == true )
	{		
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBAZ );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgbaz160 ( my_image, temp_image, my_image, bs_recv_image_size );
		#endif
	}
	else 
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * RGBAZ );

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( bs_recv_image_ptr, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	
	return EXIT_SUCCESS;

}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 3 NODES (2-3-4 Decomposition)
 *         (RGBAZ160 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap3_rgbaz160 ( unsigned int my_rank, unsigned int nnodes, \
							 unsigned int width, unsigned int height, unsigned int image_type, \
							 float *my_image, float *temp_image, \
							 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend, isend_to_0, isend_to_1;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size; 
	
	float* bs_send_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 

	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================
	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else {
		bs_pair_node = 0; 
	}

	if ( my_rank == 0 ) // bs_left_node = TRUE;
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBAZ );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgbaz160 ( my_image, temp_image, my_image, bs_recv_image_size );
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		MPI_Irecv( temp_image, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgbaz160 ( my_image, temp_image, my_image, bs_recv_image_size );
		#endif

	}
	else if ( my_rank == 1 ) // bs_left_node = FALSE;
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * RGBAZ );

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif

		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		MPI_Irecv( temp_image, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgbaz160 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		#endif

	}
	else if ( my_rank == 2 ) {
	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 , 1 with 2 ( BINARY SWAP )
	// ====================================================================

		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_ptr = my_image;

		bs_send_image_size = bs_half_image_size;
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend_to_0 );

		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBAZ );

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_12_TAG, MPI_COMM_BSWAP, &isend_to_1 );
		MPI_Wait( &isend_to_0, &status );
		MPI_Wait( &isend_to_1, &status );
	}

	return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Traditional Binary-Swap Image Exchange 
 *         GROUP OF 4 NODES (2-3-4 Decomposition)
 *         (RGBAZ160 Pixels) 
 *
 *  @param  my_rank        [in] My Rank
 *  @param  nnodes         [in] Number of Nodes
 *  @param  width          [in] Image Width
 *  @param  height         [in] Image Height
 *  @param  image_type     [in] Image Type
 *  @param  my_image       [in] Image Data 
 *  @param  temp_image     [in] Temporary Buffer
 *  @param  bs_offset      [in] Offset for MPI_Gatherv
 *  @param  bs_counts      [in] Counts for MPI_Gatherv
 *  @param  MPI_COMM_BSWAP [in] MPI Communicator
*/
/*========================================================*/
int partial_bswap4_rgbaz160 ( unsigned int my_rank, unsigned int nnodes, \
							 unsigned int width, unsigned int height, unsigned int image_type, \
							 float *my_image, float *temp_image, \
							 MPI_Comm MPI_COMM_BSWAP )
{
	MPI_Status  status;
	MPI_Request isend;
	MPI_Request irecv;

	unsigned int bs_send_image_size, bs_recv_image_size, bs_half_image_size;
	unsigned int image_size;

	float* bs_send_image_ptr;
	float* bs_recv_image_ptr;
	float* bs_blnd_image_ptr;

	unsigned int bs_pair_node; 
	
	// ====================================================================
	// 			 	COMPOSITE IMAGES 0 and 1 ( BINARY SWAP )
	// ====================================================================

	// Power of two number of pixels
	#ifdef _GATHERV	
		image_size = width * height;
	#else
		// Add remaining pixels (Blank Pixels) to become power-of-two
		image_size = width * height + global_add_pixels; 
	#endif

	bs_send_image_size = 0;

	bs_half_image_size  = image_size; // width * height
	bs_half_image_size *= 0.5; // DIV 2

	bs_pair_node = 0;

	if ( my_rank == 0 ) {
		bs_pair_node = 1; 
	} else if ( my_rank == 1 ){
		bs_pair_node = 0; 
	} else if ( my_rank == 2 ){
		bs_pair_node = 3; 
	} else if ( my_rank == 3 ){
		bs_pair_node = 2; 
	}
	
	// =========================================================
	// 					BINARY-SWAP STAGE 1
	// =========================================================

	if (( my_rank == 0 ) || ( my_rank == 2 )) // LEFT NODE
	{	
		//=====================================
		//  LEFT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		bs_recv_image_ptr  = temp_image;
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBAZ );

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &irecv );

		MPI_Wait( &isend, &status );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgbaz160 ( my_image, temp_image, my_image, bs_recv_image_size );
		#endif
	}
	else if (( my_rank == 1 ) || ( my_rank == 3 )) // RIGHT NODE
	{		
		//=====================================
		//  RIGHT NODE of pair
		//=====================================
		bs_send_image_size = bs_half_image_size; 
		bs_recv_image_size = bs_send_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		//=====================================
		//  Offset for Image Gathering
		//=====================================
		bs_recv_image_ptr = temp_image;
		bs_send_image_ptr = my_image;

		//=====================================
		//  Image Exchange between pairs
		//=====================================
		MPI_Irecv( bs_recv_image_ptr, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, SEND_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, RECV_TAG, MPI_COMM_BSWAP, &isend );

		MPI_Wait( &irecv, &status );
		MPI_Wait( &isend, &status );

		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * RGBAZ );

		#ifdef _NOBLEND
		#else
		composite_alpha_rgbaz160 ( temp_image, bs_blnd_image_ptr, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}

	// =========================================================
	// 					BINARY-SWAP STAGE 2
	// =========================================================
	if ( my_rank == 0 ) // LEFT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 2; 

		bs_recv_image_size = bs_half_image_size;
		MPI_Irecv( temp_image, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		composite_alpha_rgbaz160 ( my_image, temp_image, my_image, bs_recv_image_size );
		#endif
	}
	else if ( my_rank == 1 ) // LEFT NODE
	{
		//=====================================
		// 		Image Receiving from 2
		//=====================================
		bs_pair_node = 3; 

		bs_recv_image_size = bs_half_image_size;

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_recv_image_size++;
			}
		#endif

		MPI_Irecv( temp_image, bs_recv_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &irecv );
		MPI_Wait( &irecv, &status );

		#ifdef _NOBLEND
		#else
		//=====================================
		//  Image Compositing (Alpha or Depth)
		//=====================================
		bs_blnd_image_ptr  = my_image;
		bs_blnd_image_ptr += ( bs_half_image_size * RGBAZ );

		composite_alpha_rgbaz160 ( bs_blnd_image_ptr, temp_image, bs_blnd_image_ptr, bs_recv_image_size );
		#endif
	}
	else if ( my_rank == 2 )
	{
		//=====================================
		//  		Image Sending to 0
		//=====================================
		bs_pair_node = 0; 
		bs_send_image_ptr = my_image;

		bs_send_image_size = bs_half_image_size;
		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_02_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );
	}
	else if ( my_rank == 3 )
	{
		//=====================================
		//  		Image Sending to 1
		//=====================================
		bs_pair_node = 1; 
		bs_send_image_ptr  = my_image;
		bs_send_image_ptr += ( bs_half_image_size * RGBAZ );

		#ifdef _GATHERV	
			if (( image_size % 2 ) != 0 )
			{
				bs_send_image_size++;
			}
		#endif

		MPI_Isend( bs_send_image_ptr, bs_send_image_size * RGBAZ, MPI_FLOAT, bs_pair_node, PAIR_13_TAG, MPI_COMM_BSWAP, &isend );
		MPI_Wait( &isend, &status );
	}
	
	return EXIT_SUCCESS;
}


