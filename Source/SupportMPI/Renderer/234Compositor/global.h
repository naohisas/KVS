//#pragma once
#include "typedef.h"
#include <mpi.h>


// ======================================
//	    VARIABLES (Image Data ) 
// ======================================
BYTE *temp_image_rgba32;		// Temporary Image Data (RGBA32 Pixels)
BYTE *temp_image_rgba32_ptr;	// Pointer for the Temporary Image Data (RGBA32 Pixels)

BYTE *temp_image_rgba56;		// Temporary Image Data (RGBA56 Pixels)
BYTE *temp_image_rgba56_ptr;	// Pointer for the Temporary Image Data (RGBA56 Pixels)

BYTE *temp_image_rgba64;		// Temporary Image Data (RGBA64 Pixels)
BYTE *temp_image_rgba64_ptr;	// Pointer for the Temporary Image Data (RGBA64 Pixels)

BYTE *temp_image_rgbaz64;		// Temporary Image Data (RGBAZ64 Pixels)
BYTE *temp_image_rgbaz64_ptr;	// Pointer for the Temporary Image Data (RGBAZ64 Pixels)

BYTE *temp_image_rgbaz88;		// Temporary Image Data (RGBAZ88 Pixels)
BYTE *temp_image_rgbaz88_ptr;	// Pointer for the Temporary Image Data (RGBAZ88 Pixels)

BYTE *temp_image_rgbaz96;		// Temporary Image Data (RGBAZ96 Pixels)
BYTE *temp_image_rgbaz96_ptr;	// Pointer for the Temporary Image Data (RGBAZ96 Pixels)

BYTE *temp_image_byte_ptr;		// Pointer for the Temporary Image Data (BYTE)

float *temp_image_rgba128;		// Temporary Image Data (RGBA128 Pixels)
float *temp_image_rgba128_ptr;	// Pointer for the Temporary Image Data (RGBA128 Pixels)

float *temp_image_rgbaz160;		// Temporary Image Data (RGBAZ160 Pixels)
float *temp_image_rgbaz160_ptr;	// Pointer for the Temporary Image Data (RGBAZ160 Pixels)

float *temp_image_float_ptr;	// Pointer for the Temporary Image Data (FLOAT) 

unsigned int global_width;		// Image width
unsigned int global_height;		// Image height

unsigned int global_num_pixels;	// Number of pixels ( Image size )
unsigned int global_image_size;	// image size ( Number of pixels * image_type )

unsigned int global_mod_pixels; // Remainder pixels (Number of pixels mod Number of nodes)
unsigned int global_add_pixels; // Added pixels to complete a divisible number of pixels

unsigned int global_image_type;	// Image type ( RGBA32, RGBAZ_64, RGBA128, RGBAZ160 )
unsigned int pixel_ID;			// pixel ID (ID_RGBA32, ID_RGBAZ64, ID_RGBA128, ID_RGBAZ160)

// ======================================
//	    VARIABLES ( MPI related ) 
// ======================================
MPI_Status  global_status;	// Status object for MPI_Recv
MPI_Request global_isend;	// ISend request parameter
MPI_Request global_irecv;	// IRecv request parameter

//_Bool is_power_of_two; 		// Check wether is power-of-two (2^n)
bool is_power_of_two; 		// Check wether is power-of-two (2^n)

// ======================================
//	    TRADITIONAL GATHERV 
//	 Final image gathering (MPI_Gatherv)
// ======================================
unsigned int bs_counts;			// Data counts information for MPI_Gatherv				
unsigned int bs_offset;			// Offset information for MPI_Gatherv

int *bs_gatherv_offset;			// List of offset data for MPI_Gatherv
int *bs_gatherv_counts;			// List of data counts information for MPI_Gatherv 
int *bs_gatherv_counts_offset;		// List of data counts information for MPI_Gatherv 

int *bs_gatherv_offset_ptr;		// Pointer for bs_gatherv_offset
int *bs_gatherv_counts_ptr;		// Pointer for bs_gatherv_counts
int *bs_gatherv_counts_offset_ptr;	// Pointer for bs_gatherv_counts

// ======================================
//		    BIT-REVERSAL 
//	 Final image gathering (MPI_Gather)
// ======================================

MPI_Comm MPI_COMM_BITREV; 	// MPI Communicator (Bit-reversal rank order)

int bitrev_my_rank;		// My Rank (MPI_COMM_BITREV)
int bitrev_nnodes;		// Num Nodes (MPI_COMM_BITREV)

int bitrev_my_group;		// Group(Color) for MPI_Comm_split

//==========================
// Not even image size
//==========================
BYTE* send_byte_pixel_ptr;	// Pointer for remaining pixels (BYTE) to be sent	
BYTE* recv_byte_pixel_ptr;	// Pointer to receive remaining pixels (BYTE) 

float* send_float_pixel_ptr;	// Pointer for remaining pixels (float) to be sent	
float* recv_float_pixel_ptr;	// Pointer to receive remaining pixels (float) 

// ======================================
//	    2-3-4 Decomposition
// ======================================

unsigned int near_pow2;		// Nearest power of two smaller than total number of nodes

unsigned int ngroups_234;	// Number of groups
unsigned int base_234;		// Base for 2-3-4 Decomposition
unsigned int over_234;		// Nodes over threshold
unsigned int threshold_234;	// Threshold for 2-3-4 Decomposition (3)

//==========================
MPI_Comm MPI_COMM_234; 		// MPI Communicator (Groups of 2, 3 or 4)

int my_rank_234;		// My Rank (Groups of 2, 3 or 4)
int nnodes_234;			// Num Nodes (Groups of 2, 3 or 4)

unsigned int my_group_234;	// Group(Color) for MPI_Comm_split
//==========================

// ======================================
//	    2nd stage Binary-Swap
// ======================================
int *group_bswap;			// List of nodes (2nd stage Binary-Swap) 
int *group_bswap_ptr;			// Pointer for group_bswap

MPI_Group MPI_GROUP_WORLD;		// Group (Entire nodes)
MPI_Group MPI_GROUP_STAGE2_BSWAP;	// Group (2nd stage Binary-Swap)

//==========================
MPI_Comm  MPI_COMM_STAGE2_BSWAP;	// MPI Communicator (2nd stage Binary-Swap)

int stage2_bswap_my_rank;		// My Rank (2nd stage Binary-Swap)
int stage2_bswap_nnodes; ;		// Num Nodes (2nd stage Binary-Swap)

MPI_Comm  MPI_COMM_STAGE2_BITREV;	// MPI Communicator (2nd stage Bit-Reversed Binary-Swap)

int stage2_bitrev_my_rank;		// My Rank (2nd stage Binary-Swap)
int stage2_bitrev_nnodes; ;		// Num Nodes (2nd stage Binary-Swap)
//==========================

// ======================================
//	    BIT-REVERSAL  (for MPI_Gather)
// ======================================
MPI_Comm MPI_COMM_MSTEP1_BITREV; // MPI Communicator (Step 1 w/ Bit-reversal rank order)
MPI_Comm MPI_COMM_MSTEP2_BITREV; // MPI Communicator (Step 2 w/ Bit-reversal rank order)

int bitrev_mstep1_my_rank;	// My Rank   (MPI_COMM_MSTEP1_BITREV)
int bitrev_mstep1_nnodes;	// Num Nodes (MPI_COMM_MSTEP1_BITREV)

int bitrev_mstep2_my_rank;	// My Rank   (MPI_COMM_MSTEP2_BITREV)
int bitrev_mstep2_nnodes;	// Num Nodes (MPI_COMM_MSTEP2_BITREV)

int bitrev_mstep1_my_group;	// Group(Color) for MPI_Comm_split
int bitrev_mstep2_my_group;	// Group(Color) for MPI_Comm_split


unsigned int global_my_rank;
unsigned int global_nnodes;

// ========= ALPHA BLENDING LOOK UP TABLE ========= //
BYTE LUT_Mult[ 256 * 256 ]; /**< Product Lookup Table: (255 - Alpha) * Color */ 
BYTE LUT_Sat [ 512 ];       /**< Saturation Lookup Table: 255 if Color > 255 */ 
//=====================================
