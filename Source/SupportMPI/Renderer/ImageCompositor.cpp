/*****************************************************************************/
/**
 *  @file   ImageCompositor.cpp
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
#include "ImageCompositor.h"
#define OMPI_SKIP_MPICXX 1
#define MPICH_SKIP_MPICXX 1
#define MPI_NO_CPPBIND 1
#include "./234Compositor/234compositor.h"
#include <kvs/Assert>
#include <utility>


namespace kvs
{

namespace mpi
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ImageCompositor class.
 *  @param  rank [in] MPI rank
 *  @param  size [in] number of MPI processes
 *  @param  comm [in] MPI communicator
 */
/*===========================================================================*/
ImageCompositor::ImageCompositor(
    const int rank,
    const int size,
    const MPI_Comm comm ):
    m_rank( rank ),
    m_size( size ),
    m_comm( comm )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ImageCompositor class.
 *  @param  comm [in] MPI communicator
 */
/*===========================================================================*/
ImageCompositor::ImageCompositor(
    const kvs::mpi::Communicator& comm ):
    m_rank( comm.rank() ),
    m_size( comm.size() ),
    m_comm( comm.handler() )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ImageCompositor class.
 */
/*===========================================================================*/
ImageCompositor::~ImageCompositor()
{
    this->destroy();
}

/*===========================================================================*/
/**
 *  @brief  Initializes the image compositor.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  enable_depth_testing [in] flag for depth testing    
 *  @return true, if the Initialization is done successfully
 */
/*===========================================================================*/
bool ImageCompositor::initialize(
    const size_t width,
    const size_t height,
    const bool enable_depth_testing )
{
    if ( m_width != width || m_height != height ) { this->destroy(); }

    m_width = width;
    m_height = height;
    m_pixel_type = enable_depth_testing ? ID_RGBAZ64 : ID_RGBA32;
    m_merge_type = enable_depth_testing ? DEPTH : ALPHA;
    auto status = Init_234Composition( m_rank, m_size, m_width, m_height, m_pixel_type );
    if ( status == EXIT_FAILURE ) { return false; }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ImageCompositor class.
 */
/*===========================================================================*/
bool ImageCompositor::destroy()
{
    if ( m_width == 0 && m_height == 0 ) { return true; }

    auto status = Destroy_234Composition( m_pixel_type );
    if ( status == EXIT_FAILURE ) { return false; }

    m_width = 0;
    m_height = 0;
    m_pixel_type = -1;
    m_merge_type = -1;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Runs the image compositor w/o depth testing.
 *  @param  color_buffer [in] color buffer
 *  @return true, if the process is done successfully
 */
/*===========================================================================*/
bool ImageCompositor::run( kvs::ValueArray<kvs::UInt8>& color_buffer )
{
    KVS_ASSERT( m_pixel_type == ALPHA );
    KVS_ASSERT( color_buffer.size() == m_width * m_height * 4 );

    auto status = Do_234Composition(
        m_rank, m_size,
        m_width, m_height,
        m_pixel_type, m_merge_type,
        color_buffer.data(),
        m_comm );

    return status == EXIT_SUCCESS;
}

/*===========================================================================*/
/**
 *  @brief  Runs the image compositor w/ sorting.
 *  @param  color_buffer [in] color buffer
 *  @param  depth [in] depth for the color buffer
 *  @param  btof [in] flag for sorting order (if true, back-to-front)
 *  @return true, if the process is done successfully
 */
/*===========================================================================*/
bool ImageCompositor::run(
    kvs::ValueArray<kvs::UInt8>& color_buffer,
    const kvs::Real32 depth,
    const bool btof  )
{
    kvs::mpi::Communicator comm( m_comm );

    // Depth list
    kvs::ValueArray<kvs::Real32> depth_list( comm.size() );
    comm.allGather( depth, depth_list );

    // Rank list (sorted with depth)
    const bool ascending = btof; // ordering inverted???
    auto rank_list = depth_list.argsort( ascending );

    // Iterator (i) to element, which includes my_rank, in rank_list
    const size_t my_rank = static_cast<size_t>( comm.rank() );
    auto i = std::find( rank_list.begin(), rank_list.end(), my_rank );

    // Sort color_buffer in back-to-front order
    const int send_rank = static_cast<int>( std::distance( rank_list.begin(), i ) );
    const int recv_rank = rank_list[ my_rank ];
    if ( int( my_rank ) != send_rank )
    {
        kvs::ValueArray<kvs::UInt8> recv_buffer( m_width * m_height * 4 );
        auto recv_request = comm.immediateReceive( recv_rank, RECV_TAG, recv_buffer );
        auto send_request = comm.immediateSend( send_rank, RECV_TAG, color_buffer );
        recv_request.wait();
        send_request.wait();
        color_buffer = recv_buffer;
    }

    return this->run( color_buffer );
}

/*===========================================================================*/
/**
 *  @brief  Runs the image compositor w/ depth testing.
 *  @param  color_buffer [in] color buffer
 *  @param  depth_buffer [in] depth buffer
 *  @return true, if the process is done successfully
 */
/*===========================================================================*/
bool ImageCompositor::run(
    kvs::ValueArray<kvs::UInt8>& color_buffer,
    kvs::ValueArray<kvs::Real32>& depth_buffer )
{
    KVS_ASSERT( color_buffer.size() == m_width * m_height * 4 );
    KVS_ASSERT( depth_buffer.size() == m_width * m_height );

    auto status = Do_234ZComposition(
        m_rank, m_size,
        m_width, m_height,
        m_pixel_type, m_merge_type,
        color_buffer.data(),
        depth_buffer.data(),
        m_comm );

    return status == EXIT_SUCCESS;
}

} // end of namespace mpi

} // end of namespace kvs
