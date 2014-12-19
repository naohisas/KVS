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
 *  @param  scalars [out] scalar values
 *  @param  vertices [out] vertex coords
 */
/*===========================================================================*/
template <typename ValueType>
inline void GetScalarsAndVertices(
    const kvs::UnstructuredVolumeObject* volume,
    const size_t index,
    kvs::Real32* scalars,
    kvs::Vec3* vertices )
{
    const kvs::UInt32* const connections = volume->connections().data();
    const kvs::Real32* const coords = volume->coords().data();
    const ValueType* const values = static_cast<const ValueType*>( volume->values().data() );

    const size_t nnodes = volume->numberOfCellNodes();
    const kvs::UInt32 connection_index = nnodes * index;
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const kvs::UInt32 node_index = connections[ connection_index + i ];
        scalars[i] = static_cast<kvs::Real32>( values[ node_index ] );

        const kvs::UInt32 coord_index = 3 * node_index;
        vertices[i].set( coords[ coord_index ], coords[ coord_index + 1 ], coords[ coord_index + 2 ] );
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
    m_global_point( 0, 0, 0 ),
    m_local_point( 0, 0, 0 ),
    m_reference_volume( volume )
{
    const size_t dimension = 3;
    const size_t nnodes = m_nnodes;
    try
    {
        m_vertices = new kvs::Vec3 [nnodes];
        if ( !m_vertices ) throw "Cannot allocate memory for 'm_vertices'";
        memset( m_vertices, 0, sizeof( kvs::Vec3 ) * nnodes );

        m_scalars = new kvs::Real32 [nnodes];
        if ( !m_scalars ) throw "Cannot allocate memory for 'm_scalars'";
        memset( m_scalars, 0, sizeof( kvs::Real32 ) * nnodes );

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
    if ( m_vertices ) delete [] m_vertices;
    if ( m_scalars ) delete [] m_scalars;
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
    case kvs::Type::TypeInt8:   ::GetScalarsAndVertices<kvs::Int8>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeUInt8:  ::GetScalarsAndVertices<kvs::UInt8>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeInt16:  ::GetScalarsAndVertices<kvs::Int16>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeUInt16: ::GetScalarsAndVertices<kvs::UInt16>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeInt32:  ::GetScalarsAndVertices<kvs::Int32>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeUInt32: ::GetScalarsAndVertices<kvs::UInt32>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeInt64:  ::GetScalarsAndVertices<kvs::Int64>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeUInt64: ::GetScalarsAndVertices<kvs::UInt64>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeReal32: ::GetScalarsAndVertices<kvs::Real32>( volume, index, m_scalars, m_vertices ); break;
    case kvs::Type::TypeReal64: ::GetScalarsAndVertices<kvs::Real64>( volume, index, m_scalars, m_vertices ); break;
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a point in the global coordinate.
 *  @param  global [in] coordinate value in the global coordinate
 */
/*===========================================================================*/
void CellBase::setGlobalPoint( const kvs::Vec3& global ) const
{
    m_global_point = global;
    this->setLocalPoint( this->transformGlobalToLocal( global ) );
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
    this->interpolationFunctions( local );
    this->differentialFunctions( local );
}

/*===========================================================================*/
/**
 *  @brief  Transforms the global to the local coordinate.
 *  @param  global [in] point in the global coodinate
 */
/*===========================================================================*/
const kvs::Vec3 CellBase::transformGlobalToLocal( const kvs::Vec3& global ) const
{
    const kvs::Vec3 X( global );

    // Calculate the coordinate of 'global' in the local coordinate
    // by using Newton-Raphson method.
    const float TinyValue = static_cast<float>( 1.e-6 );
    const size_t MaxLoop = 100;
    kvs::Vec3 x0( 0.25f, 0.25f, 0.25f ); // Initial point in local coordinate.
    for ( size_t i = 0; i < MaxLoop; i++ )
    {
        this->setLocalPoint( x0 );
        const kvs::Vec3 X0( this->transformLocalToGlobal( x0 ) );
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
 *  @param  point [in] point in the local coordinate
 */
/*===========================================================================*/
const kvs::Vec3 CellBase::transformLocalToGlobal( const kvs::Vec3& local ) const
{
    kvs::IgnoreUnusedVariable( local );

    const float* N = m_interpolation_functions;
    const kvs::Vec3* V = m_vertices;
    const size_t nnodes = m_nnodes;

    float X = 0; for ( size_t i = 0; i < nnodes; i++ ) X += N[i] * V[i].x();
    float Y = 0; for ( size_t i = 0; i < nnodes; i++ ) Y += N[i] * V[i].y();
    float Z = 0; for ( size_t i = 0; i < nnodes; i++ ) Z += N[i] * V[i].z();

    return kvs::Vec3( X, Y, Z );
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly in the cell.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
const kvs::Vec3 CellBase::randomSampling() const
{
    kvsMessageError("'randomSampling' is not implemented.");
    return kvs::Vec3( 0.0f, 0.0f, 0.0f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
const kvs::Real32 CellBase::volume() const
{
    kvsMessageError("'volume' is not implemented.");
    return kvs::Real32( 0.0f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the averaged scalar value.
 *  @return averaged scalar value
 */
/*===========================================================================*/
const kvs::Real32 CellBase::averagedScalar() const
{
    const size_t nnodes = m_nnodes;
    const kvs::Real32* s = m_scalars;
    kvs::Real32 S = 0; for ( size_t i = 0; i < nnodes; i++ ) { S += s[i]; }
    return S / nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Returns the interpolated scalar value at the attached point.
 */
/*===========================================================================*/
const kvs::Real32 CellBase::scalar() const
{
    const size_t nnodes = m_nnodes;
    const float* N = m_interpolation_functions;
    const kvs::Real32* s = m_scalars;
    kvs::Real32 S = 0; for ( size_t i = 0; i < nnodes; i++ ) { S += N[i] * s[i]; }
    return S;
}

/*===========================================================================*/
/**
 *  @brief  Returns the gradient vector at the attached point.
 */
/*===========================================================================*/
const kvs::Vec3 CellBase::gradient() const
{
    // Calculate a gradient vector in the local coordinate.
    const kvs::UInt32 nnodes = m_nnodes;
    const float* dNdx = m_differential_functions;
    const float* dNdy = m_differential_functions + nnodes;
    const float* dNdz = m_differential_functions + nnodes + nnodes;
    const kvs::Real32* s = m_scalars;

    float dsdx = 0.0f;
    float dsdy = 0.0f;
    float dsdz = 0.0f;
    for ( size_t i = 0; i < nnodes; i++ )
    {
        dsdx += s[i] * dNdx[i];
        dsdy += s[i] * dNdy[i];
        dsdz += s[i] * dNdz[i];
    }

    const kvs::Vec3 g( dsdx, dsdy, dsdz );

    // Calculate a gradient vector in the global coordinate.
    const kvs::Mat3 J = this->JacobiMatrix();

    float determinant = 0.0f;
    const kvs::Vec3 G = J.inverted( &determinant ) * g;

    return kvs::Math::IsZero( determinant ) ? kvs::Vec3( 0.0f, 0.0f, 0.0f ) : G;
}

/*===========================================================================*/
/**
 *  @brief  Returns the jacobi matrix.
 *  @return Jacobi matrix
 */
/*===========================================================================*/
const kvs::Mat3 CellBase::JacobiMatrix() const
{
    const kvs::UInt32 nnodes = m_nnodes;
    const float* dNdx = m_differential_functions;
    const float* dNdy = m_differential_functions + nnodes;
    const float* dNdz = m_differential_functions + nnodes * 2;
    const kvs::Vec3* V = m_vertices;

    float dXdx = 0; for ( size_t i = 0; i < nnodes; i++ ) dXdx += dNdx[i]*V[i].x();
    float dYdx = 0; for ( size_t i = 0; i < nnodes; i++ ) dYdx += dNdx[i]*V[i].y();
    float dZdx = 0; for ( size_t i = 0; i < nnodes; i++ ) dZdx += dNdx[i]*V[i].z();

    float dXdy = 0; for ( size_t i = 0; i < nnodes; i++ ) dXdy += dNdy[i]*V[i].x();
    float dYdy = 0; for ( size_t i = 0; i < nnodes; i++ ) dYdy += dNdy[i]*V[i].y();
    float dZdy = 0; for ( size_t i = 0; i < nnodes; i++ ) dZdy += dNdy[i]*V[i].z();

    float dXdz = 0; for ( size_t i = 0; i < nnodes; i++ ) dXdz += dNdz[i]*V[i].x();
    float dYdz = 0; for ( size_t i = 0; i < nnodes; i++ ) dYdz += dNdz[i]*V[i].y();
    float dZdz = 0; for ( size_t i = 0; i < nnodes; i++ ) dZdz += dNdz[i]*V[i].z();

    return kvs::Mat3( dXdx, dYdx, dZdx, dXdy, dYdy, dZdy, dXdz, dYdz, dZdz );
}

/*===========================================================================*/
/**
 *  @brief  Returns a random number that is generated by using the xorshift.
 */
/*===========================================================================*/
const kvs::Real32 CellBase::randomNumber() const
{
    // xorshift RGNs with period at least 2^128 - 1.
    static kvs::Real32 t24 = 1.0/16777216.0; /* 0.5**24 */
    static kvs::UInt32 x=123456789,y=362436069,z=521288629,w=88675123;
    kvs::UInt32 t;
    t=(x^(x<<11));
    x=y;y=z;z=w;
    w=(w^(w>>19))^(t^(t>>8));

    return t24*static_cast<kvs::Real32>(w>>8);
}

} // end of namespace kvs
