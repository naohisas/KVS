/*****************************************************************************/
/**
 *  @file   ImageCompositor.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <kvs/mpi/Communicator>
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

namespace mpi
{

/*===========================================================================*/
/**
 *  @brief  Image composition class.
 */
/*===========================================================================*/
class ImageCompositor
{
private:
    int m_rank = 0; ///< MPI rank (my rank)
    int m_size = 0; ///< MPI size (number of nodes)
    MPI_Comm m_comm = 0; ///< MPI communicator
    size_t m_width = 0; ///< image width
    size_t m_height = 0; ///< image height
    unsigned int m_pixel_type = 0; ///< pixel type (RGBA 32-bit or RGBA-Z 64-bit)
    unsigned int m_merge_type = 0; ///< merge type (depth-testing or alpha-blending)

public:
    ImageCompositor( const int rank, const int size, const MPI_Comm comm = MPI_COMM_WORLD );
    ImageCompositor( const kvs::mpi::Communicator& comm );
    ~ImageCompositor();

    bool initialize( const size_t width, const size_t height, const bool enable_depth_testing = false );
    bool destroy();
    bool run( kvs::ValueArray<kvs::UInt8>& color_buffer );
    bool run( kvs::ValueArray<kvs::UInt8>& color_buffer, const kvs::Real32 depth, const bool btof = true );
    bool run( kvs::ValueArray<kvs::UInt8>& color_buffer, kvs::ValueArray<kvs::Real32>& depth_buffer );
};

} // end of namespace mpi

} // end of namespace kvs
