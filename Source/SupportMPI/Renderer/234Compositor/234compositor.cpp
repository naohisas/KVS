/**********************************************************/
/**
 * 234Compositor - Image data merging library
 *
 * Copyright (c) 2013-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 **/ 
/**********************************************************/

// @file   234compositor.c
// @brief  Main routines for 234Compositor
//          2-3-4 Decomposition + Binary-Swap
//          MPI_Gather using MPI Rank in bit-reversed order
// @author Jorji Nonaka (jorji@riken.jp)


#include "234compositor.h"
#include "misc.h"
#include "exchange.h"
#include "merge.h"
#include "global.h"

/*========================================================*/
/**
 *  @brief Initialize variables and image buffer for
 *         234 Image Compositing
 *
 *  @param  my_rank [in] MPI Rank
 *  @param  nnodes  [in] MPI number of nodes
 *  @param  width   [in] Image width
 *  @param  height  [in] Image size
 *  @param  height  [in] Pixel type
 */
/*========================================================*/
int Init_234Composition ( unsigned int my_rank, unsigned int nnodes, unsigned int width, unsigned int height, unsigned int pixel_ID )
{
    if (( pixel_ID == ID_RGBA32  ) || ( pixel_ID == ID_RGBA56  ) || ( pixel_ID == ID_RGBA64  ) ||
        ( pixel_ID == ID_RGBAZ64 ) || ( pixel_ID == ID_RGBAZ88 ) || ( pixel_ID == ID_RGBAZ96 )) {
        Init_234Composition_BYTE ( my_rank, nnodes, width, height, pixel_ID );
    }
    else if (( pixel_ID == ID_RGBA128  ) || ( pixel_ID == ID_RGBAZ160 )) {
        Init_234Composition_FLOAT ( my_rank, nnodes, width, height, pixel_ID );
    }
    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Do 234 Composition
 *                (BYTE image pixel: RGBA32)
 *
 *  @param  my_rank        [in]  MPI rank number
 *  @param  nnodes         [in]  MPI number of nodes
 *  @param  width          [in]  Image width
 *  @param  height         [in]  Image height
 *  @param  pixel_ID       [in]  Pixel type
 *  @param  rgba_image     [in,out]  Input and Blended Image
 *  @param  MPI_COMM_234BS [in]  MPI Communicator for 234 + Binary-Swap
 */
/*========================================================*/
int  Do_234Composition ( unsigned int my_rank, unsigned int nnodes,
                         unsigned int width, unsigned int height,
                         unsigned int pixel_ID, unsigned int merge_ID,
                         void *my_image, MPI_Comm MPI_COMM_COMPOSITION )
{
    if (( pixel_ID == ID_RGBA32  ) || ( pixel_ID == ID_RGBA56  ) || ( pixel_ID == ID_RGBA64  ) ||
        ( pixel_ID == ID_RGBAZ64 ) || ( pixel_ID == ID_RGBAZ88 ) || ( pixel_ID == ID_RGBAZ96 )) {
        Do_234Composition_Core_BYTE ( my_rank, nnodes,
                                      width, height, pixel_ID, merge_ID,
                                      (BYTE *)my_image, MPI_COMM_COMPOSITION );

        // Copy the gathered image to my_image_byte
        if (( nnodes != 3 ) && ( my_rank == ROOT_NODE )) {
            memcpy ( my_image, temp_image_byte_ptr, width * height * global_image_type * sizeof(BYTE) );
        }
    }
    else if (( pixel_ID == ID_RGBA128  ) || ( pixel_ID == ID_RGBAZ160 )) {
        Do_234Composition_Core_FLOAT ( my_rank, nnodes,
                                       width, height, pixel_ID, merge_ID,
                                       (float *)my_image, MPI_COMM_COMPOSITION );

        // Copy the gathered image to my_image_byte
        if (( nnodes != 3 ) && ( my_rank == ROOT_NODE )) {
            if ( pixel_ID == ID_RGBA128  ) {
                memcpy ( my_image, temp_image_rgba128, width * height * RGBA * sizeof(float));
            }
            else if ( pixel_ID == ID_RGBAZ160 ) {
                memcpy ( my_image, temp_image_rgbaz160, width * height * RGBAZ * sizeof(float));
            }
        }
    }

    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Do 234 Composition 
 *                (BYTE image pixel: RGBA32)
 *
 *  @param  my_rank        [in]  MPI rank number
 *  @param  nnodes         [in]  MPI number of nodes
 *  @param  width          [in]  Image width
 *  @param  height         [in]  Image height
 *  @param  pixel_ID       [in]  Pixel type
 *  @param  rgba_image     [in,out]  Input and Blended Image
 *  @param  MPI_COMM_234BS [in]  MPI Communicator for 234 + Binary-Swap
 */
/*========================================================*/

struct RankDepth {
    int   rank;
    float depth;
} ;

int MyQSortComparator(const void* a , const void* b)
{
    struct RankDepth *x, *y;

    x = (struct RankDepth *)a;
    y = (struct RankDepth *)b;

    return ( x -> depth ) - ( y -> depth ); // Closest in Front
// return ( y -> depth ) - ( x -> depth ); // Closest at Back
}

int  Do_234ZComposition ( unsigned int my_rank, unsigned int nnodes,
                          unsigned int width, unsigned int height,
                          unsigned int pixel_ID, unsigned int merge_ID,
                          void *my_image, const float *my_depth, MPI_Comm MPI_COMM_COMPOSITION )
{
    unsigned int send_rank;
    unsigned int recv_rank;

    unsigned int i, j;
    unsigned int image_size;

    BYTE*  my_BYTE_image_ptr;
    float* my_FLOAT_image_ptr;

    float *my_depth_ptr;
    float min_depth, depth;

    float *depth_list;
    float send_depth[1];

    struct RankDepth BtoF_List[82944];

    BYTE*   rgbaz64_img;
    BYTE*   rgbaz64_byte_ptr;
    float*  rgbaz64_float_ptr;

    float*  rgbaz160_img;
    float*  rgbaz160_float_ptr;

    if (( pixel_ID == ID_RGBA32 ) && ( merge_ID == ALPHA ))
    {

        image_size   = width * height;
        my_depth_ptr = (float *)my_depth;
        min_depth    = (float)*my_depth_ptr++;

        for ( i = 1; i < image_size; i++ ) {
            depth = (float)*my_depth_ptr++;
            if ( depth < min_depth ) {
                min_depth = depth;
            }
        }

#ifdef _234DEBUG
        printf ("[%d of %d] MIN_DEPTH = %f \n", my_rank, nnodes, min_depth );
#endif

        send_depth[0] = min_depth;
        depth_list = (float *)malloc ( nnodes * sizeof(float));
        MPI_Allgather( send_depth, 1, MPI_FLOAT, depth_list, 1, MPI_FLOAT, MPI_COMM_COMPOSITION );

#ifdef _234DEBUG
        for ( i = 0; i < nnodes; i++ )
            printf ("[%d of %d] DEPTH[%d] = %f \n", my_rank, nnodes, i, depth_list[i] );
#endif

        for ( i = 0; i < nnodes; i++ ) {
            BtoF_List[i].rank  = i;
            BtoF_List[i].depth = depth_list[i];
        }

#ifdef _234DEBUG
        for ( i = 0; i < nnodes; i++ )
            printf ("[%d of %d] RANK[%d] DEPTH[%f] \n", my_rank, nnodes, BtoF_List[i].rank, BtoF_List[i].depth );
#endif

        qsort( BtoF_List, nnodes, sizeof(struct RankDepth), MyQSortComparator );

#ifdef _234DEBUG
        for ( i = 0; i < nnodes; i++ )
            printf ("[%d of %d] RANK[%d] DEPTH[%f] \n", my_rank, nnodes, BtoF_List[i].rank, BtoF_List[i].depth );
#endif

        i = 0;
        while ( my_rank != (unsigned int)BtoF_List[i].rank ) {
            i++;
        }

        send_rank = i;
        recv_rank = BtoF_List[my_rank].rank;

        if ( my_rank != send_rank ) {

#ifdef _234DEBUG
            printf ("[%d of %d] SEND TO %d ; RECV FROM %d \n", my_rank, nnodes, send_rank, recv_rank);
#endif

            MPI_Irecv( temp_image_rgba32, image_size * RGBA32, MPI_BYTE, recv_rank, RECV_TAG, MPI_COMM_COMPOSITION, &global_irecv );
            MPI_Isend( my_image,  image_size * RGBA32, MPI_BYTE, send_rank, RECV_TAG, MPI_COMM_COMPOSITION, &global_isend );
            MPI_Wait( &global_irecv, &global_status );
            MPI_Wait( &global_isend, &global_status );

            memcpy ( my_image, temp_image_rgba32, width * height * global_image_type * sizeof(BYTE) );
        }

        Do_234Composition_Core_BYTE ( my_rank, nnodes,
                                      width, height, pixel_ID, merge_ID,
                                      (BYTE *)my_image, MPI_COMM_COMPOSITION );

        // Copy the gathered image to my_image_byte
        if (( nnodes != 3 ) && ( my_rank == ROOT_NODE )) {
            memcpy ( my_image, temp_image_byte_ptr, width * height * global_image_type * sizeof(BYTE) );
        }

        free( depth_list );
    }
    else if (( pixel_ID == ID_RGBAZ64 ) && ( merge_ID == DEPTH ))
    {
        image_size   = width * height;

        // Generate RGBAZ Image Buffer
        if ( ( rgbaz64_img = (BYTE *)allocate_byte_memory_region ( 
                   (unsigned int)( image_size * RGBAZ64 ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        } ;

        my_BYTE_image_ptr = (BYTE  *)my_image;
        my_depth_ptr      = (float *)my_depth;

        rgbaz64_byte_ptr  = (BYTE  *)rgbaz64_img;
        rgbaz64_float_ptr = (float *)rgbaz64_img;

        for ( i = 0; i < height; i++ ) {
            for ( j = 0; j < width; j++ ) {
                *rgbaz64_byte_ptr++ = (unsigned char)*my_BYTE_image_ptr++; // R
                *rgbaz64_byte_ptr++ = (unsigned char)*my_BYTE_image_ptr++; // G
                *rgbaz64_byte_ptr++ = (unsigned char)*my_BYTE_image_ptr++; // B
                *rgbaz64_byte_ptr++ = (unsigned char)*my_BYTE_image_ptr++; // A

                rgbaz64_float_ptr++;
                *rgbaz64_float_ptr++ = (float)*my_depth_ptr++; // Z

                rgbaz64_byte_ptr += 4;
            }
        }

        Do_234Composition_Core_BYTE ( my_rank, nnodes,
                                      width, height, pixel_ID, merge_ID,
                                      (BYTE *)rgbaz64_img, MPI_COMM_COMPOSITION );

        if ( my_rank == ROOT_NODE ) {

            my_BYTE_image_ptr = (BYTE  *)my_image;
            my_depth_ptr = (float*)my_depth; // ADD: sakamoto

            if ( nnodes != 3 )
            {
                rgbaz64_byte_ptr  = (BYTE  *)temp_image_rgbaz64;
            }
            else  // nnodes == 3
            {
                rgbaz64_byte_ptr  = (BYTE  *)rgbaz64_img;
            }

            for ( i = 0; i < height; i++ ) {
                for ( j = 0; j < width; j++ ) {
                    *my_BYTE_image_ptr++ = (unsigned char)*rgbaz64_byte_ptr++; // R
                    *my_BYTE_image_ptr++ = (unsigned char)*rgbaz64_byte_ptr++; // G
                    *my_BYTE_image_ptr++ = (unsigned char)*rgbaz64_byte_ptr++; // B
                    *my_BYTE_image_ptr++ = (unsigned char)*rgbaz64_byte_ptr++; // A

                    *my_depth_ptr++ = *(float*)(rgbaz64_byte_ptr); // ADD: sakamoto
                    rgbaz64_byte_ptr += 4; // SKIP Z
                }
            }
        }

        free( rgbaz64_img );
    }
    else if (( pixel_ID == ID_RGBA128 ) && ( merge_ID == ALPHA )) 
    {
        image_size   = width * height;
        my_depth_ptr = (float *)my_depth;
        min_depth    = (float)*my_depth_ptr++;

        for ( i = 1; i < image_size; i++ ) {
            depth = (float)*my_depth_ptr++;
            if ( depth < min_depth ) {
                min_depth = depth;
            }
        }

#ifdef _234DEBUG
        printf ("[%d of %d] MIN_DEPTH = %f \n", my_rank, nnodes, min_depth );
#endif

        send_depth[0] = min_depth;
        depth_list = (float *)malloc ( nnodes * sizeof(float));
        MPI_Allgather( send_depth, 1, MPI_FLOAT, depth_list, 1, MPI_FLOAT, MPI_COMM_COMPOSITION );

#ifdef _234DEBUG
        for ( i = 0; i < nnodes; i++ )
            printf ("[%d of %d] DEPTH[%d] = %f \n", my_rank, nnodes, i, depth_list[i] );
#endif

        for ( i = 0; i < nnodes; i++ ) {
            BtoF_List[i].rank  = i;
            BtoF_List[i].depth = depth_list[i];
        }

#ifdef _234DEBUG
        for ( i = 0; i < nnodes; i++ )
            printf ("[%d of %d] RANK[%d] DEPTH[%f] \n", my_rank, nnodes, BtoF_List[i].rank, BtoF_List[i].depth );
#endif

        qsort( BtoF_List, nnodes, sizeof(struct RankDepth), MyQSortComparator );

#ifdef _234DEBUG
        for ( i = 0; i < nnodes; i++ )
            printf ("[%d of %d] RANK[%d] DEPTH[%f] \n", my_rank, nnodes, BtoF_List[i].rank, BtoF_List[i].depth );
#endif

        i = 0;
        while ( my_rank != (unsigned int)BtoF_List[i].rank ) {
            i++;
        }

        send_rank = i;
        recv_rank = BtoF_List[my_rank].rank;

        if ( my_rank != send_rank ) {

#ifdef _234DEBUG
            printf ("[%d of %d] SEND TO %d ; RECV FROM %d \n", my_rank, nnodes, send_rank, recv_rank);
#endif

            MPI_Irecv( temp_image_rgba128, image_size * RGBA, MPI_FLOAT, recv_rank, RECV_TAG, MPI_COMM_COMPOSITION, &global_irecv );
            MPI_Isend( my_image,  image_size * RGBA, MPI_FLOAT, send_rank, RECV_TAG, MPI_COMM_COMPOSITION, &global_isend );
            MPI_Wait( &global_irecv, &global_status );
            MPI_Wait( &global_isend, &global_status );

            memcpy ( my_image, temp_image_rgba128, width * height * RGBA * sizeof(float) );
        }

        Do_234Composition_Core_FLOAT ( my_rank, nnodes,
                                       width, height, pixel_ID, merge_ID,
                                       (float *)my_image, MPI_COMM_COMPOSITION );

        // Copy the gathered image to my_image_byte
        if (( nnodes != 3 ) && ( my_rank == ROOT_NODE )) {
            memcpy ( my_image, temp_image_rgba128, width * height * global_image_type * sizeof(float) );
        }

        free( depth_list );
    }
    else if (( pixel_ID == ID_RGBAZ160 ) && ( merge_ID == DEPTH )) {

        image_size   = width * height;

        // Generate RGBAZ Image Buffer
        if ( ( rgbaz160_img = (float *)allocate_float_memory_region (
                   (unsigned int)( image_size * RGBAZ160 ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        } ;

        my_FLOAT_image_ptr = (float *)my_image;
        my_depth_ptr       = (float *)my_depth;

        rgbaz160_float_ptr = (float *)rgbaz160_img;

        for ( i = 0; i < height; i++ ) {
            for ( j = 0; j < width; j++ ) {
                *rgbaz160_float_ptr++ = (float)*my_FLOAT_image_ptr++; // R
                *rgbaz160_float_ptr++ = (float)*my_FLOAT_image_ptr++; // G
                *rgbaz160_float_ptr++ = (float)*my_FLOAT_image_ptr++; // B
                *rgbaz160_float_ptr++ = (float)*my_FLOAT_image_ptr++; // A
                *rgbaz160_float_ptr++ = (float)*my_depth_ptr++; // Z
            }
        }

        Do_234Composition_Core_FLOAT ( my_rank, nnodes,
                                       width, height, pixel_ID, merge_ID,
                                       (float *)rgbaz160_img, MPI_COMM_COMPOSITION );

        if ( my_rank == ROOT_NODE ) {

            my_FLOAT_image_ptr = (float *)my_image;
            rgbaz160_float_ptr  = (float *)temp_image_rgbaz160;

            for ( i = 0; i < height; i++ ) {
                for ( j = 0; j < width; j++ ) {
                    *my_FLOAT_image_ptr++ = (float)*rgbaz160_float_ptr++; // R
                    *my_FLOAT_image_ptr++ = (float)*rgbaz160_float_ptr++; // G
                    *my_FLOAT_image_ptr++ = (float)*rgbaz160_float_ptr++; // B
                    *my_FLOAT_image_ptr++ = (float)*rgbaz160_float_ptr++; // A
                    rgbaz160_float_ptr++; // SKIP Z
                }
            }
        }

        free( rgbaz160_img );
    }
    else
    {
        printf ("Image type NOT VALID !!!! ( Pixel_ID = %d merge_ID = %d ) \n", pixel_ID, merge_ID );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Do 234 Composition_Ptr
 *         (Returns the pointer of Temporary Buffer)
 *         (Avoids costly memory copy to my_image)
 *
 *  @param  my_rank        [in]  MPI rank number
 *  @param  nnodes         [in]  MPI number of nodes
 *  @param  width          [in]  Image width
 *  @param  height         [in]  Image height
 *  @param  pixel_ID       [in]  Pixel type
 *  @param  rgba_image     [in,out]  Input and Blended Image
 *  @param  MPI_COMM_COMPOSITION [in]  MPI Communicator for 234 + Binary-Swap
 */
/*========================================================*/
void*  Do_234Composition_Ptr ( unsigned int my_rank, unsigned int nnodes,
                               unsigned int width, unsigned int height,
                               unsigned int pixel_ID, unsigned int merge_ID,
                               void *my_image, MPI_Comm MPI_COMM_COMPOSITION )
{
    if (( pixel_ID == ID_RGBA32  ) || ( pixel_ID == ID_RGBA56  ) || ( pixel_ID == ID_RGBA64  ) ||
        ( pixel_ID == ID_RGBAZ64 ) || ( pixel_ID == ID_RGBAZ88 ) || ( pixel_ID == ID_RGBAZ96 )) {

        Do_234Composition_Core_BYTE ( my_rank, nnodes,
                                      width, height, pixel_ID, merge_ID,
                                      (BYTE *)my_image, MPI_COMM_COMPOSITION );

        // Return the pointer of the gathered image
        if (( nnodes != 3 ) && ( my_rank == ROOT_NODE )) {
            switch ( pixel_ID ) {
            case ID_RGBA32 : return (BYTE *)temp_image_rgba32;
            case ID_RGBA56 : return (BYTE *)temp_image_rgba56;
            case ID_RGBA64 : return (BYTE *)temp_image_rgba64;
            case ID_RGBAZ64: return (BYTE *)temp_image_rgbaz64;
            case ID_RGBAZ88: return (BYTE *)temp_image_rgbaz88;
            case ID_RGBAZ96: return (BYTE *)temp_image_rgbaz96;
            }
        }
    }
    else if (( pixel_ID == ID_RGBA128  ) || ( pixel_ID == ID_RGBAZ160 )) {

        Do_234Composition_Core_FLOAT ( my_rank, nnodes,
                                       width, height, pixel_ID, merge_ID,
                                       (float *)my_image, MPI_COMM_COMPOSITION );

        // Return the pointer of the gathered image
        if (( nnodes != 3 ) && ( my_rank == ROOT_NODE )) {
            switch ( pixel_ID ) {
            case ID_RGBA128 : return (float *)temp_image_rgba128;
            case ID_RGBAZ160: return (float *)temp_image_rgbaz160;
            }
        }
    }
    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Destroy variables and image buffer for 
 *         234 Image  Compositing 
 *
 *  @param  my_rank [in] MPI Rank
 *  @param  nnodes  [in] MPI number of nodes
 *  @param  width   [in] Image width
 *  @param  height  [in] Image size
 *  @param  height  [in] Pixel type 
 */
/*========================================================*/
int Destroy_234Composition ( unsigned int pixel_ID )
{
    if (( pixel_ID == ID_RGBA32  ) || ( pixel_ID == ID_RGBA56  ) || ( pixel_ID == ID_RGBA64  ) ||
        ( pixel_ID == ID_RGBAZ64 ) || ( pixel_ID == ID_RGBAZ88 ) || ( pixel_ID == ID_RGBAZ96 )) {
        Destroy_234Composition_BYTE ( pixel_ID );
    }
    else if (( pixel_ID == ID_RGBA128  ) || ( pixel_ID == ID_RGBAZ160 )) {
        Destroy_234Composition_FLOAT ( pixel_ID );
    }
    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Initialize variables and image buffer for 
 *         234 Image  Compositing 
 *
 *  @param  my_rank [in] MPI Rank
 *  @param  nnodes  [in] MPI number of nodes
 *  @param  width   [in] Image width
 *  @param  height  [in] Image size
 *  @param  height  [in] Pixel type 
 */
/*========================================================*/
int Init_234Composition_BYTE ( unsigned int my_rank, unsigned int nnodes, \
                               unsigned int width, unsigned int height, unsigned int pixel_ID )
{
    // pixel_ID
    //
    // ID_RGBA32:   RGBA   32-bit
    // ID_RGBA128:  RGBA  128-bit
    // ID_RGBAZ160: RGBAZ160-bit

    // ====================================================================
    unsigned int temp_buffer_size;
    unsigned int i;

    global_num_pixels = width * height;
    global_add_pixels = 0;

#if !defined _GATHERV // MPI_GATHER
    if ( check_pow2 ( nnodes ) == false ) { // Non-power-of-two number of nodes
        i = get_nearest_pow2 ( nnodes );
    }
    else {
        i = nnodes;
    }

    global_mod_pixels = global_num_pixels % i;

    if ( global_mod_pixels != 0 )
    {
        global_add_pixels = i - global_mod_pixels;
        global_num_pixels += global_add_pixels;
    }
#endif

    // Temporary Buffer Size for storing
    // half of the image image (Binary-Swap)
    temp_buffer_size = 0;
    if ( global_num_pixels <= MPIXELS_8 )
    {
        temp_buffer_size = MPIXELS_8;
    }
    else if ( global_num_pixels <= MPIXELS_16 )
    {
        temp_buffer_size = MPIXELS_16;
    }
    else if ( global_num_pixels <= MPIXELS_32 )
    {
        temp_buffer_size = MPIXELS_32;
    }
    else if ( global_num_pixels <= MPIXELS_64 )
    {
        temp_buffer_size = MPIXELS_64;
    }
    else if ( global_num_pixels <= MPIXELS_128 )
    {
        temp_buffer_size = MPIXELS_128;
    }

    // =======================================
    //   Prepare temporay image buffer
    // =======================================
    if ( pixel_ID == ID_RGBA32 )
    {
        global_image_type = RGBA32;
        global_image_size = global_num_pixels * RGBA32; // 8 BYTES RGBA(4)
        temp_buffer_size *= RGBA32;

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgba32 = (BYTE *)allocate_byte_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        } ;

        Create_AlphaBlend_LUT( );
    }
    else if ( pixel_ID == ID_RGBAZ64 )
    {
        global_image_type = RGBAZ64;
        global_image_size = global_num_pixels * RGBAZ64; // 8 BYTES RGBA(4)+Z(4)
        temp_buffer_size *= RGBAZ64;

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgbaz64 = (BYTE *)allocate_byte_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else if ( pixel_ID == ID_RGBA56 )
    {
        global_image_type = RGBA56;
        global_image_size = global_num_pixels * RGBA56; // 7 BYTES RGB(3)+A(4)
        temp_buffer_size *= RGBA56;

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgba56 = (BYTE *)allocate_byte_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else if ( pixel_ID == ID_RGBAZ88 )
    {
        global_image_type = RGBAZ88;
        global_image_size = global_num_pixels * RGBAZ88; // 11 BYTES RGB(3)+A(4)+Z(4)
        temp_buffer_size *= RGBAZ88;

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgbaz88 = (BYTE *)allocate_byte_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else if ( pixel_ID == ID_RGBA64 )
    {
        global_image_type = RGBA64;
        global_image_size = global_num_pixels * RGBA64; // 8 BYTES RGBX(4)+A(4)
        temp_buffer_size *= RGBA64;

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgba64 = (BYTE *)allocate_byte_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else if ( pixel_ID == ID_RGBAZ96 )
    {
        global_image_type = RGBAZ96;
        global_image_size = global_num_pixels * RGBAZ96; // 12 BYTES RGBX(4)+A(4)+Z(4)
        temp_buffer_size *= RGBAZ96;

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgbaz96 = (BYTE *)allocate_byte_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf ("Image type NOT VALID !!!! \n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // ====================================================================

    // =======================================
    //   Prepare list for MPI_Gatherv
    // =======================================
    if ( ( bs_gatherv_offset = (int *)allocate_int_memory_region (
               (unsigned int)( nnodes ))) == NULL ) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if ( ( bs_gatherv_counts = (int *)allocate_int_memory_region (
               (unsigned int)( nnodes ))) == NULL ) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if ( ( bs_gatherv_counts_offset = (int *)allocate_int_memory_region (
               (unsigned int)( 2 * nnodes ))) == NULL ) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // ====================================================================
    // ====================================================================

    if ( check_pow2 ( nnodes ) == true )
    {
        // =======================================
        //  TRADITIONAL BINARY-SWAP
        // =======================================
        is_power_of_two = true; // Power-of-two number of nodes

        // =======================================
        //  Prepare bit-reverse order communicator
        //  for MPI_Gather without reconstruction
        // =======================================
        bitrev_my_rank  = bitrevorder ( my_rank, nnodes ); // Get new Rank (Bit-reversed order)
        bitrev_my_group = 1; // Color for new MPI communicator
        // 1 unique color = 1 group of communicator

        // Generate a new MPI communicator with bit-reversed ranks
        MPI_Comm_split ( MPI_COMM_WORLD, bitrev_my_group, bitrev_my_rank, &MPI_COMM_BITREV );
        MPI_Comm_rank  ( MPI_COMM_BITREV, (int *)&bitrev_my_rank ); // New Rank inside MPI_COMM_BITREV
        MPI_Comm_size  ( MPI_COMM_BITREV, (int *)&bitrev_nnodes  ); // NUmber of nodes in MPI_COMM_BITREV
    }
    else
    {
        // =======================================
        //  2-3-4 DECOMPOSIITON
        // =======================================
        is_power_of_two = false; // Non-power-of-two number of nodes 

        // GROUPS of (2 and 3) or (3 and 4)
        //
        // 2-3 : 2^n < m < 2^n + 2^(n-1)
        // 3-3 : 2^n + 2^(n-1)
        // 3-4 : 2^n + 2^(n-1) < m < 2^(n+1)
        //
        // Example: n = 2
        //          2-3: m = 5 (4 < m < 6)
        //          3-3: m = 6
        //          3-4: m = 7 (6 < m < 8)

        // =======================================
        //  Number of nodes != Power-of-two (2^n)
        // =======================================

        // Obtain the nearest power of two, that is,  2^(n+1)
        // Should be smaller than number of nodes
        near_pow2 = get_nearest_pow2 ( nnodes );

        // Obtain the value of 2^(n)
        ngroups_234 = near_pow2 * 0.5;

        base_234 = floor ( nnodes / ngroups_234 );
        over_234 = nnodes % ngroups_234;

        // Calculate threshold for choosing 2-3 or 3-4
        // 2^(n) + 2^(n-1)
        threshold_234 = nnodes - ( over_234 * ( base_234 + 1 ) );

        if ( my_rank < threshold_234 )
        {
            // 2-3 (GROUPS of 2 and 3)
            my_group_234 = floor ( my_rank / base_234 );
        }
        else
        {
            // 3-4 (GROUPS of 3 and 4)
            my_group_234 = ( ngroups_234 - over_234 ) + (( my_rank - threshold_234 ) / ( base_234 + 1 ));
        }

        // Generate new MPI communciators for each of the 2-3-4 groups
        MPI_Comm_split ( MPI_COMM_WORLD, my_group_234, my_rank, &MPI_COMM_234 );
        MPI_Comm_rank ( MPI_COMM_234, &my_rank_234 );
        MPI_Comm_size ( MPI_COMM_234, &nnodes_234 );

        // =======================================
        //   2nd stage Binary-Swap
        // =======================================

        // List of nodes (2nd stage Binary-Swap) 
        // Local root node (n) and its neighbor (n+1)
        group_bswap = (int *)malloc( sizeof(int) * ngroups_234 * 2 );
        memset( group_bswap, 0x00, sizeof(int) * ngroups_234 * 2 );
        group_bswap_ptr = group_bswap;

        // Smaller than 2-3-4 decomposition threshold
        // 2 of 2-3 ; 3 of 3-3 ; 3 of 3-4
        for ( i = 0; i < ( ngroups_234 - over_234 ); i++ )
        {
            *group_bswap_ptr++ = (int)(  i * base_234 );
            *group_bswap_ptr++ = (int)(( i * base_234 ) + 1 );
        }

        // Larger than 2-3-4 decomposition threshold  
        // 3 of 2-3 ; 4 of 3-4
        for ( i = 0; i < over_234; i++ )
        {
            *group_bswap_ptr++ = (int)(( ngroups_234 - over_234 ) * base_234 ) + ( i * ( base_234 + 1 ));
            *group_bswap_ptr++ = (int)(( ngroups_234 - over_234 ) * base_234 ) + ( i * ( base_234 + 1 ) + 1 );
        }

        // Generate new MPI communcator for 2nd stage Binary-Swap
        MPI_Comm_group ( MPI_COMM_WORLD, &MPI_GROUP_WORLD );
        MPI_Group_incl ( MPI_GROUP_WORLD, ngroups_234 * 2, group_bswap, &MPI_GROUP_STAGE2_BSWAP );
        MPI_Comm_create( MPI_COMM_WORLD, MPI_GROUP_STAGE2_BSWAP, &MPI_COMM_STAGE2_BSWAP );
        MPI_Group_rank ( MPI_GROUP_STAGE2_BSWAP, &stage2_bswap_my_rank );
        MPI_Group_size ( MPI_GROUP_STAGE2_BSWAP, &stage2_bswap_nnodes );

        if (( stage2_bswap_my_rank >= 0 ) && ( stage2_bswap_my_rank < stage2_bswap_nnodes))
        {
            bitrev_my_rank  = bitrevorder ( stage2_bswap_my_rank, stage2_bswap_nnodes ); // Get new Rank (Bit-reversed order)
            bitrev_my_group = 1; // Color for new MPI communicator 
            // 1 unique color = 1 group of communicator

            // Generate a new MPI communicator with bit-reversed ranks
            MPI_Comm_split ( MPI_COMM_STAGE2_BSWAP, bitrev_my_group, bitrev_my_rank, &MPI_COMM_STAGE2_BITREV ); 
            MPI_Comm_rank  ( MPI_COMM_STAGE2_BITREV, &stage2_bitrev_my_rank ); // New Rank inside MPI_COMM_BITREV
            MPI_Comm_size  ( MPI_COMM_STAGE2_BITREV, &stage2_bitrev_nnodes  ); // NUmber of nodes in MPI_COMM_BITREV
        }
    }
    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Destroy variables and image buffer for
 *         234 Image  Compositing
 *
 *  @param  my_rank [in] MPI Rank
 *  @param  nnodes  [in] MPI number of nodes
 *  @param  width   [in] Image width
 *  @param  height  [in] Image size
 *  @param  height  [in] Pixel type
 */
/*========================================================*/
int Destroy_234Composition_BYTE ( unsigned int pixel_ID )
{
    // pixel_ID
    //
    // ID_RGBA32 : RGBA  32-bit
    // ID_RGBA56 : RGBA  56-bit
    // ID_RGBA64 : RGBA  64-bit
    // ID_RGBAZ88: RGBAZ 88-bit
    // ID_RGBAZ96: RGBAZ 96-bit
    // ====================================================================

    // =======================================
    //  Destroy temporay image buffer
    // =======================================
    if ( pixel_ID == ID_RGBA32 )
    {
        if ( temp_image_rgba32 )
            free ( temp_image_rgba32 );
    }
    else if ( pixel_ID == ID_RGBA56 )
    {
        if ( temp_image_rgba56 )
            free ( temp_image_rgba56 );
    }
    else if ( pixel_ID == ID_RGBA64 )
    {
        if ( temp_image_rgba64 )
            free ( temp_image_rgba64 );
    }
    else if ( pixel_ID == ID_RGBAZ88 )
    {
        if ( temp_image_rgbaz88 )
            free ( temp_image_rgbaz88 );
    }
    else if ( pixel_ID == ID_RGBAZ96 )
    {
        if ( temp_image_rgbaz96 )
            free ( temp_image_rgbaz96 );
    }

    // =======================================
    //  Destroy lists for MPI_Gatherv
    // =======================================
    if ( bs_gatherv_offset )
        free ( bs_gatherv_offset );

    if ( bs_gatherv_counts )
        free ( bs_gatherv_counts );

    if ( bs_gatherv_counts_offset )
        free ( bs_gatherv_counts_offset );

    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Do 234 Composition 
 *         (BYTE image pixel: RGBA32)
 *
 *  @param  my_rank        [in]  MPI rank number
 *  @param  nnodes         [in]  MPI number of nodes
 *  @param  width          [in]  Image width
 *  @param  height         [in]  Image height
 *  @param  pixel_ID       [in]  Pixel type
 *  @param  rgba_image     [in,out]  Input and Blended Image
 *  @param  MPI_COMM_234BS [in]  MPI Communicator for 234 + Binary-Swap
 */
/*========================================================*/
int  Do_234Composition_Core_BYTE ( unsigned int my_rank, unsigned int nnodes,
                                   unsigned int width, unsigned int height, unsigned int pixel_ID, unsigned int merge_ID,
                                   BYTE *my_image_byte, MPI_Comm MPI_COMM_234BS )
{

#ifdef _GATHERV
    unsigned int counts_offset[2]; // Pixel Counts and Offset
    unsigned int counter;          // Loop counter
#endif

    BYTE* comp_image_byte;

    if ( is_power_of_two == true )
    {
        // ====================================================================
        //  TRADITIONAL BINARY-SWAP
        // ====================================================================
        if (( pixel_ID == ID_RGBA32) || ( pixel_ID == ID_RGBA56 ) || ( pixel_ID == ID_RGBA64 ))
        {
            //=========================================
            if ( pixel_ID == ID_RGBA32 )
            {
                temp_image_byte_ptr = temp_image_rgba32;
            }
            else if ( pixel_ID == ID_RGBA56 )
            {
                temp_image_byte_ptr = temp_image_rgba56;
            }
            else if ( pixel_ID == ID_RGBA64 )
            {
                temp_image_byte_ptr = temp_image_rgba64;
            }
            //=========================================

            bswap_rgba_BYTE ( my_rank, nnodes, width, height, pixel_ID,
                              my_image_byte, &comp_image_byte, &bs_offset, &bs_counts, MPI_COMM_234BS );

            // ============ Final Image Gathering ==============
#ifdef _NOGATHER
            // NO FINAL IMAGE GATHERING

#elif _GATHERV
            // ============ (BEGIN)  MPI_Gatherv ===============
            bs_counts *= global_image_type; // 4, 7 or 8 BYTES
            bs_offset *= global_image_type; // 4, 7 or 8 BYTES

#ifdef _GATHER_TWICE
            MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );
            MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );

            MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                          temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset,
                          MPI_BYTE, ROOT_NODE, MPI_COMM_234BS );
#else
            counts_offset[0] = bs_counts;
            counts_offset[1] = bs_offset;

            MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT,
                        ROOT_NODE, MPI_COMM_234BS );

            bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
            bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
            bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

            for ( counter = 0; counter < nnodes; counter++ ) {
                *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
            }

            MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                          temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset,
                          MPI_BYTE, ROOT_NODE, MPI_COMM_234BS );
#endif
            // ============== (END) MPI_Gatherv ===============

#else // #ifdef _NOGATHER #elif _GATHERV

            // ============ (BEGIN) MPI_Gather ================
            bs_counts *= global_image_type; // 4, 7 or 8 BYTES

            // Gather the composited partial images to TEMP_IMAGE
            // since its size is larger than initial IMAGE_BUFFER
            MPI_Gather ( comp_image_byte, bs_counts, MPI_BYTE,
                         temp_image_byte_ptr, bs_counts, MPI_BYTE, ROOT_NODE, MPI_COMM_BITREV );
            // =============== (END) MPI_Gather ===============

#endif // ifndef _GATHERV
        }
        else if (( pixel_ID == ID_RGBAZ64 ) || ( pixel_ID == ID_RGBAZ88 ) || ( pixel_ID == ID_RGBAZ96 ))
        {
            //=========================================
            if ( pixel_ID == ID_RGBAZ64 )
            {
                temp_image_byte_ptr = temp_image_rgbaz64;
            }
            else if ( pixel_ID == ID_RGBAZ88 )
            {
                temp_image_byte_ptr = temp_image_rgbaz88;
            }
            else if ( pixel_ID == ID_RGBAZ96 )
            {
                temp_image_byte_ptr = temp_image_rgbaz96;
            }
            //=========================================

            bswap_rgbaz_BYTE ( my_rank, nnodes, width, height, pixel_ID,
                               my_image_byte, &comp_image_byte, &bs_offset, &bs_counts, MPI_COMM_234BS );

            // ============ Final Image Gathering ==============
#ifdef _NOGATHER
            // NO FINAL IMAGE GATHERING

#elif _GATHERV

            // ============ (BEGIN)  MPI_Gatherv ===============
            bs_counts *= global_image_type; // 8, 11 or 12 BYTES
            bs_offset *= global_image_type; // 8, 11 or 12 BYTES

#ifdef _GATHER_TWICE
            MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );
            MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );

            MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                          temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset,
                          MPI_BYTE, ROOT_NODE, MPI_COMM_234BS );
#else
            counts_offset[0] = bs_counts;
            counts_offset[1] = bs_offset;

            MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT,
                        ROOT_NODE, MPI_COMM_234BS );

            bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
            bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
            bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

            for (counter = 0; counter < nnodes; counter++ ) {
                *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
            }

            MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                          temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset,
                          MPI_BYTE, ROOT_NODE, MPI_COMM_234BS );
#endif
            // ============== (END) MPI_Gatherv ===============

#else // #elif _GATHERV

            // ============ (BEGIN) MPI_Gather ================
            bs_counts *= global_image_type; // 8, 11 or 12 BYTES

            // Gather the composited partial images to TEMP_IMAGE
            // since its size is larger than initial IMAGE_BUFFER
            MPI_Gather ( comp_image_byte, bs_counts, MPI_BYTE,
                         temp_image_byte_ptr, bs_counts, MPI_BYTE, ROOT_NODE, MPI_COMM_BITREV );
            // =============== (END) MPI_Gather ===============

#endif // #ifdef _NOGATHER

        }
        else
        {
            printf ("MPI Rank [%d of %d]: Undefined Pixel ID !!! \n", my_rank, nnodes );
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else if (( is_power_of_two == false ) && ( nnodes > 4 ))
    {
        // ====================================================================
        //  234 Composition
        // ====================================================================

        if (( pixel_ID == ID_RGBA32 ) || ( pixel_ID == ID_RGBA56 ) || ( pixel_ID == ID_RGBA64 ))
        {

            //=========================================
            if ( pixel_ID == ID_RGBA32 )
            {
                temp_image_byte_ptr = temp_image_rgba32;
            }
            else if ( pixel_ID == ID_RGBA56 )
            {
                temp_image_byte_ptr = temp_image_rgba56;
            }
            else if ( pixel_ID == ID_RGBA64 )
            {
                temp_image_byte_ptr = temp_image_rgba64;
            }
            //=========================================

            if ( nnodes_234 == 2 )
            {
                partial_bswap2_rgba_BYTE ( my_rank_234, nnodes_234, width, height, global_image_type,
                                           my_image_byte, temp_image_byte_ptr, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 3 )
            {
                partial_bswap3_rgba_BYTE ( my_rank_234, nnodes_234, width, height, global_image_type,
                                           my_image_byte, temp_image_byte_ptr, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 4 )
            {
                partial_bswap4_rgba_BYTE ( my_rank_234, nnodes_234, width, height, global_image_type,
                                           my_image_byte, temp_image_byte_ptr, MPI_COMM_234 );
            }
            else
            {
                printf ("[%d of %d] WRONG DECOMPOSITION SIZE !!! \n", my_rank, nnodes );
                MPI_Finalize();
            }

            // ================= Stage 2 Binary-Swap ==================
            if (( stage2_bswap_my_rank >= 0 ) && ( stage2_bswap_my_rank < stage2_bswap_nnodes))
            {

                stage2_bswap_rgba_BYTE ( stage2_bswap_my_rank, stage2_bswap_nnodes, width, height, pixel_ID,
                                         my_image_byte, &comp_image_byte, &bs_offset, &bs_counts, MPI_COMM_STAGE2_BSWAP );

                // ============ Final Image Gathering ==============
#ifdef _NOGATHER
                // NO FINAL IMAGE GATHERING

#elif _GATHERV

                // ============ (BEGIN)  MPI_Gatherv ===============
                bs_counts *= global_image_type; // 4, 7 or 8 BYTES
                bs_offset *= global_image_type; // 4, 7 or 8 BYTES

#ifdef _GATHER_TWICE
                MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT,
                             ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
                MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT,
                             ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                              temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset,
                              MPI_BYTE, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#else
                counts_offset[0] = bs_counts;
                counts_offset[1] = bs_offset;

                MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2,
                            MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
                bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
                bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

                for (counter = 0; counter < nnodes; counter++ ) {
                    *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                    *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                }

                MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                              temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset,
                              MPI_BYTE, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#endif
                // ============== (END) MPI_Gatherv ===============

#else // #elif _GATHERV

                // ============ (BEGIN) MPI_Gather ================
                bs_counts *= global_image_type; // 8, 11 or 12 BYTES

                // Gather the composited partial images to TEMP_IMAGE
                // since its size is larger than initial IMAGE_BUFFER
                MPI_Gather ( comp_image_byte, bs_counts, MPI_BYTE,
                             temp_image_byte_ptr, bs_counts, MPI_BYTE, ROOT_NODE, MPI_COMM_STAGE2_BITREV );
                // =============== (END) MPI_Gather ===============

#endif // #ifdef _NOGATHER
            }
        }
        else if (( pixel_ID == ID_RGBAZ64 ) || ( pixel_ID == ID_RGBAZ88 ) || ( pixel_ID == ID_RGBAZ96 ))
        {

            //=========================================
            if ( pixel_ID == ID_RGBAZ64 )
            {
                temp_image_byte_ptr = temp_image_rgbaz64;
            }
            else if ( pixel_ID == ID_RGBAZ88 )
            {
                temp_image_byte_ptr = temp_image_rgbaz88;
            }
            else if ( pixel_ID == ID_RGBAZ96 )
            {
                temp_image_byte_ptr = temp_image_rgbaz96;
            }
            //=========================================

            if ( nnodes_234 == 2 )
            {
                partial_bswap2_rgbaz_BYTE ( my_rank_234, nnodes_234, width, height, global_image_type,
                                            my_image_byte, temp_image_byte_ptr, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 3 )
            {
                partial_bswap3_rgbaz_BYTE ( my_rank_234, nnodes_234, width, height, global_image_type,
                                            my_image_byte, temp_image_byte_ptr, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 4 )
            {
                partial_bswap4_rgbaz_BYTE ( my_rank_234, nnodes_234, width, height, global_image_type,
                                            my_image_byte, temp_image_byte_ptr, MPI_COMM_234 );
            }
            else
            {
                printf ("[%d of [%d] WRONG DECOMPOSITION SIZE !!! \n", my_rank, nnodes );
                MPI_Finalize();
            }

            // ================= Stage 2 Binary-Swap ==================
            if (( stage2_bswap_my_rank >= 0 ) && ( stage2_bswap_my_rank < stage2_bswap_nnodes))
            {

                stage2_bswap_rgbaz_BYTE ( stage2_bswap_my_rank, stage2_bswap_nnodes, width, height, pixel_ID,
                                          my_image_byte, &comp_image_byte, &bs_offset, &bs_counts, MPI_COMM_STAGE2_BSWAP );

                // ============ Final Image Gathering ==============
#ifdef _NOGATHER
                // NO FINAL IMAGE GATHERING

#elif _GATHERV
                bs_counts *= global_image_type;  // 8, 11 or 12 BYTES
                bs_offset *= global_image_type;  // 8, 11 or 12 BYTES

#ifdef _GATHER_TWICE
                MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT,
                             ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
                MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT,
                             ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                              temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset, MPI_BYTE,
                              ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#else
                counts_offset[0] = bs_counts;
                counts_offset[1] = bs_offset;

                MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT,
                            ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
                bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
                bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

                for (counter = 0; counter < nnodes; counter++ ) {
                    *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                    *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                }

                MPI_Gatherv ( comp_image_byte, bs_counts, MPI_BYTE,
                              temp_image_byte_ptr, bs_gatherv_counts, bs_gatherv_offset, MPI_BYTE,
                              ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#endif
                // ============== (END) MPI_Gatherv ===============

#else // #elif _GATHERV

                // ============ (BEGIN) MPI_Gather ================
                bs_counts *= global_image_type; // 8, 11 or 12 BYTES

                // Gather the composited partial images to TEMP_IMAGE
                // since its size is larger than initial IMAGE_BUFFER
                MPI_Gather ( comp_image_byte, bs_counts, MPI_BYTE,
                             temp_image_byte_ptr, bs_counts, MPI_BYTE, ROOT_NODE, MPI_COMM_STAGE2_BITREV );
                // =============== (END) MPI_Gather ===============

#endif // #ifdef _NOGATHER
            }
        }
        else
        {
            printf ("MPI Rank [%d of %d]: Undefined Pixel ID !!! \n", my_rank, nnodes );
            MPI_Finalize();
        }
    }
    else if ( nnodes == 3 )
    {
        // ====================================================================
        //   3-node Composition
        // ====================================================================
        switch ( pixel_ID ) {
        case ID_RGBA32 : dsend3_rgba_BYTE ( my_rank, nnodes, width, height, RGBA32, my_image_byte, temp_image_rgba32, MPI_COMM_234BS );
            break;
        case ID_RGBA56 : dsend3_rgba_BYTE ( my_rank, nnodes, width, height, RGBA56, my_image_byte, temp_image_rgba56, MPI_COMM_234BS );
            break;
        case ID_RGBA64 : dsend3_rgba_BYTE ( my_rank, nnodes, width, height, RGBA64, my_image_byte, temp_image_rgba64, MPI_COMM_234BS );
            break;
        case ID_RGBAZ64: dsend3_rgbaz_BYTE ( my_rank, nnodes, width, height, RGBAZ64, my_image_byte, temp_image_rgbaz64, MPI_COMM_234BS );
            break;
        case ID_RGBAZ88: dsend3_rgbaz_BYTE ( my_rank, nnodes, width, height, RGBAZ88, my_image_byte, temp_image_rgbaz88, MPI_COMM_234BS );
            break;
        case ID_RGBAZ96: dsend3_rgbaz_BYTE ( my_rank, nnodes, width, height, RGBAZ96, my_image_byte, temp_image_rgbaz96, MPI_COMM_234BS );
            break;
        default: printf ("MPI Rank [%d of %d]: Undefined Pixel ID !!! \n", my_rank, nnodes );
            MPI_Finalize();
            break;
        }
    }
    else if ( nnodes == 1 )
    {
        if ( my_rank == ROOT_NODE )
        {
            printf("Nothing to do with only ONE IMAGE !!! \n");
        }
    }
    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Initialize variables and image buffer for
 *         234 Image  Compositing
 *
 *  @param  my_rank [in] MPI Rank
 *  @param  nnodes  [in] MPI number of nodes
 *  @param  width   [in] Image width
 *  @param  height  [in] Image size
 *  @param  height  [in] Pixel type
 */
/*========================================================*/
int Init_234Composition_FLOAT ( unsigned int my_rank, unsigned int nnodes,
                                unsigned int width, unsigned int height, unsigned int pixel_ID )
{
    // pixel_ID
    //
    // ID_RGBA32:   RGBA   32-bit
    // ID_RGBA128:  RGBA  128-bit
    // ID_RGBAZ160: RGBAZ160-bit

    // ====================================================================
    unsigned int temp_buffer_size;
    unsigned int i;

    global_num_pixels = width * height;
    global_add_pixels = 0;

#if !defined _GATHERV // MPI_GATHER
    if ( check_pow2 ( nnodes ) == false ) { // Non-power-of-two number of nodes
        i = get_nearest_pow2 ( nnodes );
    }
    else {
        i = nnodes;
    }

    global_mod_pixels = global_num_pixels % i;

    if ( global_mod_pixels != 0 )
    {
        global_add_pixels = i - global_mod_pixels;
        global_num_pixels += global_add_pixels;
    }
#endif

    // Temporary Buffer Size for storing 
    // half of the image image (Binary-Swap)
    temp_buffer_size = 0;
    if ( global_num_pixels <= MPIXELS_8 )
    {
        temp_buffer_size = MPIXELS_8;
    }
    else if ( global_num_pixels <= MPIXELS_16 )
    {
        temp_buffer_size = MPIXELS_16;
    }
    else if ( global_num_pixels <= MPIXELS_32 )
    {
        temp_buffer_size = MPIXELS_32;
    }
    else if ( global_num_pixels <= MPIXELS_64 )
    {
        temp_buffer_size = MPIXELS_64;
    }
    else if ( global_num_pixels <= MPIXELS_128 )
    {
        temp_buffer_size = MPIXELS_128;
    }

    // =======================================
    //   Prepare temporay image buffer
    // =======================================
    if ( pixel_ID == ID_RGBA128 )
    {
        global_image_type = RGBA128;
        global_image_size = global_num_pixels * RGBA; // 4 elements RGBA
        temp_buffer_size *= RGBA; // 4 elements RGBA

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgba128 = (float *)allocate_float_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else if ( pixel_ID == ID_RGBAZ160 )
    {
        global_image_type = RGBAZ160;
        global_image_size = global_num_pixels * RGBAZ; // 5 elements RGBAZ
        temp_buffer_size *= RGBAZ; // 5 elements RGBAZ

        // Temporary Image Buffer ( Partial images for Send and Receive stages)
        if ( ( temp_image_rgbaz160 = (float *)allocate_float_memory_region (
                   (unsigned int)( temp_buffer_size ))) == NULL ) {
            MPI_Finalize();
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf ("Image type is NOT VALID !!!! \n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // ====================================================================

    // =======================================
    //   Prepare lists for MPI_Gatherv
    // =======================================
    if ( ( bs_gatherv_offset = (int *)allocate_int_memory_region (
               (unsigned int)( nnodes ))) == NULL ) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if ( ( bs_gatherv_counts = (int *)allocate_int_memory_region (
               (unsigned int)( nnodes ))) == NULL ) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if ( ( bs_gatherv_counts_offset = (int *)allocate_int_memory_region (
               (unsigned int)( 2 * nnodes ))) == NULL ) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // ====================================================================
    // ====================================================================
    if ( check_pow2 ( nnodes ) == true )
    {
        // =======================================
        //   TRADITIONAL BINARY-SWAP
        // =======================================

        is_power_of_two = true; // Power-of-two number of nodes 

        // =======================================
        //  Prepare bit-reverse order communicator
        //  for MPI_Gather without reconstruction
        // =======================================

        bitrev_my_rank  = bitrevorder ( my_rank, nnodes ); // Get new Rank (Bit-reversed order)
        bitrev_my_group = 1; // Color for new MPI communicator
        // 1 unique color = 1 group of communicator

        // Generate a new MPI communicator with bit-reversed ranks
        MPI_Comm_split ( MPI_COMM_WORLD, bitrev_my_group, bitrev_my_rank, &MPI_COMM_BITREV ); 
        MPI_Comm_rank  ( MPI_COMM_BITREV, (int *)&bitrev_my_rank ); // New Rank inside MPI_COMM_BITREV
        MPI_Comm_size  ( MPI_COMM_BITREV, (int *)&bitrev_nnodes  ); // NUmber of nodes in MPI_COMM_BITREV
    }
    else
    {
        // =======================================
        //  2-3-4 DECOMPOSIITON
        // =======================================
        is_power_of_two = false; // Non-power-of-two number of nodes

        // GROUPS of (2 and 3) or (3 and 4)
        //
        // 2-3 : 2^n < m < 2^n + 2^(n-1)
        // 3-3 : 2^n + 2^(n-1)
        // 3-4 : 2^n + 2^(n-1) < m < 2^(n+1)
        //
        // Example: n = 2
        //          2-3: m = 5 (4 < m < 6)
        //          3-3: m = 6
        //          3-4: m = 7 (6 < m < 8)

        // =======================================
        //  Number of nodes != Power-of-two (2^n)
        // =======================================

        // Obtain the nearest power of two, that is,  2^(n+1)
        // Should be smaller than number of nodes
        near_pow2 = get_nearest_pow2 ( nnodes );

        // Obtain the value of 2^(n)
        ngroups_234 = near_pow2 * 0.5;

        base_234 = floor ( nnodes / ngroups_234 );
        over_234 = nnodes % ngroups_234;

        // Calculate threshold for choosing 2-3 or 3-4
        // 2^(n) + 2^(n-1)
        threshold_234 = nnodes - ( over_234 * ( base_234 + 1 ) );

        if ( my_rank < threshold_234 )
        {
            // 2-3 (GROUPS of 2 and 3)
            my_group_234 = floor ( my_rank / base_234 );
        }
        else
        {
            // 3-4 (GROUPS of 3 and 4)
            my_group_234 = ( ngroups_234 - over_234 ) +	(( my_rank - threshold_234 ) / ( base_234 + 1 ));
        }

        // Generate new MPI communciators for each of the 2-3-4 groups
        MPI_Comm_split ( MPI_COMM_WORLD, my_group_234, my_rank, &MPI_COMM_234 ); 
        MPI_Comm_rank  ( MPI_COMM_234, &my_rank_234 ); 
        MPI_Comm_size  ( MPI_COMM_234, &nnodes_234 ); 

        // =======================================
        // 			2nd stage Binary-Swap
        // =======================================

        // List of nodes (2nd stage Binary-Swap) 
        // Local root node (n) and its neighbor (n+1)
        group_bswap = (int *)malloc( sizeof(int) * ngroups_234 * 2 );
        memset( group_bswap, 0x00, sizeof(int) * ngroups_234 * 2 );
        group_bswap_ptr = group_bswap;

        // Smaller than 2-3-4 decomposition threshold
        // 2 of 2-3 ; 3 of 3-3 ; 3 of 3-4
        for ( i = 0; i < ( ngroups_234 - over_234 ); i++ )
        {
            *group_bswap_ptr++ = (int)(  i * base_234 );
            *group_bswap_ptr++ = (int)(( i * base_234 ) + 1 );
        }

        // Larger than 2-3-4 decomposition threshold  
        // 3 of 2-3 ; 4 of 3-4
        for ( i = 0; i < over_234; i++ )
        {
            *group_bswap_ptr++ = (int)(( ngroups_234 - over_234 ) * base_234 ) + ( i * ( base_234 + 1 ));
            *group_bswap_ptr++ = (int)(( ngroups_234 - over_234 ) * base_234 ) + ( i * ( base_234 + 1 ) + 1 );
        }

        // Generate new MPI communciator for 2nd stage Binary-Swap
        MPI_Comm_group ( MPI_COMM_WORLD, &MPI_GROUP_WORLD ); 
        MPI_Group_incl ( MPI_GROUP_WORLD, ngroups_234 * 2, group_bswap, &MPI_GROUP_STAGE2_BSWAP ); 
        MPI_Comm_create( MPI_COMM_WORLD, MPI_GROUP_STAGE2_BSWAP, &MPI_COMM_STAGE2_BSWAP ); 
        MPI_Group_rank ( MPI_GROUP_STAGE2_BSWAP, &stage2_bswap_my_rank ); 
        MPI_Group_size ( MPI_GROUP_STAGE2_BSWAP, &stage2_bswap_nnodes ); 

        if (( stage2_bswap_my_rank >= 0 ) && ( stage2_bswap_my_rank < stage2_bswap_nnodes)) 
        { 	
            bitrev_my_rank  = bitrevorder ( stage2_bswap_my_rank, stage2_bswap_nnodes ); // Get new Rank (Bit-reversed order)
            bitrev_my_group = 1; 	// Color for new MPI communicator 
            // 1 unique color = 1 group of communicator		

            // Generate a new MPI communicator with bit-reversed ranks
            MPI_Comm_split ( MPI_COMM_STAGE2_BSWAP, bitrev_my_group, bitrev_my_rank, &MPI_COMM_STAGE2_BITREV ); 
            MPI_Comm_rank  ( MPI_COMM_STAGE2_BITREV, &stage2_bitrev_my_rank );	// New Rank inside MPI_COMM_BITREV
            MPI_Comm_size  ( MPI_COMM_STAGE2_BITREV, &stage2_bitrev_nnodes  );	// NUmber of nodes in MPI_COMM_BITREV
        }
    }
    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Destroy variables and image buffer for 
 *	 	   234 Image  Compositing 
 *         
 *  @param  my_rank [in] MPI Rank
 *  @param  nnodes  [in] MPI number of nodes
 *  @param  width   [in] Image width
 *  @param  height  [in] Image size
 *  @param  height  [in] Pixel type 
 */
/*========================================================*/
int Destroy_234Composition_FLOAT ( unsigned int pixel_ID )
{
    // pixel_ID
    //	
    // ID_RGBA128:  RGBA  128-bit	
    // ID_RGBAZ160: RGBAZ 160-bit	
    // ====================================================================

    // =======================================  
    // 		Destroy temporay image buffer
    // =======================================  	
    if ( pixel_ID == ID_RGBA128 ) 
    {
        if ( temp_image_rgba128 )
            free ( temp_image_rgba128 );
    }
    else if ( pixel_ID == ID_RGBAZ160 ) 
    {
        if ( temp_image_rgbaz160 )
            free ( temp_image_rgbaz160 );
    }

    // =======================================  
    // 		Destroy lists for MPI_Gatherv
    // =======================================  
    if ( bs_gatherv_offset )
        free ( bs_gatherv_offset );

    if ( bs_gatherv_counts )
        free ( bs_gatherv_counts );

    if ( bs_gatherv_counts_offset )
        free ( bs_gatherv_counts_offset );

    return EXIT_SUCCESS;
}

/*========================================================*/
/**
 *  @brief Do 234 Composition 
 *		   (Float image pixel: RGBA128, RGBAZ 160)
 *
 *  @param  my_rank        [in]  MPI rank number
 *  @param  nnodes         [in]  MPI number of nodes
 *  @param  width          [in]  Image width
 *  @param  height         [in]  Image height
 *  @param  pixel_ID       [in]  Pixel type
 *  @param  rgba_image     [in,out]  Input and Blended Image
 *  @param  MPI_COMM_234BS [in]  MPI Communicator for 234 + Binary-Swap
 */
/*========================================================*/
int  Do_234Composition_Core_FLOAT ( unsigned int my_rank, unsigned int nnodes, \
				    unsigned int width, unsigned int height, unsigned int pixel_ID, unsigned int merge_ID, \
				    float *my_image_float, MPI_Comm MPI_COMM_234BS )
{

#ifdef _GATHERV
    unsigned int counts_offset[2]; // Pixel Counts and Offset
    unsigned int counter; // Loop counter
#endif

    float *comp_image_float; 
	
    if ( (merge_ID == DEPTH_ROI) || (merge_ID == DEPTH_COMPRESS) ) 
    {
        printf("Unsupported merging option \n");	
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if ( is_power_of_two == true )
    {
        // ====================================================================
        //			TRADITIONAL BINARY-SWAP
        // ====================================================================
        if ( pixel_ID == ID_RGBA128 ) 
        {	
            bswap_rgba128 ( my_rank, nnodes, width, height, global_image_type, \
                            my_image_float, &comp_image_float, &bs_offset, &bs_counts, MPI_COMM_234BS );

            // ============ Final Image Gathering ==============
#ifdef _NOGATHER
            // =========================
            // NO FINAL IMAGE GATHERING
            // =========================
#elif _GATHERV
            // ============ (BEGIN)  MPI_Gatherv =============== 
            bs_counts *= RGBA; // 4 elements
            bs_offset *= RGBA; // 4 elements

#ifdef _GATHER_TWICE
            MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );
            MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );

            MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgba128, \
                          bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_234BS );
#else
            counts_offset[0] = (unsigned int)bs_counts;
            counts_offset[1] = (unsigned int)bs_offset;

            MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT, ROOT_NODE, MPI_COMM_234BS );

            bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
            bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
            bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

            for ( counter = 0; counter < nnodes; counter++ ) {
                *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
            }

            MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgba128, \
                          bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_234BS );
#endif
            // ============== (END) MPI_Gatherv =============== 

#else
            // ============ (BEGIN) MPI_Gather ================ 
            bs_counts *= RGBA; // 4 elements

            // Gather the composited partial images to temp_image_rgba128
            // since its size is larger than  my_image_float
            MPI_Gather ( comp_image_float, bs_counts, MPI_FLOAT, \
                         temp_image_rgba128, bs_counts, MPI_FLOAT, ROOT_NODE, MPI_COMM_BITREV );
            // =============== (END) MPI_Gather ===============
			
#endif // ifndef _GATHERV
        }
        else if ( pixel_ID == ID_RGBAZ160 ) 
        {
            bswap_rgbaz160 ( my_rank, nnodes, width, height, global_image_type, \
                             my_image_float, &comp_image_float, &bs_offset, &bs_counts, MPI_COMM_234BS );

            // ============ Final Image Gathering ==============
#ifdef _NOGATHER
            // NO FINAL IMAGE GATHERING

#elif _GATHERV
            // ============ (BEGIN)  MPI_Gatherv =============== 
            bs_counts *= RGBAZ; // 5 elements
            bs_offset *= RGBAZ; // 5 elements

#ifdef _GATHER_TWICE
            MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );
            MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT, ROOT_NODE, MPI_COMM_234BS );

            MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgbaz160, \
                          bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_234BS );
#else
            counts_offset[0] = bs_counts;
            counts_offset[1] = bs_offset;

            MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT, ROOT_NODE, MPI_COMM_234BS );

            bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
            bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
            bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

            for (counter = 0; counter < nnodes; counter++ ) {
                *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
            }

            MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgbaz160, \
                          bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_234BS );
#endif
            // ============== (END) MPI_Gatherv =============== 

#else
            // ============ (BEGIN) MPI_Gather ================ 
            bs_counts *= RGBAZ; // 5 elements

            // Gather the composited partial images to TEMP_IMAGE
            // since its size is larger than initial IMAGE_BUFFER
            MPI_Gather ( comp_image_float, bs_counts, MPI_FLOAT, \
                         temp_image_rgbaz160, bs_counts, MPI_FLOAT, ROOT_NODE, MPI_COMM_BITREV );

            // =============== (END) MPI_Gather ===============

#endif // ifndef _GATHERV
        }
        else 
        {
            printf ("MPI Rank [%d of %d]: Undefined Pixel ID !!! \n", my_rank, nnodes );
            MPI_Finalize();
        }
    }
    else if (( is_power_of_two == false ) && ( nnodes > 4 ))
    {
        // ====================================================================
        //				  		234 Composition
        // ====================================================================

        if ( pixel_ID == ID_RGBA128 ) 
        {	
            if ( nnodes_234 == 2 ) 
            {
                partial_bswap2_rgba128 ( my_rank_234, nnodes_234, width, height, global_image_type, \
                                         my_image_float, temp_image_rgba128, MPI_COMM_234 );
            }	
            else if ( nnodes_234 == 3 ) 
            {
                partial_bswap3_rgba128 ( my_rank_234, nnodes_234, width, height, global_image_type, \
                                         my_image_float, temp_image_rgba128, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 4 ) 
            {
                partial_bswap4_rgba128 ( my_rank_234, nnodes_234, width, height, global_image_type, \
                                         my_image_float, temp_image_rgba128, MPI_COMM_234 );
            }
            else 
            {
                printf ("[%d of [%d] WRONG DECOMPOSITION SIZE !!! \n", my_rank, nnodes );
                MPI_Finalize();
            }

            // ================= Stage 2 Binary-Swap ================== 
            if (( stage2_bswap_my_rank >= 0 ) && ( stage2_bswap_my_rank < stage2_bswap_nnodes)) 
            { 		
                stage2_bswap_rgba128 ( stage2_bswap_my_rank, stage2_bswap_nnodes, width, height, global_image_type, \
                                       my_image_float, &comp_image_float, &bs_offset, &bs_counts, MPI_COMM_STAGE2_BSWAP );

                // ============ Final Image Gathering ==============
#ifdef _NOGATHER
                // NO FINAL IMAGE GATHERING

#elif _GATHERV

                // ============ (BEGIN)  MPI_Gatherv =============== 
                bs_counts *= RGBA; // 4 elements
                bs_offset *= RGBA; // 4 elements

#ifdef _GATHER_TWICE
                MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
                MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgba128, \
                              bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#else
                counts_offset[0] = bs_counts;
                counts_offset[1] = bs_offset;

                MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
                bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
                bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

                for (counter = 0; counter < nnodes; counter++ ) {
                    *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                    *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                }

                MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgba128, \
                              bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#endif
                // ============== (END) MPI_Gatherv =============== 

#else
                // ============ (BEGIN) MPI_Gather ================ 
                bs_counts *= RGBA; // 4 elements

                // Gather the composited partial images to TEMP_IMAGE
                // since its size is larger than initial IMAGE_BUFFER
                MPI_Gather ( comp_image_float, bs_counts, MPI_FLOAT, \
                             temp_image_rgba128, bs_counts, MPI_FLOAT, ROOT_NODE, MPI_COMM_STAGE2_BITREV );

                // =============== (END) MPI_Gather ===============

#endif
            }					
        }
        else if ( pixel_ID == ID_RGBAZ160 ) 
        {	

            if ( nnodes_234 == 2 ) 
            {
                partial_bswap2_rgbaz160 ( my_rank_234, nnodes_234, width, height, global_image_type, \
                                          my_image_float, temp_image_rgbaz160, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 3 ) 
            {
                partial_bswap3_rgbaz160 ( my_rank_234, nnodes_234, width, height, global_image_type, \
                                          my_image_float, temp_image_rgbaz160, MPI_COMM_234 );
            }
            else if ( nnodes_234 == 4 ) 
            {
                partial_bswap4_rgbaz160 ( my_rank_234, nnodes_234, width, height, global_image_type, \
                                          my_image_float, temp_image_rgbaz160, MPI_COMM_234 );
            }
            else 
            {
                printf ("[%d of [%d] WRONG DECOMPOSITION SIZE !!! \n", my_rank, nnodes );
                MPI_Finalize();
            }

            // ================= Stage 2 Binary-Swap ==============
            if (( stage2_bswap_my_rank >= 0 ) && ( stage2_bswap_my_rank < stage2_bswap_nnodes)) 
            { 		

                stage2_bswap_rgbaz160 ( stage2_bswap_my_rank, stage2_bswap_nnodes, width, height, global_image_type, \
                                        my_image_float, &comp_image_float, &bs_offset, &bs_counts, MPI_COMM_STAGE2_BSWAP );

                // ============ Final Image Gathering ==============
#ifdef _NOGATHER
                // NO FINAL IMAGE GATHERING

#elif _GATHERV

                // ============ (BEGIN)  MPI_Gatherv =============== 
                bs_counts *= RGBAZ; // 5 elements
                bs_offset *= RGBAZ; // 5 elements

#ifdef _GATHER_TWICE
                MPI_Gather ( (void *)&bs_offset, 1, MPI_INT, bs_gatherv_offset, 1, MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
                MPI_Gather ( (void *)&bs_counts, 1, MPI_INT, bs_gatherv_counts, 1, MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgbaz160, \
                              bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#else
                counts_offset[0] = bs_counts;
                counts_offset[1] = bs_offset;

                MPI_Gather( (unsigned int *)counts_offset, 2, MPI_INT, bs_gatherv_counts_offset, 2, MPI_INT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );

                bs_gatherv_counts_offset_ptr = (int *)bs_gatherv_counts_offset;
                bs_gatherv_counts_ptr = (int *)bs_gatherv_counts;
                bs_gatherv_offset_ptr = (int *)bs_gatherv_offset;

                for (counter = 0; counter < nnodes; counter++ ) {
                    *bs_gatherv_counts_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                    *bs_gatherv_offset_ptr++ = (int)*bs_gatherv_counts_offset_ptr++;
                }

                MPI_Gatherv ( comp_image_float, bs_counts, MPI_FLOAT, temp_image_rgbaz160, \
                              bs_gatherv_counts, bs_gatherv_offset, MPI_FLOAT, ROOT_NODE, MPI_COMM_STAGE2_BSWAP );
#endif
                // ============== (END) MPI_Gatherv =============== 

#else
                // ============ (BEGIN) MPI_Gather ================ 
                bs_counts *= RGBAZ; // 5 elements

                // Gather the composited partial images to TEMP_IMAGE
                // since its size is larger than initial IMAGE_BUFFER
                MPI_Gather ( comp_image_float, bs_counts, MPI_FLOAT, \
                             temp_image_rgbaz160, bs_counts, MPI_FLOAT, ROOT_NODE, MPI_COMM_STAGE2_BITREV );
                // =============== (END) MPI_Gather ===============
#endif
            }					
        }
    }
    else if ( nnodes == 3 )
    {  

        // ====================================================================
        //		  		3-node Composition
        // ====================================================================
        if ( pixel_ID == ID_RGBA128 ) 
        {
            dsend3_rgba128 ( my_rank, nnodes, width, height, RGBA128, my_image_float, temp_image_rgba128, MPI_COMM_234BS );
        }
        else if ( pixel_ID == ID_RGBAZ160 ) 
        {
            dsend3_rgbaz160 ( my_rank, nnodes, width, height, RGBAZ160, my_image_float, temp_image_rgbaz160, MPI_COMM_234BS );
        }
        else 
        {
            printf ("MPI Rank [%d of %d]: Undefined Pixel ID !!! \n", my_rank, nnodes );
            MPI_Finalize();
        }
    }
    else if ( nnodes == 1 )
    {
        if ( my_rank == ROOT_NODE )
        {
            printf("Nothing to do with only ONE IMAGE !!! \n");
        }
    }

    return EXIT_SUCCESS;
}

