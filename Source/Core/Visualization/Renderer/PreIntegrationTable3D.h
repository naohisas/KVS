/*****************************************************************************/
/**
 *  @file   PreIntegrationTable3D.h
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
/*----------------------------------------------------------------------------
 *
 * Acknowledgements:
 * Original source code was developped by University of Utah and can be
 * downloaded at http://havs.sourceforge.net/
 */
/*****************************************************************************/
#ifndef KVS__PRE_INTEGRATION_TABLE_3D_H_INCLUDE
#define KVS__PRE_INTEGRATION_TABLE_3D_H_INCLUDE

#include <kvs/Vector4>
#include <kvs/ValueArray>
#include <kvs/TransferFunction>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PreItegrationTable3D class.
 */
/*===========================================================================*/
class PreIntegrationTable3D
{
private:

    kvs::ValueArray<kvs::Real32> m_transfer_function; ///< serialized transfer function
    kvs::ValueArray<kvs::Real32> m_table; ///< 3D pre-integration table
    size_t m_scalar_resolution; ///< resolution of the scalar axis
    size_t m_depth_resolution; ///< resolution of the depth axis

public:

    PreIntegrationTable3D();
    PreIntegrationTable3D( const size_t scalar_resolution, const size_t depth_resolution );

    size_t scalarResolution() const { return m_scalar_resolution; }
    size_t depthResolution() const { return m_depth_resolution; }
    const kvs::Vector3ui resolution() const { return kvs::Vector3ui( m_scalar_resolution, m_scalar_resolution, m_depth_resolution); }
    const kvs::ValueArray<kvs::Real32>& table() const { return m_table; }

    void setScalarResolution( const size_t scalar_resolution ) { m_scalar_resolution = scalar_resolution; }
    void setDepthResolution( const size_t depth_resolution ) { m_depth_resolution = depth_resolution; }
    void setTransferFunction( const kvs::TransferFunction& transfer_function, const float min_scalar, const float max_scalar );

    void create( const float max_size_of_cell );

private:

    void compute_exact_level( float* slice0, const float dl );
    void compute_incremental_level( float* slice, const float* slicep, const float* slice0, const float l, const float dl );
};

} // end of namespace kvs

#endif // KVS__PRE_INTEGRATION_TABLE_3D_H_INCLUDE
