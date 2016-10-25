/*****************************************************************************/
/**
 *  @file   CellBase.cpp
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
#include "CellBase.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Gets scalar values and vertex coords specified by the given cell index.
 *  @param  volume [in] input volume
 *  @param  index [in] cell index
 *  @param  cell_values [out] values of the cell
 *  @param  cell_coords [out] coords of the cell
 */
/*===========================================================================*/
template <typename ValueType>
inline void Bind(
    const kvs::UnstructuredVolumeObject* volume,
    const size_t index,
    kvs::Real32* cell_values,
    kvs::Vec3* cell_coords )
{
    const size_t nnodes = volume->numberOfCellNodes();
    const size_t veclen = volume->veclen();

    const kvs::UInt32* connections = volume->connections().data() + nnodes * index;
    const kvs::Real32* coords = volume->coords().data();
    const ValueType* values = static_cast<const ValueType*>( volume->values().data() );
    for ( size_t j = 0; j < nnodes; j++ )
    {
        const kvs::UInt32 node_index = *( connections++ );
        for ( size_t i = 0; i < veclen; i++ )
        {
            cell_values[ i * nnodes + j ] = kvs::Real32( values[ veclen * node_index + i ] );
        }

        cell_coords[j] = kvs::Vec3( coords + 3 * node_index );
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellBase class.
 *  @param  volume [in] pointer to the unstructured volume object.
 */
/*===========================================================================*/
CellBase::CellBase(
    const kvs::UnstructuredVolumeObject* volume ):
    m_nnodes( volume->numberOfCellNodes() ),
    m_veclen( volume->veclen() ),
    m_local_point( 0, 0, 0 ),
    m_reference_volume( volume )
{
    const size_t dimension = 3;
    const size_t nnodes = m_nnodes;
    const size_t veclen = m_veclen;
    try
    {
        m_coords = new kvs::Vec3 [nnodes];
        if ( !m_coords ) throw "Cannot allocate memory for 'm_coords'";
        memset( m_coords, 0, sizeof( kvs::Vec3 ) * nnodes );

        m_values = new kvs::Real32 [nnodes*veclen];
        if ( !m_values ) throw "Cannot allocate memory for 'm_values'";
        memset( m_values, 0, sizeof( kvs::Real32 ) * nnodes * veclen );

        m_interpolation_functions = new kvs::Real32 [nnodes];
        if ( !m_interpolation_functions ) throw "Cannot allocate memory for 'm_interpolation_functions'";
        memset( m_interpolation_functions, 0, sizeof( kvs::Real32 ) * nnodes );

        m_differential_functions  = new kvs::Real32 [nnodes*dimension];
        if ( !m_differential_functions ) throw "Cannot allocate memory for 'm_differential_functions'";
        memset( m_differential_functions, 0, sizeof( kvs::Real32 ) * nnodes * dimension );
    }
    catch ( char* error_message )
    {
        kvsMessageError( error_message );
        return;
    }
}

/*===========================================================================*/
/**
 *  @brief  Destructs the CellBase class.
 */
/*===========================================================================*/
CellBase::~CellBase()
{
    if ( m_coords ) delete [] m_coords;
    if ( m_values ) delete [] m_values;
    if ( m_interpolation_functions ) delete [] m_interpolation_functions;
    if ( m_differential_functions ) delete [] m_differential_functions;
}

/*===========================================================================*/
/**
 *  @brief  Binds a cell indicated by the index.
 *  @param  index [in] cell index
 */
/*===========================================================================*/
void CellBase::bindCell( const kvs::UInt32 index )
{
    const kvs::UnstructuredVolumeObject* volume = m_reference_volume;
    switch ( volume->values().typeID() )
    {
    case kvs::Type::TypeInt8:   ::Bind<kvs::Int8>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeUInt8:  ::Bind<kvs::UInt8>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeInt16:  ::Bind<kvs::Int16>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeUInt16: ::Bind<kvs::UInt16>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeInt32:  ::Bind<kvs::Int32>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeUInt32: ::Bind<kvs::UInt32>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeInt64:  ::Bind<kvs::Int64>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeUInt64: ::Bind<kvs::UInt64>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeReal32: ::Bind<kvs::Real32>( volume, index, m_values, m_coords ); break;
    case kvs::Type::TypeReal64: ::Bind<kvs::Real64>( volume, index, m_values, m_coords ); break;
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a point in the local coordinate.
 *  @param  local [in] coordinate value in the local coordinate
 */
/*===========================================================================*/
void CellBase::setLocalPoint( const kvs::Vec3& local ) const
{
    m_local_point = local;
    this->updateInterpolationFunctions( local );
    this->updateDifferentialFunctions( local );
}

/*===========================================================================*/
/**
 *  @brief  True if this cell contains a point defined in the local coordinate.
 *  @param  local [in] local point
 *  @return True if this cell contains the local point
 */
/*===========================================================================*/
bool CellBase::containsLocalPoint( const kvs::Vec3& local ) const
{
    if ( local.x() < 0 || 1 < local.x() ) { return false; }
    if ( local.y() < 0 || 1 < local.y() ) { return false; }
    if ( local.z() < 0 || 1 < local.z() ) { return false; }
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Transforms the local to the global coordinate.
 *  @return global point for the local point
 */
/*===========================================================================*/
kvs::Vec3 CellBase::globalPoint() const
{
    const size_t nnodes = m_nnodes;
    const float* N = m_interpolation_functions;
    const kvs::Vec3* V = m_coords;
    return this->interpolateCoord( V, N, nnodes );
}

/*===========================================================================*/
/**
 *  @brief  Transforms the global to the local coordinate.
 *  @param  global [in] point in the global coodinate
 */
/*===========================================================================*/
kvs::Vec3 CellBase::globalToLocal( const kvs::Vec3& global ) const
{
    const kvs::Vec3 X( global );

    // Calculate the coordinate of 'global' in the local coordinate
    // by using Newton-Raphson method.
    const float TinyValue = static_cast<float>( 1.e-6 );
    const size_t MaxLoop = 100;
    kvs::Vec3 x0( 0.25f, 0.25f, 0.25f ); // Initial point in local coordinate.
    for ( size_t i = 0; i < MaxLoop; i++ )
    {
        const kvs::Vec3 X0( this->localToGlobal( x0 ) );
        const kvs::Vec3 dX( X - X0 );

        const kvs::Mat3 J( this->JacobiMatrix() );
        const kvs::Vec3 dx = J.transposed().inverted() * dX;
        if ( dx.length() < TinyValue ) break; // Converged.

        x0 += dx;
    }

    return x0;
}

/*===========================================================================*/
/**
 *  @brief  Transforms the local to the global coordinate.
 *  @param  local [in] point in the local coodinate
 */
/*===========================================================================*/
kvs::Vec3 CellBase::localToGlobal( const kvs::Vec3& local ) const
{
    this->setLocalPoint( local );
    return this->globalPoint();
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly in the cell.
 *  @return coordinate value of the sampled point in the global coordinate
 */
/*===========================================================================*/
kvs::Vec3 CellBase::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float p = this->randomNumber();
    const float q = this->randomNumber();
    const float r = this->randomNumber();

    const kvs::Vec3 local( p, q, r );
    return this->localToGlobal( local );
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 CellBase::volume() const
{
    kvsMessageError("'volume' is not implemented.");
    return kvs::Real32( 0.0f );
}

/*===========================================================================*/
/**
 *  @brief  Returns a center of cell in the local coordinate.
 *  @return center of cell in the local coordinate.
 */
/*===========================================================================*/
kvs::Vec3 CellBase::localCenter() const
{
    return kvs::Vec3( 0.5f, 0.5f, 0.5f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the jacobi matrix.
 *  @return Jacobi matrix
 */
/*===========================================================================*/
kvs::Mat3 CellBase::JacobiMatrix() const
{
    const kvs::UInt32 nnodes = m_nnodes;
    const float* dNdp = m_differential_functions;
    const float* dNdq = dNdp + nnodes;
    const float* dNdr = dNdq + nnodes;
    const kvs::Vec3* coords = m_coords;

    const kvs::Vec3 dx = this->interpolateCoord( coords, dNdp, nnodes );
    const kvs::Vec3 dy = this->interpolateCoord( coords, dNdq, nnodes );
    const kvs::Vec3 dz = this->interpolateCoord( coords, dNdr, nnodes );
    return kvs::Mat3( dx[0], dy[0], dz[0], dx[1], dy[1], dz[1], dx[2], dy[2], dz[2] );
}

/*===========================================================================*/
/**
 *  @brief  Returns a center of cell in the global coordinate.
 *  @return center of cell in the global coordinate.
 */
/*===========================================================================*/
kvs::Vec3 CellBase::center() const
{
    const size_t nnodes = this->numberOfCellNodes();
    kvs::Vec3 center = kvs::Vec3::Zero();
    for ( size_t i = 0; i < nnodes; i ++ ) { center += m_coords[i]; }
    return center /= nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Returns the interpolated scalar value at the point.
 */
/*===========================================================================*/
kvs::Real32 CellBase::scalar() const
{
    KVS_ASSERT( m_veclen == 1 );

    const size_t nnodes = m_nnodes;
    const kvs::Real32* N = m_interpolation_functions;
    const kvs::Real32* S = m_values;
    return this->interpolateValue( S, N, nnodes );
}

/*===========================================================================*/
/**
 *  @brief  Returns the interpolated vector value at the point.
 */
/*===========================================================================*/
kvs::Vec3 CellBase::vector() const
{
    KVS_ASSERT( m_veclen == 3 );

    const size_t nnodes = m_nnodes;
    const kvs::Real32* N = m_interpolation_functions;
    const kvs::Real32* Su = m_values;
    const kvs::Real32* Sv = Su + nnodes;
    const kvs::Real32* Sw = Sv + nnodes;

    const kvs::Real32 u = this->interpolateValue( Su, N, nnodes );
    const kvs::Real32 v = this->interpolateValue( Sv, N, nnodes );
    const kvs::Real32 w = this->interpolateValue( Sw, N, nnodes );
    return kvs::Vec3( u, v, w );
}

/*===========================================================================*/
/**
 *  @brief  Returns the gradient vector at the attached point.
 */
/*===========================================================================*/
kvs::Vec3 CellBase::gradientVector() const
{
    KVS_ASSERT( m_veclen == 1 );

    // Calculate a gradient vector in the local coordinate.
    const kvs::UInt32 nnodes = m_nnodes;
    const float* dNdp = m_differential_functions;
    const float* dNdq = m_differential_functions + nnodes;
    const float* dNdr = m_differential_functions + nnodes + nnodes;
    const kvs::Real32* S = m_values;

    const float dSdp = this->interpolateValue( S, dNdp, nnodes );
    const float dSdq = this->interpolateValue( S, dNdq, nnodes );
    const float dSdr = this->interpolateValue( S, dNdr, nnodes );
    const kvs::Vec3 g( dSdp, dSdq, dSdr );

    // Calculate a gradient vector in the global coordinate.
    const kvs::Mat3 J = this->JacobiMatrix();

    float determinant = 0.0f;
    const kvs::Vec3 G = 3.0f * J.inverted( &determinant ) * g;
    return kvs::Math::IsZero( determinant ) ? kvs::Vec3::Zero() : G;
}

/*===========================================================================*/
/**
 *  @brief  Returns the gradient tensor at the attached point.
 */
/*===========================================================================*/
kvs::Mat3 CellBase::gradientTensor() const
{
    KVS_ASSERT( m_veclen == 3 );

    // Calculate a gradient tensor in the local coordinate.
    const kvs::UInt32 nnodes = m_nnodes;
    const float* dNdp = m_differential_functions;
    const float* dNdq = m_differential_functions + nnodes;
    const float* dNdr = m_differential_functions + nnodes + nnodes;
    const kvs::Real32* Su = m_values;
    const kvs::Real32* Sv = Su + nnodes;
    const kvs::Real32* Sw = Sv + nnodes;

    const float dudp = this->interpolateValue( Su, dNdp, nnodes );
    const float dudq = this->interpolateValue( Su, dNdq, nnodes );
    const float dudr = this->interpolateValue( Su, dNdr, nnodes );
    const float dvdp = this->interpolateValue( Sv, dNdp, nnodes );
    const float dvdq = this->interpolateValue( Sv, dNdq, nnodes );
    const float dvdr = this->interpolateValue( Sv, dNdr, nnodes );
    const float dwdp = this->interpolateValue( Sw, dNdp, nnodes );
    const float dwdq = this->interpolateValue( Sw, dNdq, nnodes );
    const float dwdr = this->interpolateValue( Sw, dNdr, nnodes );
    const kvs::Mat3 t( dudp, dvdp, dwdp, dudq, dvdq, dwdq, dudr, dvdr, dwdr );

    // Calculate a gradient tensor in the global coordinate.
    const kvs::Mat3 J = this->JacobiMatrix();

    float determinant = 0.0f;
    const kvs::Mat3 T = 3.0f * J.inverted( &determinant ) * t;
    return kvs::Math::IsZero( determinant ) ? kvs::Mat3::Zero() : T;
}

/*===========================================================================*/
/**
 *  @brief  Returns true if this cell contains the global point.
 *  @param  global [in] global point
 *  @return true if this cell contains the global point
 */
/*===========================================================================*/
bool CellBase::contains( const kvs::Vec3& global ) const
{
    if ( this->containsInBounds( global ) )
    {
        const kvs::Vec3 local = this->globalToLocal( global );
        return this->containsLocalPoint( local );
    }
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Returns a random number that is generated by using the xorshift.
 */
/*===========================================================================*/
kvs::Real32 CellBase::randomNumber() const
{
    return m_rand();
#if 0
    // xorshift RGNs with period at least 2^128 - 1.
    static kvs::Real32 t24 = 1.0/16777216.0; /* 0.5**24 */
    static kvs::UInt32 x=123456789,y=362436069,z=521288629,w=88675123;
    kvs::UInt32 t;
    t=(x^(x<<11));
    x=y;y=z;z=w;
    w=(w^(w>>19))^(t^(t>>8));

    return t24*static_cast<kvs::Real32>(w>>8);
#endif
}

/*===========================================================================*/
/**
 *  @brief  True if the global point is contained within the bounding box of the cell.
 *  @param  global [in] global point
 *  @return true if the cell contains the global point
 */
/*===========================================================================*/
bool CellBase::containsInBounds( const kvs::Vec3& global ) const
{
    kvs::Vec3 min_coord = this->coords()[0];
    kvs::Vec3 max_coord = this->coords()[0];
    const size_t nnodes = this->numberOfCellNodes();
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const kvs::Vec3 v = this->coords()[i];
        min_coord.x() = kvs::Math::Min( min_coord.x(), v.x() );
        min_coord.y() = kvs::Math::Min( min_coord.y(), v.y() );
        min_coord.z() = kvs::Math::Min( min_coord.z(), v.z() );
        max_coord.x() = kvs::Math::Max( max_coord.x(), v.x() );
        max_coord.y() = kvs::Math::Max( max_coord.y(), v.y() );
        max_coord.z() = kvs::Math::Max( max_coord.z(), v.z() );
    }

    if ( global.x() < min_coord.x() || global.x() > max_coord.x() ) { return false; }
    if ( global.y() < min_coord.y() || global.y() > max_coord.y() ) { return false; }
    if ( global.z() < min_coord.z() || global.z() > max_coord.z() ) { return false; }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Interpolates a value at the local point with interpolation functions.
 *  @param  values [in] values at the nodes
 *  @param  weights [in] weights for the values
 *  @param  nnodes [in] number of nodes
 *  @return interpolated value
 */
/*===========================================================================*/
kvs::Real32 CellBase::interpolateValue(
    const kvs::Real32* values,
    const kvs::Real32* weights,
    const size_t nnodes ) const
{
    kvs::Real32 value = 0;
    for ( size_t i = 0; i < nnodes; ++i )
    {
        value += *values * *weights;
        ++values; ++weights;
    }
    return value;
}

/*===========================================================================*/
/**
 *  @brief  Interpolates a coordinate value with interpolation functions.
 *  @param  coords [in] coordinate values at the nodes
 *  @param  weights [in] weights for the values
 *  @param  nnodes [in] number of nodes
 *  @return interpolated value
 */
/*===========================================================================*/
kvs::Vec3 CellBase::interpolateCoord(
    const kvs::Vec3* coords,
    const kvs::Real32* weights,
    const size_t nnodes ) const
{
    kvs::Vec3 coord = kvs::Vec3::Zero();
    for ( size_t i = 0; i < nnodes; ++i )
    {
        coord += *coords * *weights;
        ++coords; ++weights;
    }
    return coord;
}

} // end of namespace kvs
