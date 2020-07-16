#pragma once

// ======================================
//          CONSTANTS (Image Data ) 
// ======================================

#define MPIXELS_4         4194304       //   4096 x 1024 equivalent (  4 Mega Pixels)
#define MPIXELS_8         8388608       //   8192 x 1024 equivalent (  8 Mega Pixels)
#define MPIXELS_16       16777216       //  16384 x 1024 equivalent ( 16 Mega Pixels)
#define MPIXELS_32       33554432       //  32768 x 1024 equivalent ( 32 Mega Pixels)
#define MPIXELS_64       67108864       //  65536 X 1024 equivalent ( 64 Mega Pixels)
#define MPIXELS_128     134217728       // 131072 X 1024 equivalent (128 Mega Pixels)

#define ID_RGBA32               0       // ID for RGBA   32-bit
#define ID_RGBAZ64              1       // ID for RGBAZ  64-bit
#define ID_RGBA128              2       // ID for RGBA  128-bit
#define ID_RGBAZ160             3       // ID for RGBAZ 160-bit
#define ID_RGBA56               4       // ID for RGBA   56-bit
#define ID_RGBAZ88              5       // ID for RGBAZ  88-bit
#define ID_RGBA64               6       // ID for RGBA   64-bit
#define ID_RGBAZ96              7       // ID for RGBAZ  96-bit

#define RGBA32                  4       //  4 Bytes ( Byte  RGBA : 4 * 1 Byte  )
#define RGBAZ64                 8       //  8 Bytes ( Byte  RGBA Float Z: 4 * 1 Byte + 1 * 4 Bytes )
#define RGBA128                 16      // 16 Bytes ( Float RGBA : 4 * 4 Bytes )
#define RGBAZ160                20      // 20 Bytes ( Float RGBAZ: 5 * 4 Bytes )
#define RGBA56                  7       //  7 Bytes ( Byte  RGB   Float A : 3 * 1 Byte + 1 * 4 Bytes )
#define RGBAZ88                 11      // 11 Bytes ( Byte  RGB   Float AZ: 3 * 1 Byte + 2 * 4 Bytes )
#define RGBA64                  8       //  8 Bytes ( Byte  RGBX  Float A : 4 * 1 Byte + 1 * 4 Bytes )
#define RGBAZ96                 12      // 12 Bytes ( Byte  RGBX  Float AZ: 4 * 1 Byte + 2 * 4 Bytes )

#define RGB                     3       // 3 Components ( R, G and B )
#define RGBA                    4       // 4 Components ( R, G, B and A )
#define RGBAZ                   5       // 5 Components ( R, G, B, A and Z )
#define RGBA2                   8       // Equivalent to 2 RGBA ( 4-byte RGBA + 4-byte Z )

#define LEFT                    0       // Node on the left side
#define RIGHT                   1       // Node on the right side

#define ALPHA_BtoF              -1      // Alpha-blending (OLD)
#define ALPHA                   0       // Alpha-blending
#define DEPTH                   1       // Z-depth Sorting
#define ALPHA_ROI               2       // Alpha-blending with ROI 
#define DEPTH_ROI               3       // Z-depth Sorting with ROI
#define ALPHA_COMPRESS          4       // Alpha-blending with COMPRESSION
#define DEPTH_COMPRESS          5       // Z-depth Sorting with COMPRESSION
// Other Pixel Merging Modes


// ======================================
//      CONSTANTS (MPI related)
// ======================================

#define ROOT_NODE       0       // Master Node's MPI Rank 

#define PAIR_TAG        100     // MPI Communicator Tag
#define SEND_TAG        101     // MPI Communicator Tag
#define RECV_TAG        102     // MPI Communicator Tag

// 3-Node Composition
#define PAIR_02_TAG     103     // MPI Communicator Tag ( Nodes 0 and 2 )
#define PAIR_12_TAG     104     // MPI Communicator Tag ( Nodes 1 and 2 )
#define PAIR_13_TAG     105     // MPI Communicator Tag ( Nodes 1 and 3 )
