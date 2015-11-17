/****************************************************************************/
/**
 *  @file   GridBase.cpp
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
/****************************************************************************/
#include "GridBase.h"
#include <kvs/Type>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Gets values and differential values.
 *  @param  volume [in] input volume
 *  @param  indices [in] indices of the grid points
 *  @param  values [out] values
 */
/*===========================================================================*/
template <typename ValueType>
inline void Bind(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Vec3ui& base_index,
    kvs::Real32* values )
{
    const size_t line_size = volume->numberOfNodesPerLine();
    const size_t slice_size = volume->numberOfNodesPerSlice();
    kvs::UInt32 index[8];
    index[0] = base_index.x() + base_index.y() * line_size + base_index.z() * slice_size;
    index[1] = index[0] + 1;
    index[2] = index[1] + line_size;
    index[3] = index[0] + line_size;
    index[4] = index[0] + slice_size;
    index[5] = index[1] + slice_size;
    index[6] = index[2] + slice_size;
    index[7] = index[3] + slice_size;

    const size_t nnodes = 8;
    const size_t veclen = volume->veclen();
    const ValueType* const S = volume->values().asValueArray<ValueType>().data();
    for ( size_t i = 0; i < veclen; i++ )
    {
        values[ i * nnodes + 0 ] = kvs::Real32( S[ index[0] * veclen + i ] );
        values[ i * nnodes + 1 ] = kvs::Real32( S[ index[1] * veclen + i ] );
        values[ i * nnodes + 2 ] = kvs::Real32( S[ index[2] * veclen + i ] );
        values[ i * nnodes + 3 ] = kvs::Real32( S[ index[3] * veclen + i ] );
        values[ i * nnodes + 4 ] = kvs::Real32( S[ index[4] * veclen + i ] );
        values[ i * nnodes + 5 ] = kvs::Real32( S[ index[5] * veclen + i ] );
        values[ i * nnodes + 6 ] = kvs::Real32( S[ index[6] * veclen + i ] );
        values[ i * nnodes + 7 ] = kvs::Real32( S[ index[7] * veclen + i ] );
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new GridBase class.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
GridBase::GridBase( const kvs::StructuredVolumeObject* volume ):
    m_nnodes( 8 ),
    m_veclen( volume->veclen() ),
    m_base_index( kvs::Vec3ui::Zero() ),
    m_local_point( kvs::Vec3::Zero() ),
    m_reference_volume( volume )
{
    const size_t dimension = 3;
    const size_t nnodes = m_nnodes;
    const size_t veclen = m_veclen;
    try
    {
        m_values = new kvs::Real32 [nnodes * veclen];
        if ( !m_values ) throw "Cannot allocate memory for 'm_values'";
        memset( m_values, 0, sizeof( kvs::Real32 ) * nnodes * veclen );

        m_interpolation_functions = new kvs::Real32 [nnodes];
        if ( !m_interpolation_functions ) throw "Cannot allocate memory for 'm_interpolation_functions'";
        memset( m_interpolation_functions, 0, sizeof( kvs::Real32 ) * nnodes );

        m_differential_functions = new kvs::Real32 [nnodes * dimension];
        if ( !m_differential_functions ) throw "Cannot allocate memory for 'm_differential_functions'";
        memset( m_differential_functions, 0, sizeof( kvs::Real32 ) * nnodes * dimension );
    }
    catch ( char* error_message )
    {
        kvsMessageError( error_message );
        return;
    }
}

GridBase::~GridBase()
{
    if ( m_values ) delete [] m_values;
    if ( m_interpolation_functions ) delete m_interpolation_functions;
    if ( m_differential_functions ) delete m_differential_functions;
}

void GridBase::bind( const kvs::Vec3ui& base_index )
{
    KVS_ASSERT( base_index.x() < m_reference_volume->resolution().x() - 1 );
    KVS_ASSERT( base_index.y() < m_reference_volume->resolution().y() - 1 );
    KVS_ASSERT( base_index.z() < m_reference_volume->resolution().z() - 1 );

    m_base_index = base_index;
    const kvs::StructuredVolumeObject* volume = m_reference_volume;
    switch ( volume->values().typeID() )
    {
    case kvs::Type::TypeInt8:   ::Bind<kvs::Int8>( volume, base_index, m_values ); break;
    case kvs::Type::TypeUInt8:  ::Bind<kvs::UInt8>( volume, base_index, m_values ); break;
    case kvs::Type::TypeInt16:  ::Bind<kvs::Int16>( volume, base_index, m_values ); break;
    case kvs::Type::TypeUInt16: ::Bind<kvs::UInt16>( volume, base_index, m_values ); break;
    case kvs::Type::TypeInt32:  ::Bind<kvs::Int32>( volume,  base_index, m_values ); break;
    case kvs::Type::TypeUInt32: ::Bind<kvs::UInt32>( volume, base_index, m_values ); break;
    case kvs::Type::TypeInt64:  ::Bind<kvs::Int64>( volume, base_index, m_values ); break;
    case kvs::Type::TypeUInt64: ::Bind<kvs::UInt64>( volume, base_index, m_values ); break;
    case kvs::Type::TypeReal32: ::Bind<kvs::Real32>( volume, base_index, m_values ); break;
    case kvs::Type::TypeReal64: ::Bind<kvs::Real64>( volume, base_index, m_values ); break;
    default: break;
    }
}

void GridBase::bind( const kvs::UInt32 grid_index )
{
    this->bind( this->baseIndexOf( grid_index ) );
}

void GridBase::bind( const kvs::Vec3& global )
{
    const int grid_index = this->findGrid( global );
    KVS_ASSERT( grid_index >= 0 );
    this->bind( grid_index );
    this->setLocalPoint( this->globalToLocal( global ) );
}

void GridBase::setLocalPoint( const kvs::Vec3& local )
{
    m_local_point = local;
    this->updateInterpolationFunctions( local );
    this->updateDifferentialFunctions( local );
}

void GridBase::updateInterpolationFunctions( const kvs::Vec3& local )
{
    const float p = local.x();
    const float q = local.y();
    const float r = local.z();

    const float pq = p * q;
    const float qr = q * r;
    const float rp = r * p;
    const float pqr = pq * r;

    kvs::Real32* N = m_interpolation_functions;
    N[0] = 1.0f - p - q - r + pq + qr + rp - pqr;
    N[1] = p - pq - rp + pqr;
    N[2] = pq - pqr;
    N[3] = q - pq - qr + pqr;
    N[4] = r - rp - qr + pqr;
    N[5] = rp - pqr;
    N[6] = pqr;
    N[7] = qr - pqr;
}

void GridBase::updateDifferentialFunctions( const kvs::Vec3& local )
{
    const float p = local.x();
    const float q = local.y();
    const float r = local.z();
    const float pq = p * q;
    const float qr = q * r;
    const float rp = r * p;

    const size_t nnodes = m_nnodes;
    kvs::Real32* dN = m_differential_functions;
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dNdp + nnodes;
    kvs::Real32* dNdr = dNdq + nnodes;

    dNdp[0] =  - 1.0f + q +r - qr;
    dNdp[1] =  1.0f - q - r + qr;
    dNdp[2] =  q - qr;
    dNdp[3] =  - q + qr;
    dNdp[4] =  - r + qr;
    dNdp[5] =  r - qr;
    dNdp[6] =  qr;
    dNdp[7] =  - qr;

    dNdq[0] =  - 1.0f + p + r - rp;
    dNdq[1] =  - p + rp;
    dNdq[2] =  p - rp;
    dNdq[3] =  1.0f - p - r + rp;
    dNdq[4] =  - r + rp;
    dNdq[5] =  - rp;
    dNdq[6] =  rp;
    dNdq[7] =  r - rp;

    dNdr[0] =  - 1.0f + q + p - pq;
    dNdr[1] =  - p + pq;
    dNdr[2] =  - pq;
    dNdr[3] =  - q + pq;
    dNdr[4] =  1.0f - q - p + pq;
    dNdr[5] =  p - pq;
    dNdr[6] =  pq;
    dNdr[7] =  q - pq;
}

kvs::Real32 GridBase::scalar() const
{
    KVS_ASSERT( m_veclen == 1 );

    const kvs::Real32* N = m_interpolation_functions;
    const kvs::Real32* S = m_values;
    return this->interpolateValue( S, N );
}

kvs::Vec3 GridBase::vector() const
{
    KVS_ASSERT( m_veclen == 3 );

    const kvs::Real32* N = m_interpolation_functions;
    const kvs::Real32* Sx = m_values;
    const kvs::Real32* Sy = Sx + m_nnodes;
    const kvs::Real32* Sz = Sy + m_nnodes;
    const kvs::Real32 x = this->interpolateValue( Sx, N );
    const kvs::Real32 y = this->interpolateValue( Sy, N );
    const kvs::Real32 z = this->interpolateValue( Sz, N );
    return kvs::Vec3( x, y, z );
}

kvs::Vec3 GridBase::gradientVector() const
{
    KVS_ASSERT( m_veclen == 1 );

    const kvs::UInt32 nnodes = m_nnodes;
    const kvs::Real32* dN = m_differential_functions;
    const kvs::Real32* dNdp = dN;
    const kvs::Real32* dNdq = dNdp + nnodes;
    const kvs::Real32* dNdr = dNdq + nnodes;

    const kvs::Real32* S = m_values;
    const kvs::Real32 dSdx = this->interpolateValue( S, dNdp );
    const kvs::Real32 dSdy = this->interpolateValue( S, dNdq );
    const kvs::Real32 dSdz = this->interpolateValue( S, dNdr );
    return kvs::Vec3( -dSdx, -dSdy, -dSdz );
}

kvs::UInt32 GridBase::gridIndexOf( const kvs::Vec3ui& base_index ) const
{
//    const kvs::UInt32 line_size = m_reference_volume->numberOfNodesPerLine();
//    const kvs::UInt32 slice_size = m_reference_volume->numberOfNodesPerSlice();
//    return base_index.x() + base_index.y() * line_size + base_index.z() * slice_size;
    const size_t dimx = m_reference_volume->resolution().x() - 1;
    const size_t dimy = m_reference_volume->resolution().y() - 1;
    return base_index.x() + base_index.y() * dimx + base_index.z() * dimx * dimy;
}

kvs::Vec3ui GridBase::baseIndexOf( const kvs::UInt32 grid_index ) const
{
//    const size_t dimx = m_reference_volume->resolution().x();
//    const size_t dimy = m_reference_volume->resolution().y();
    const size_t dimx = m_reference_volume->resolution().x() - 1;
    const size_t dimy = m_reference_volume->resolution().y() - 1;
    const size_t dimxy = dimx * dimy;
    const size_t i = grid_index % dimxy % dimx;
    const size_t j = grid_index % dimxy / dimx;
    const size_t k = grid_index / dimxy;
    return kvs::Vec3ui( i, j, k );
}

kvs::Real32 GridBase::interpolateValue( const kvs::Real32* values, const kvs::Real32* weights ) const
{
    kvs::Real32 value = 0;
    for ( size_t i = 0; i < m_nnodes; i++ ) { value += weights[i] * values[i]; }
    return value;
}

} // end of namespace kvs
