/*****************************************************************************/
/**
 *  @file   TetrahedraToTetrahedra.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TetrahedraToTetrahedra.cpp 1403 2012-12-07 07:35:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "TetrahedraToTetrahedra.h"
#include <algorithm>
#include <map>
#include <kvs/AnyValueArray>


namespace
{

/*===========================================================================*/
/**
 *  @brief  ID map class for removing the duplicated nodes.
 */
/*===========================================================================*/
class IDMap
{
public:

    typedef kvs::UInt32 Key;
    typedef kvs::UInt32 Value;
    typedef std::map<Key,Value> Bucket;

private:

    size_t m_index; ///< index of the connection ID
    Bucket m_bucket; ///< bucket for the IDs

public:

    IDMap();

    const Bucket& bucket() const;
    void insert( const kvs::UInt32 id );
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new IDMap class.
 */
/*===========================================================================*/
IDMap::IDMap():
    m_index( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the ID bucket.
 *  @return ID bucket
 */
/*===========================================================================*/
const IDMap::Bucket& IDMap::bucket() const
{
    return m_bucket;
}

/*===========================================================================*/
/**
 *  @brief  Inserts a ID.
 *  @param  id [in] ID
 */
/*===========================================================================*/
void IDMap::insert( const kvs::UInt32 id )
{
    const Key key = id;
    const Value value = m_index;

    Bucket::iterator i = m_bucket.find( key );
    Bucket::const_iterator last = m_bucket.end();
    if ( i == last )
    {
        m_bucket.insert( Bucket::value_type( key, value ) );
        m_index++;
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TetrahedraToTetrahedra class.
 */
/*===========================================================================*/
TetrahedraToTetrahedra::TetrahedraToTetrahedra():
    m_method( TetrahedraToTetrahedra::Subdivision8 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new TetrahedraToTetrahedra class.
 *  @param  volume [in] pointer to a volume data
 *  @param  method [in] conversion method
 */
/*===========================================================================*/
TetrahedraToTetrahedra::TetrahedraToTetrahedra(
    const kvs::UnstructuredVolumeObject* volume,
    const TetrahedraToTetrahedra::Method method )
{
    this->setMethod( method );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the TetrahedraToTetrahedra class.
 */
/*===========================================================================*/
TetrahedraToTetrahedra::~TetrahedraToTetrahedra()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes this class.
 *  @param  object [in] pointer to the object
 *  @return pointer to the converted object
 */
/*===========================================================================*/
TetrahedraToTetrahedra::SuperClass* TetrahedraToTetrahedra::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not supported.");
        return NULL;
    }

    if ( volume->cellType() == kvs::UnstructuredVolumeObject::QuadraticTetrahedra )
    {
        if ( m_method == TetrahedraToTetrahedra::Subdivision8 )
        {
            const std::type_info& type = volume->values().typeInfo()->type();
            if (      type == typeid( kvs::Int8   ) ) this->subdivide_8_tetrahedra<kvs::Int8>(   volume );
            else if ( type == typeid( kvs::Int16  ) ) this->subdivide_8_tetrahedra<kvs::Int16>(  volume );
            else if ( type == typeid( kvs::Int32  ) ) this->subdivide_8_tetrahedra<kvs::Int32>(  volume );
            else if ( type == typeid( kvs::Int64  ) ) this->subdivide_8_tetrahedra<kvs::Int64>(  volume );
            else if ( type == typeid( kvs::UInt8  ) ) this->subdivide_8_tetrahedra<kvs::UInt8>(  volume );
            else if ( type == typeid( kvs::UInt16 ) ) this->subdivide_8_tetrahedra<kvs::UInt16>( volume );
            else if ( type == typeid( kvs::UInt32 ) ) this->subdivide_8_tetrahedra<kvs::UInt32>( volume );
            else if ( type == typeid( kvs::UInt64 ) ) this->subdivide_8_tetrahedra<kvs::UInt64>( volume );
            else if ( type == typeid( kvs::Real32 ) ) this->subdivide_8_tetrahedra<kvs::Real32>( volume );
            else if ( type == typeid( kvs::Real64 ) ) this->subdivide_8_tetrahedra<kvs::Real64>( volume );
            else
            {
                BaseClass::setSuccess( false );
                kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
                return NULL;
            }
        }
        else if ( m_method == TetrahedraToTetrahedra::Removal )
        {
            const std::type_info& type = volume->values().typeInfo()->type();
            if (      type == typeid( kvs::Int8   ) ) this->remove_quadratic_nodes<kvs::Int8>(   volume );
            else if ( type == typeid( kvs::Int16  ) ) this->remove_quadratic_nodes<kvs::Int16>(  volume );
            else if ( type == typeid( kvs::Int32  ) ) this->remove_quadratic_nodes<kvs::Int32>(  volume );
            else if ( type == typeid( kvs::Int64  ) ) this->remove_quadratic_nodes<kvs::Int64>(  volume );
            else if ( type == typeid( kvs::UInt8  ) ) this->remove_quadratic_nodes<kvs::UInt8>(  volume );
            else if ( type == typeid( kvs::UInt16 ) ) this->remove_quadratic_nodes<kvs::UInt16>( volume );
            else if ( type == typeid( kvs::UInt32 ) ) this->remove_quadratic_nodes<kvs::UInt32>( volume );
            else if ( type == typeid( kvs::UInt64 ) ) this->remove_quadratic_nodes<kvs::UInt64>( volume );
            else if ( type == typeid( kvs::Real32 ) ) this->remove_quadratic_nodes<kvs::Real32>( volume );
            else if ( type == typeid( kvs::Real64 ) ) this->remove_quadratic_nodes<kvs::Real64>( volume );
            else
            {
                BaseClass::setSuccess( false );
                kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
                return NULL;
            }
        }
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not tetrahedral cells.");
        return NULL;
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Sets conversion method.
 *  @param  method [in] conversion method
 */
/*===========================================================================*/
void TetrahedraToTetrahedra::setMethod( const TetrahedraToTetrahedra::Method method )
{
    m_method = method;
}

/*===========================================================================*/
/**
 *  @brief  Subdivides a qudratic tetrahedron into eight tetrahedra.
 *  @param  volume [in] pointer to the volume data
 */
/*===========================================================================*/
template <typename T>
void TetrahedraToTetrahedra::subdivide_8_tetrahedra( const kvs::UnstructuredVolumeObject* volume )
{
    // Quadratic tetrahedral cells.
    const size_t tet2_ncells = volume->numberOfCells();
    const kvs::UInt32* tet2_pconnections = volume->connections().data();

    // Tetrahedral cells.
    const size_t ndivisions = 8;
    const size_t tet_ncells = tet2_ncells * ndivisions;
    kvs::ValueArray<kvs::UInt32> tet_connections( tet_ncells * 4 );
    kvs::UInt32* tet_pconnections = tet_connections.data();
    for ( size_t i = 0; i < tet2_ncells; i++ )
    {
        const kvs::UInt32 id0 = *(tet2_pconnections++);
        const kvs::UInt32 id1 = *(tet2_pconnections++);
        const kvs::UInt32 id2 = *(tet2_pconnections++);
        const kvs::UInt32 id3 = *(tet2_pconnections++);
        const kvs::UInt32 id4 = *(tet2_pconnections++);
        const kvs::UInt32 id5 = *(tet2_pconnections++);
        const kvs::UInt32 id6 = *(tet2_pconnections++);
        const kvs::UInt32 id7 = *(tet2_pconnections++);
        const kvs::UInt32 id8 = *(tet2_pconnections++);
        const kvs::UInt32 id9 = *(tet2_pconnections++);

        *(tet_pconnections++) = id0;
        *(tet_pconnections++) = id4;
        *(tet_pconnections++) = id5;
        *(tet_pconnections++) = id6;

        *(tet_pconnections++) = id4;
        *(tet_pconnections++) = id1;
        *(tet_pconnections++) = id7;
        *(tet_pconnections++) = id9;

        *(tet_pconnections++) = id5;
        *(tet_pconnections++) = id7;
        *(tet_pconnections++) = id2;
        *(tet_pconnections++) = id8;

        *(tet_pconnections++) = id6;
        *(tet_pconnections++) = id9;
        *(tet_pconnections++) = id8;
        *(tet_pconnections++) = id3;

        *(tet_pconnections++) = id5;
        *(tet_pconnections++) = id6;
        *(tet_pconnections++) = id4;
        *(tet_pconnections++) = id9;

        *(tet_pconnections++) = id5;
        *(tet_pconnections++) = id9;
        *(tet_pconnections++) = id4;
        *(tet_pconnections++) = id7;

        *(tet_pconnections++) = id5;
        *(tet_pconnections++) = id8;
        *(tet_pconnections++) = id9;
        *(tet_pconnections++) = id7;

        *(tet_pconnections++) = id5;
        *(tet_pconnections++) = id6;
        *(tet_pconnections++) = id9;
        *(tet_pconnections++) = id8;
    }

    if ( volume->hasMinMaxExternalCoords() )
    {
        const kvs::Vector3f min_coord( volume->minExternalCoord() );
        const kvs::Vector3f max_coord( volume->maxExternalCoord() );
        SuperClass::setMinMaxExternalCoords( min_coord, max_coord );
    }

    if ( volume->hasMinMaxObjectCoords() )
    {
        const kvs::Vector3f min_coord( volume->minObjectCoord() );
        const kvs::Vector3f max_coord( volume->maxObjectCoord() );
        SuperClass::setMinMaxObjectCoords( min_coord, max_coord );
    }

    if ( volume->hasMinMaxValues() )
    {
        const kvs::Real64 min_value( volume->minValue() );
        const kvs::Real64 max_value( volume->maxValue() );
        SuperClass::setMinMaxValues( min_value, max_value );
    }

    SuperClass::setVeclen( volume->veclen() );
    SuperClass::setNumberOfNodes( volume->numberOfNodes() );
    SuperClass::setNumberOfCells( tet_ncells );
    SuperClass::setCellType( kvs::UnstructuredVolumeObject::Tetrahedra );
    SuperClass::setCoords( volume->coords() );
    SuperClass::setConnections( tet_connections );
    SuperClass::setValues( volume->values() );
}

/*===========================================================================*/
/**
 *  @brief  Removes the quadratic nodes.
 *  @param  volume [in] pointer to the volume data
 */
/*===========================================================================*/
template <typename T>
void TetrahedraToTetrahedra::remove_quadratic_nodes( const kvs::UnstructuredVolumeObject* volume )
{
    const size_t tet2_ncells = volume->numberOfCells();
    const kvs::UInt32* tet2_pconnections = volume->connections().data();
    const T* tet2_pvalues = static_cast<const T*>( volume->values().data() );
    const kvs::Real32* tet2_pcoords = volume->coords().data();

    ::IDMap id_map;
    for ( size_t i = 0; i < tet2_ncells; i++ )
    {
        const kvs::UInt32 id0 = tet2_pconnections[ 10 * i + 0 ];
        const kvs::UInt32 id1 = tet2_pconnections[ 10 * i + 1 ];
        const kvs::UInt32 id2 = tet2_pconnections[ 10 * i + 2 ];
        const kvs::UInt32 id3 = tet2_pconnections[ 10 * i + 3 ];

        id_map.insert( id0 );
        id_map.insert( id1 );
        id_map.insert( id2 );
        id_map.insert( id3 );
    }

    const size_t tet_ncells = tet2_ncells;
    kvs::ValueArray<kvs::UInt32> tet_connections( tet_ncells * 4 );
    kvs::UInt32* tet_pconnections = tet_connections.data();
    for ( size_t i = 0; i < tet2_ncells; i++ )
    {
        const kvs::UInt32 id0 = tet2_pconnections[ 10 * i + 0 ];
        const kvs::UInt32 id1 = tet2_pconnections[ 10 * i + 1 ];
        const kvs::UInt32 id2 = tet2_pconnections[ 10 * i + 2 ];
        const kvs::UInt32 id3 = tet2_pconnections[ 10 * i + 3 ];

        *(tet_pconnections++) = id_map.bucket().find( id0 )->second;
        *(tet_pconnections++) = id_map.bucket().find( id1 )->second;
        *(tet_pconnections++) = id_map.bucket().find( id2 )->second;
        *(tet_pconnections++) = id_map.bucket().find( id3 )->second;
    }

    const size_t tet_veclen = volume->veclen();
    const size_t tet_nnodes = id_map.bucket().size();
    kvs::ValueArray<T> tet_values( tet_nnodes * tet_veclen );
    kvs::ValueArray<kvs::Real32> tet_coords( tet_nnodes * 3 );
    T* tet_pvalues = tet_values.data();
    kvs::Real32* tet_pcoords = tet_coords.data();
    ::IDMap::Bucket::const_iterator id = id_map.bucket().begin();
    while ( id != id_map.bucket().end() )
    {
        // Value array.
        for ( size_t i = 0; i < tet_veclen; i++ )
        {
            tet_pvalues[ id->second * tet_veclen + i ] = tet2_pvalues[ id->first * tet_veclen + i ];
        }

        // Coordinate data array.
        tet_pcoords[ id->second * 3 + 0 ] = tet2_pcoords[ id->first * 3 + 0 ];
        tet_pcoords[ id->second * 3 + 1 ] = tet2_pcoords[ id->first * 3 + 1 ];
        tet_pcoords[ id->second * 3 + 2 ] = tet2_pcoords[ id->first * 3 + 2 ];

        ++id;
    }

    if ( volume->hasMinMaxExternalCoords() )
    {
        const kvs::Vector3f min_coord( volume->minExternalCoord() );
        const kvs::Vector3f max_coord( volume->maxExternalCoord() );
        SuperClass::setMinMaxExternalCoords( min_coord, max_coord );
    }

    if ( volume->hasMinMaxObjectCoords() )
    {
        const kvs::Vector3f min_coord( volume->minObjectCoord() );
        const kvs::Vector3f max_coord( volume->maxObjectCoord() );
        SuperClass::setMinMaxObjectCoords( min_coord, max_coord );
    }

    if ( volume->hasMinMaxValues() )
    {
        const kvs::Real64 min_value( volume->minValue() );
        const kvs::Real64 max_value( volume->maxValue() );
        SuperClass::setMinMaxValues( min_value, max_value );
    }

    SuperClass::setVeclen( volume->veclen() );
    SuperClass::setNumberOfNodes( tet_nnodes );
    SuperClass::setNumberOfCells( tet_ncells );
    SuperClass::setCellType( kvs::UnstructuredVolumeObject::Tetrahedra );
    SuperClass::setCoords( tet_coords );
    SuperClass::setConnections( tet_connections );
    SuperClass::setValues( kvs::AnyValueArray( tet_values ) );
}

} // end of namespace kvs
