/*****************************************************************************/
/**
 *  @file   ExternalFaces.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExternalFaces.cpp 1776 2014-05-16 13:50:56Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ExternalFaces.h"
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/TransferFunction>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Timer>
#include <map>
#include <cstring>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Calculates color indices for the given nodes.
 *  @param  value [in] pointer to the node value
 *  @param  min_value [in] minimum value of the node value
 *  @param  max_value [in] maximum value of the node value
 *  @param  veclen [in] vector length of the node data
 *  @param  colormap_resolution [in] resolution of the color map
 *  @param  node_index [in] node indices
 *  @param  color_level [out] pointer to the color indices
 */
/*===========================================================================*/
template <const size_t N, typename T>
inline void GetColorIndices(
    const T* value,
    const kvs::Real64 min_value,
    const kvs::Real64 max_value,
    const size_t veclen,
    const size_t colormap_resolution,
    const kvs::UInt32 node_index[N],
    kvs::UInt32 (*color_index)[N] )
{
    const kvs::Real64 normalize =
        static_cast<kvs::Real64>( colormap_resolution - 1 ) / ( max_value - min_value );

    // Scalar data.
    if ( veclen == 1 )
    {
        for ( size_t i = 0; i < N; i++ )
        {
            (*color_index)[i] = kvs::UInt32( normalize * ( kvs::Real64( value[ node_index[i] ] ) - min_value ) );
        }
    }
    // Vector data.
    else
    {
        // In case of the vector component, the magnitude value is calculated.
        kvs::Real64 magnitude[N]; memset( magnitude, 0, sizeof( kvs::Real64 ) * N );
        for ( size_t i = 0; i < veclen; ++i )
        {
            for ( size_t j = 0; j < N; j++ )
            {
                magnitude[j] += kvs::Math::Square( kvs::Real64( value[ veclen * node_index[j] + i ] ) );
            }
        }

        for ( size_t i = 0; i < N; i++ )
        {
            magnitude[i] = std::sqrt( magnitude[i] );
            (*color_index)[i] = kvs::UInt32( normalize * ( magnitude[i] - min_value ) );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Triangle face class.
 */
/*===========================================================================*/
class TriangleFace
{
private:
    kvs::UInt32 m_id[3]; ///< vertex IDs

public:
    TriangleFace( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2 )
    {
        this->set( id0, id1, id2 );
    }

    kvs::UInt32 id( const size_t index ) const
    {
        return m_id[ index ];
    }

    void set( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2 )
    {
        m_id[0] = id0; m_id[1] = id1; m_id[2] = id2;
    }

    friend bool operator == ( const TriangleFace& f0, const TriangleFace& f1 )
    {
        for ( size_t i = 0; i < 3; i++ )
        {
            bool flag = false;
            for ( size_t j = 0; j < 3; j++ )
            {
                if ( f0.id(i) == f1.id(j) ) { flag = true; continue; }
            }
            if ( !flag ) { return false; }
        }
        return true;
    }
};

/*===========================================================================*/
/**
 *  @brief  Quadrangle face class.
 */
/*===========================================================================*/
class QuadrangleFace
{
private:
    kvs::UInt32 m_id[4]; ///< vertex IDs

public:
    QuadrangleFace( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2, const kvs::UInt32 id3 )
    {
        this->set( id0, id1, id2, id3 );
    }

    kvs::UInt32 id( const size_t index ) const
    {
        return m_id[ index ];
    }

    void set( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2, const kvs::UInt32 id3 )
    {
        m_id[0] = id0; m_id[1] = id1; m_id[2] = id2; m_id[3] = id3;
    }

    friend bool operator == ( const QuadrangleFace& f0, const QuadrangleFace& f1 )
    {
        for ( size_t i = 0; i < 4; i++ )
        {
            bool flag = false;
            for ( size_t j = 0; j < 4; j++ )
            {
                if ( f0.id(i) == f1.id(j) ) { flag = true; continue; }
            }
            if ( !flag ) return false;
        }
        return true;
    }
};

/*===========================================================================*/
/**
 *  @brief  TriangleFace map class.
 */
/*===========================================================================*/
class TriangleFaceMap
{
public:
    typedef kvs::UInt32 Key;
    typedef TriangleFace Value;
    typedef std::multimap<Key,Value> Bucket;

private:
    size_t m_nvertices; ///< number of vertices of the original data
    Bucket m_bucket; ///< bucket for the edge data

public:
    TriangleFaceMap( const size_t nvertices ):
        m_nvertices( nvertices ) {}

    const Bucket& bucket() const { return m_bucket; }

    void insert( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2 )
    {
        const Key key( this->get_key( id0, id1, id2 ) );
        const Value value( id0, id1, id2 );

        Bucket::iterator f = m_bucket.find( key );
        Bucket::const_iterator last = m_bucket.end();
        if ( f != last )
        {
            Bucket::const_iterator upper = m_bucket.upper_bound( key );
            do
            {
                if ( f->second == value )
                {
                    // The edge has been already inserted in the bucket.
                    m_bucket.erase( f );
                    return;
                }
                f++;
            } while ( f != upper );
        }

        m_bucket.insert( std::make_pair( key, value ) );
    }

private:
    Key get_key( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2 )
    {
        const kvs::UInt32 sum = id0 + id1 + id2;
        return sum % kvs::UInt32( m_nvertices );
    }
};

/*===========================================================================*/
/**
 *  @brief  QuadrangleFace map class.
 */
/*===========================================================================*/
class QuadrangleFaceMap
{
public:
    typedef kvs::UInt32 Key;
    typedef QuadrangleFace Value;
    typedef std::multimap<Key,Value> Bucket;

private:
    size_t m_nvertices; ///< number of vertices of the original data
    Bucket m_bucket; ///< bucket for the edge data

public:
    QuadrangleFaceMap( const size_t nvertices ):
        m_nvertices( nvertices ) {}

    const Bucket& bucket() const { return m_bucket; }

    void insert( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2, const kvs::UInt32 id3 )
    {
        const Key key( this->get_key( id0, id1, id2, id3 ) );
        const Value value( id0, id1, id2, id3 );

        Bucket::iterator f = m_bucket.find( key );
        Bucket::const_iterator last = m_bucket.end();
        if ( f != last )
        {
            Bucket::const_iterator upper = m_bucket.upper_bound( key );
            do
            {
                if ( f->second == value )
                {
                    // The edge has been already inserted in the bucket.
                    m_bucket.erase( f );
                    return;
                }
                f++;
            } while ( f != upper );
        }

        m_bucket.insert( std::make_pair( key, value ) );
    }

private:
    Key get_key( const kvs::UInt32 id0, const kvs::UInt32 id1, const kvs::UInt32 id2, const kvs::UInt32 id3 )
    {
        const kvs::UInt32 sum = id0 + id1 + id2 + id3;
        return sum % kvs::UInt32( m_nvertices );
    }
};

/*===========================================================================*/
/**
 *  @brief  Creates a face map for the tetrahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 *  @param  face_map [out] pointer to the face map
 */
/*===========================================================================*/
inline void CreateTetrahedraFaceMap(
    const kvs::UnstructuredVolumeObject* volume,
    TriangleFaceMap* face_map )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        // Local vertices of the tetrahedral cell.
        const kvs::UInt32 v0 = connections[ connection_index     ];
        const kvs::UInt32 v1 = connections[ connection_index + 1 ];
        const kvs::UInt32 v2 = connections[ connection_index + 2 ];
        const kvs::UInt32 v3 = connections[ connection_index + 3 ];
        connection_index += 4;

        // Local faces of the cell (4 triangle meshes).
        face_map->insert( v0, v1, v2 );
        face_map->insert( v0, v2, v3 );
        face_map->insert( v0, v3, v1 );
        face_map->insert( v1, v3, v2 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a face map for the quadratic tetrahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 *  @param  face_map [out] pointer to the face map
 */
/*===========================================================================*/
inline void CreateQuadraticTetrahedraFaceMap(
    const kvs::UnstructuredVolumeObject* volume,
    TriangleFaceMap* face_map )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        // Local vertices of the quadratic tetrahedral cell.
        const kvs::UInt32 v0 = connections[ connection_index     ];
        const kvs::UInt32 v1 = connections[ connection_index + 1 ];
        const kvs::UInt32 v2 = connections[ connection_index + 2 ];
        const kvs::UInt32 v3 = connections[ connection_index + 3 ];
        const kvs::UInt32 v4 = connections[ connection_index + 4 ];
        const kvs::UInt32 v5 = connections[ connection_index + 5 ];
        const kvs::UInt32 v6 = connections[ connection_index + 6 ];
        const kvs::UInt32 v7 = connections[ connection_index + 7 ];
        const kvs::UInt32 v8 = connections[ connection_index + 8 ];
        const kvs::UInt32 v9 = connections[ connection_index + 9 ];
        connection_index += 10;

        // Local faces of the cell (16 triangle meshes).
        face_map->insert( v0, v4, v5 );
        face_map->insert( v4, v1, v7 );
        face_map->insert( v5, v7, v2 );
        face_map->insert( v7, v5, v4 );

        face_map->insert( v0, v5, v6 );
        face_map->insert( v5, v2, v8 );
        face_map->insert( v6, v8, v3 );
        face_map->insert( v8, v6, v5 );

        face_map->insert( v0, v6, v4 );
        face_map->insert( v6, v3, v9 );
        face_map->insert( v4, v9, v1 );
        face_map->insert( v9, v4, v6 );

        face_map->insert( v1, v9, v7 );
        face_map->insert( v9, v3, v8 );
        face_map->insert( v7, v8, v2 );
        face_map->insert( v8, v7, v9 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a face map for the hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 *  @param  face_map [out] pointer to the face map
 */
/*===========================================================================*/
inline void CreateHexahedraFaceMap(
    const kvs::UnstructuredVolumeObject* volume,
    QuadrangleFaceMap* face_map )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        // Local vertices of the quadratic tetrahedral cell.
        const kvs::UInt32 v0 = connections[ connection_index     ];
        const kvs::UInt32 v1 = connections[ connection_index + 1 ];
        const kvs::UInt32 v2 = connections[ connection_index + 2 ];
        const kvs::UInt32 v3 = connections[ connection_index + 3 ];
        const kvs::UInt32 v4 = connections[ connection_index + 4 ];
        const kvs::UInt32 v5 = connections[ connection_index + 5 ];
        const kvs::UInt32 v6 = connections[ connection_index + 6 ];
        const kvs::UInt32 v7 = connections[ connection_index + 7 ];
        connection_index += 8;

        // Local faces of the cell (6 quadrangle meshes).
        face_map->insert( v0, v1, v2, v3 );
        face_map->insert( v4, v5, v6, v7 );
        face_map->insert( v0, v3, v7, v4 );
        face_map->insert( v3, v2, v6, v7 );
        face_map->insert( v1, v2, v6, v5 );
        face_map->insert( v0, v1, v5, v4 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a face map for the quadratic hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 *  @param  face_map [out] pointer to the face map
 */
/*===========================================================================*/
inline void CreateQuadraticHexahedraFaceMap(
    const kvs::UnstructuredVolumeObject* volume,
    QuadrangleFaceMap* face_map )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        // Local vertices of the quadratic tetrahedral cell.
        const kvs::UInt32 v0  = connections[ connection_index      ];
        const kvs::UInt32 v1  = connections[ connection_index +  1 ];
        const kvs::UInt32 v2  = connections[ connection_index +  2 ];
        const kvs::UInt32 v3  = connections[ connection_index +  3 ];
        const kvs::UInt32 v4  = connections[ connection_index +  4 ];
        const kvs::UInt32 v5  = connections[ connection_index +  5 ];
        const kvs::UInt32 v6  = connections[ connection_index +  6 ];
        const kvs::UInt32 v7  = connections[ connection_index +  7 ];
        // TO DO: Ingore the quadratic nodes...
        /*
        const kvs::UInt32 v8  = connections[ connection_index +  8 ];
        const kvs::UInt32 v9  = connections[ connection_index +  9 ];
        const kvs::UInt32 v10 = connections[ connection_index + 10 ];
        const kvs::UInt32 v11 = connections[ connection_index + 11 ];
        const kvs::UInt32 v12 = connections[ connection_index + 12 ];
        const kvs::UInt32 v13 = connections[ connection_index + 13 ];
        const kvs::UInt32 v14 = connections[ connection_index + 14 ];
        const kvs::UInt32 v15 = connections[ connection_index + 15 ];
        const kvs::UInt32 v16 = connections[ connection_index + 16 ];
        const kvs::UInt32 v17 = connections[ connection_index + 17 ];
        const kvs::UInt32 v18 = connections[ connection_index + 18 ];
        const kvs::UInt32 v19 = connections[ connection_index + 19 ];
        */
        connection_index += 20;

        // Local faces of the cell (6 quadrangle meshes).
        face_map->insert( v0, v1, v2, v3 );
        face_map->insert( v4, v5, v6, v7 );
        face_map->insert( v0, v3, v7, v4 );
        face_map->insert( v3, v2, v6, v7 );
        face_map->insert( v1, v2, v6, v5 );
        face_map->insert( v0, v1, v5, v4 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates external faces using the face map.
 *  @param  volume [in] pointer to the unstructured volume object
 *  @param  cmap [in] color map
 *  @param  face_map [in] face map
 *  @param  coords [out] pointer to the coordinate value array
 *  @param  colors [out] pointer to the color value array
 *  @param  normals [out] pointer to the normal vector array
 */
/*===========================================================================*/
template <typename T>
void CalculateFaces(
    const kvs::UnstructuredVolumeObject* volume,
    const kvs::ColorMap cmap,
    const TriangleFaceMap& face_map,
    kvs::ValueArray<kvs::Real32>* coords,
    kvs::ValueArray<kvs::UInt8>* colors,
    kvs::ValueArray<kvs::Real32>* normals )
{
    // Parameters of the volume data.
    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real64 min_value = volume->minValue();
    const kvs::Real64 max_value = volume->maxValue();
    const size_t veclen = volume->veclen();
    const T* value = reinterpret_cast<const T*>( volume->values().data() );

    const size_t nfaces = face_map.bucket().size();
    const size_t nvertices = nfaces * 3;
    const kvs::Real32* volume_coord = volume->coords().data();

    coords->allocate( nvertices * 3 );
    colors->allocate( nvertices * 3 );
    normals->allocate( nfaces * 3 );
    kvs::Real32* coord = coords->data();
    kvs::UInt8* color = colors->data();
    kvs::Real32* normal = normals->data();

    kvs::UInt32 node_index[3] = { 0, 0, 0 };
    kvs::UInt32 color_level[3] = { 0, 0, 0 };

    TriangleFaceMap::Bucket::const_iterator f = face_map.bucket().begin();
    TriangleFaceMap::Bucket::const_iterator last = face_map.bucket().end();
    while ( f != last )
    {
        node_index[0] = f->second.id(0);
        node_index[1] = f->second.id(1);
        node_index[2] = f->second.id(2);

        const kvs::Vec3 v0( volume_coord + 3 * node_index[0] );
        const kvs::Vec3 v1( volume_coord + 3 * node_index[1] );
        const kvs::Vec3 v2( volume_coord + 3 * node_index[2] );
        // v0
        *( coord++ ) = v0.x();
        *( coord++ ) = v0.y();
        *( coord++ ) = v0.z();
        // v1
        *( coord++ ) = v1.x();
        *( coord++ ) = v1.y();
        *( coord++ ) = v1.z();
        // v2
        *( coord++ ) = v2.x();
        *( coord++ ) = v2.y();
        *( coord++ ) = v2.z();

        GetColorIndices<3>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
        // c0
        *( color++ ) = cmap[ color_level[0] ].r();
        *( color++ ) = cmap[ color_level[0] ].g();
        *( color++ ) = cmap[ color_level[0] ].b();
        // c1
        *( color++ ) = cmap[ color_level[1] ].r();
        *( color++ ) = cmap[ color_level[1] ].g();
        *( color++ ) = cmap[ color_level[1] ].b();
        // c2
        *( color++ ) = cmap[ color_level[2] ].r();
        *( color++ ) = cmap[ color_level[2] ].g();
        *( color++ ) = cmap[ color_level[2] ].b();

        const kvs::Vec3 n( ( v1 - v0 ).cross( v2 - v0 ) );
        // n0
        *( normal++ ) = n.x();
        *( normal++ ) = n.y();
        *( normal++ ) = n.z();

        f++;
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates external faces using the face map.
 *  @param  volume [in] pointer to the unstructured volume object
 *  @param  cmap [in] color map
 *  @param  face_map [in] face map
 *  @param  coords [out] pointer to the coordinate value array
 *  @param  colors [out] pointer to the color value array
 *  @param  normals [out] pointer to the normal vector array
 */
/*===========================================================================*/
template <typename T>
void CalculateFaces(
    const kvs::UnstructuredVolumeObject* volume,
    const kvs::ColorMap cmap,
    const QuadrangleFaceMap& face_map,
    kvs::ValueArray<kvs::Real32>* coords,
    kvs::ValueArray<kvs::UInt8>* colors,
    kvs::ValueArray<kvs::Real32>* normals )
{
    // Parameters of the volume data.
    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real64 min_value = volume->minValue();
    const kvs::Real64 max_value = volume->maxValue();
    const size_t veclen = volume->veclen();
    const T* value = reinterpret_cast<const T*>( volume->values().data() );

    // A quadrangle face is composed of two triangle faces
    const size_t nfaces = face_map.bucket().size() * 2;
//    const size_t nvertices = nfaces * 4;
    const size_t nvertices = nfaces * 3;
    const kvs::Real32* volume_coord = volume->coords().data();

    coords->allocate( nvertices * 3 );
    colors->allocate( nvertices * 3 );
    normals->allocate( nfaces * 3 );
    kvs::Real32* coord = coords->data();
    kvs::UInt8* color = colors->data();
    kvs::Real32* normal = normals->data();

    kvs::UInt32 node_index[4] = { 0, 0, 0, 0 };
    kvs::UInt32 color_level[4] = { 0, 0, 0, 0 };

    QuadrangleFaceMap::Bucket::const_iterator f = face_map.bucket().begin();
    QuadrangleFaceMap::Bucket::const_iterator last = face_map.bucket().end();
    while ( f != last )
    {
        node_index[0] = f->second.id(0);
        node_index[1] = f->second.id(1);
        node_index[2] = f->second.id(2);
        node_index[3] = f->second.id(3);

        const kvs::Vec3 v0( volume_coord + 3 * node_index[0] );
        const kvs::Vec3 v1( volume_coord + 3 * node_index[1] );
        const kvs::Vec3 v2( volume_coord + 3 * node_index[2] );
        const kvs::Vec3 v3( volume_coord + 3 * node_index[3] );
        // v0
        *( coord++ ) = v0.x();
        *( coord++ ) = v0.y();
        *( coord++ ) = v0.z();
        // v1
        *( coord++ ) = v1.x();
        *( coord++ ) = v1.y();
        *( coord++ ) = v1.z();
        // v2
        *( coord++ ) = v2.x();
        *( coord++ ) = v2.y();
        *( coord++ ) = v2.z();

        // v2
        *( coord++ ) = v2.x();
        *( coord++ ) = v2.y();
        *( coord++ ) = v2.z();
        // v3
        *( coord++ ) = v3.x();
        *( coord++ ) = v3.y();
        *( coord++ ) = v3.z();
        // v0
        *( coord++ ) = v0.x();
        *( coord++ ) = v0.y();
        *( coord++ ) = v0.z();

        GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
        // c0
        *( color++ ) = cmap[ color_level[0] ].r();
        *( color++ ) = cmap[ color_level[0] ].g();
        *( color++ ) = cmap[ color_level[0] ].b();
        // c1
        *( color++ ) = cmap[ color_level[1] ].r();
        *( color++ ) = cmap[ color_level[1] ].g();
        *( color++ ) = cmap[ color_level[1] ].b();
        // c2
        *( color++ ) = cmap[ color_level[2] ].r();
        *( color++ ) = cmap[ color_level[2] ].g();
        *( color++ ) = cmap[ color_level[2] ].b();

        // c2
        *( color++ ) = cmap[ color_level[2] ].r();
        *( color++ ) = cmap[ color_level[2] ].g();
        *( color++ ) = cmap[ color_level[2] ].b();
        // c3
        *( color++ ) = cmap[ color_level[3] ].r();
        *( color++ ) = cmap[ color_level[3] ].g();
        *( color++ ) = cmap[ color_level[3] ].b();
        // c0
        *( color++ ) = cmap[ color_level[0] ].r();
        *( color++ ) = cmap[ color_level[0] ].g();
        *( color++ ) = cmap[ color_level[0] ].b();

        const kvs::Vec3 n( ( v1 - v0 ).cross( v2 - v0 ) );
        // n0
        *( normal++ ) = n.x();
        *( normal++ ) = n.y();
        *( normal++ ) = n.z();

        // n0
        *( normal++ ) = n.x();
        *( normal++ ) = n.y();
        *( normal++ ) = n.z();

        f++;
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExternalFaces class.
 */
/*===========================================================================*/
ExternalFaces::ExternalFaces():
    kvs::MapperBase(),
    kvs::PolygonObject()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExternalFaces class.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
ExternalFaces::ExternalFaces( const kvs::VolumeObjectBase* volume ):
    kvs::MapperBase(),
    kvs::PolygonObject()
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExternalFaces class.
 *  @param  volume [in] pointer to the structured volume object
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
ExternalFaces::ExternalFaces(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PolygonObject()
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ExternalFaces class.
 */
/*===========================================================================*/
ExternalFaces::~ExternalFaces()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the edge extraction.
 *  @param  object [in] pointer of the volume object
 *  @return pointer of the line object
 */
/*===========================================================================*/
ExternalFaces::SuperClass* ExternalFaces::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    const kvs::VolumeObjectBase::VolumeType type = volume->volumeType();
    if ( type == kvs::VolumeObjectBase::Structured )
    {
        this->mapping( kvs::StructuredVolumeObject::DownCast( volume ) );
    }
    else // type == kvs::VolumeObjectBase::Unstructured
    {
        this->mapping( kvs::UnstructuredVolumeObject::DownCast( volume ) );
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Mapping for a structured volume object.
 *  @param  volume [in] pointer to the strctured volume object
 */
/*===========================================================================*/
void ExternalFaces::mapping( const kvs::StructuredVolumeObject* volume )
{
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    this->calculate_coords( volume );

    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) { this->calculate_colors<kvs::Int8  >( volume ); }
    else if ( type == typeid( kvs::Int16  ) ) { this->calculate_colors<kvs::Int16 >( volume ); }
    else if ( type == typeid( kvs::Int32  ) ) { this->calculate_colors<kvs::Int32 >( volume ); }
    else if ( type == typeid( kvs::Int64  ) ) { this->calculate_colors<kvs::Int64 >( volume ); }
    else if ( type == typeid( kvs::UInt8  ) ) { this->calculate_colors<kvs::UInt8 >( volume ); }
    else if ( type == typeid( kvs::UInt16 ) ) { this->calculate_colors<kvs::UInt16>( volume ); }
    else if ( type == typeid( kvs::UInt32 ) ) { this->calculate_colors<kvs::UInt32>( volume ); }
    else if ( type == typeid( kvs::UInt64 ) ) { this->calculate_colors<kvs::UInt64>( volume ); }
    else if ( type == typeid( kvs::Real32 ) ) { this->calculate_colors<kvs::Real32>( volume ); }
    else if ( type == typeid( kvs::Real64 ) ) { this->calculate_colors<kvs::Real64>( volume ); }

//    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
    if ( SuperClass::numberOfOpacities() == 0 ) SuperClass::setOpacity( 255 );
}

/*===========================================================================*/
/**
 *  @brief  Calculates vertex values (coordinate, color and normal).
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExternalFaces::calculate_coords( const kvs::StructuredVolumeObject* volume )
{
    const kvs::StructuredVolumeObject::GridType type = volume->gridType();
    if ( type == kvs::StructuredVolumeObject::Uniform )
    {
        this->calculate_uniform_coords( volume );
    }
    else if ( type == kvs::StructuredVolumeObject::Rectilinear )
    {
        this->calculate_rectilinear_coords( volume );
    }
    else if ( type == kvs::StructuredVolumeObject::Curvilinear )
    {
        this->calculate_curvilinear_coords( volume );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Not supported grid type.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates vertex values of the uniform volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExternalFaces::calculate_uniform_coords( const kvs::StructuredVolumeObject* volume )
{
    const kvs::Vec3u resolution( volume->resolution() );
    const kvs::Vec3  volume_size( volume->maxObjectCoord() - volume->minObjectCoord() );
    const kvs::Vec3u ngrids( resolution - kvs::Vec3u( 1, 1, 1 ) );
    const kvs::Vec3  grid_size(
        volume_size.x() / static_cast<float>( ngrids.x() ),
        volume_size.y() / static_cast<float>( ngrids.y() ),
        volume_size.z() / static_cast<float>( ngrids.z() ) );

    const size_t nexternal_faces =
        ( 2 * ngrids.x() * ngrids.y() +
          2 * ngrids.y() * ngrids.z() +
          2 * ngrids.z() * ngrids.x() ) * 2;
//    const size_t nexternal_vertices = nexternal_faces * 4;
    const size_t nexternal_vertices = nexternal_faces * 3;

    kvs::ValueArray<kvs::Real32> coords( 3 * nexternal_vertices );
    kvs::Real32* coord = coords.data();

    kvs::ValueArray<kvs::Real32> normals( 3 * nexternal_faces );
    kvs::Real32* normal = normals.data();

    // XY (Z=0) plane.
    {
        const float z = 0.0f;
        const kvs::Vec3 n( 0.0f, 0.0f, -1.0f );
        for ( size_t j = 0; j < ngrids.y(); j++ )
        {
            const float y = grid_size.y() * static_cast<float>( j );
            for ( size_t i = 0; i < ngrids.x(); i++ )
            {
                const float x = grid_size.x() * static_cast<float>( i );
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;
                // v2
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;
                // v1
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z;

                // v1
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // XY (Z=ngrids.z()) plane.
    {
        const float z = grid_size.z() * static_cast<float>( ngrids.z() );
        const kvs::Vec3 n( 0.0f, 0.0f, 1.0f );
        for ( size_t j = 0; j < ngrids.y(); j++ )
        {
            const float y = grid_size.y() * static_cast<float>( j );
            for ( size_t i = 0; i < ngrids.x(); i++ )
            {
                const float x = grid_size.x() * static_cast<float>( i );
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v1
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v2
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;

                // v2
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // YZ (X=0) plane.
    {
        const float x = 0.0f;
        const kvs::Vec3 n( -1.0f, 0.0f, 0.0f );
        for ( size_t j = 0; j < ngrids.y(); j++ )
        {
            const float y = grid_size.y() * static_cast<float>( j );
            for ( size_t k = 0; k < ngrids.z(); k++ )
            {
                const float z = grid_size.z() * static_cast<float>( k );
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v1
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();
                // v2
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z + grid_size.z();

                // v2
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z + grid_size.z();
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // YZ (X=ngrids.x()) plane.
    {
        const float x = grid_size.y() * static_cast<float>( ngrids.x() );
        const kvs::Vec3 n( 1.0f, 0.0f, 0.0f );
        for ( size_t j = 0; j < ngrids.y(); j++ )
        {
            const float y = grid_size.y() * static_cast<float>( j );
            for ( size_t k = 0; k < ngrids.z(); k++ )
            {
                const float z = grid_size.z() * static_cast<float>( k );
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;
                // v2
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z + grid_size.z();
                // v1
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();

                // v1
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y + grid_size.y();
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // XZ (Y=0) plane.
    {
        const float y = 0.0f;
        const kvs::Vec3 n( 0.0f, -1.0f, 0.0f );
        for ( size_t k = 0; k < ngrids.z(); k++ )
        {
            const float z = grid_size.z() * static_cast<float>( k );
            for ( size_t i = 0; i < ngrids.x(); i++ )
            {
                const float x = grid_size.x() * static_cast<float>( i );
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v1
                *( coord++ ) = x + grid_size.z();
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v2
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();

                // v2
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // XZ (Y=ngrids.y()) plane.
    {
        const float y = grid_size.y() * static_cast<float>( ngrids.y() );
        const kvs::Vec3 n( 0.0f, 1.0f, 0.0f );
        for ( size_t k = 0; k < ngrids.z(); k++ )
        {
            const float z = grid_size.z() * static_cast<float>( k );
            for ( size_t i = 0; i < ngrids.x(); i++ )
            {
                const float x = grid_size.x() * static_cast<float>( i );
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();
                // v2
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();
                // v1
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z;

                // v1
                *( coord++ ) = x + grid_size.x();
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z + grid_size.z();

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    SuperClass::setCoords( coords );
    SuperClass::setNormals( normals );
}

/*===========================================================================*/
/**
 *  @brief  Calculates vertex values of the rectiliear volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExternalFaces::calculate_rectilinear_coords( const kvs::StructuredVolumeObject* volume )
{
    const size_t dimx = volume->resolution().x();
    const size_t dimy = volume->resolution().y();
    const size_t dimz = volume->resolution().z();
    const kvs::Real32* xcoords = volume->coords().data();
    const kvs::Real32* ycoords = xcoords + dimx;
    const kvs::Real32* zcoords = ycoords + dimy;

    const size_t nfaces =
        ( 2 * ( dimx - 1 ) * ( dimy - 1 ) +
          2 * ( dimy - 1 ) * ( dimz - 1 ) +
          2 * ( dimz - 1 ) * ( dimx - 1 ) ) * 2;
//    const size_t nvertices = nfaces * 4;
    const size_t nvertices = nfaces * 3;

    kvs::ValueArray<kvs::Real32> coords( 3 * nvertices );
    kvs::Real32* coord = coords.data();

    kvs::ValueArray<kvs::Real32> normals( 3 * nfaces );
    kvs::Real32* normal = normals.data();

    // XY (Z=zcoords[0]) plane.
    {
        const float z = zcoords[0];
        const kvs::Vec3 n( 0.0f, 0.0f, -1.0f );
        for ( size_t j = 0; j < dimy - 1; j++ )
        {
            const float y0 = ycoords[j];
            const float y1 = ycoords[j+1];
            for ( size_t i = 0; i < dimx - 1; i++ )
            {
                const float x0 = xcoords[i];
                const float x1 = xcoords[i+1];
                // v3
                *( coord++ ) = x0;
                *( coord++ ) = y1;
                *( coord++ ) = z;
                // v2
                *( coord++ ) = x1;
                *( coord++ ) = y1;
                *( coord++ ) = z;
                // v1
                *( coord++ ) = x1;
                *( coord++ ) = y0;
                *( coord++ ) = z;

                // v1
                *( coord++ ) = x1;
                *( coord++ ) = y0;
                *( coord++ ) = z;
                // v0
                *( coord++ ) = x0;
                *( coord++ ) = y0;
                *( coord++ ) = z;
                // v3
                *( coord++ ) = x0;
                *( coord++ ) = y1;
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // XY (Z=zcoords[dimz-1]) plane.
    {
        const float z = zcoords[dimz-1];
        const kvs::Vec3 n( 0.0f, 0.0f, 1.0f );
        for ( size_t j = 0; j < dimy - 1; j++ )
        {
            const float y0 = ycoords[j];
            const float y1 = ycoords[j+1];
            for ( size_t i = 0; i < dimx - 1; i++ )
            {
                const float x0 = xcoords[i];
                const float x1 = xcoords[i+1];
                // v0
                *( coord++ ) = x0;
                *( coord++ ) = y0;
                *( coord++ ) = z;
                // v1
                *( coord++ ) = x1;
                *( coord++ ) = y0;
                *( coord++ ) = z;
                // v2
                *( coord++ ) = x1;
                *( coord++ ) = y1;
                *( coord++ ) = z;

                // v2
                *( coord++ ) = x1;
                *( coord++ ) = y1;
                *( coord++ ) = z;
                // v3
                *( coord++ ) = x0;
                *( coord++ ) = y1;
                *( coord++ ) = z;
                // v0
                *( coord++ ) = x0;
                *( coord++ ) = y0;
                *( coord++ ) = z;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // YZ (X=xcoords[0]) plane.
    {
        const float x = xcoords[0];
        const kvs::Vec3 n( -1.0f, 0.0f, 0.0f );
        for ( size_t j = 0; j < dimy - 1; j++ )
        {
            const float y0 = ycoords[j];
            const float y1 = ycoords[j+1];
            for ( size_t k = 0; k < dimz - 1; k++ )
            {
                const float z0 = zcoords[k];
                const float z1 = zcoords[k+1];
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y0;
                *( coord++ ) = z0;
                // v1
                *( coord++ ) = x;
                *( coord++ ) = y0;
                *( coord++ ) = z1;
                // v2
                *( coord++ ) = x;
                *( coord++ ) = y1;
                *( coord++ ) = z1;

                // v2
                *( coord++ ) = x;
                *( coord++ ) = y1;
                *( coord++ ) = z1;
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y1;
                *( coord++ ) = z0;
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y0;
                *( coord++ ) = z0;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // YZ (X=xcoords[dimx-1]) plane.
    {
        const float x = xcoords[dimx-1];
        const kvs::Vec3 n( 1.0f, 0.0f, 0.0f );
        for ( size_t j = 0; j < dimy - 1; j++ )
        {
            const float y0 = ycoords[j];
            const float y1 = ycoords[j+1];
            for ( size_t k = 0; k < dimz - 1; k++ )
            {
                const float z0 = zcoords[k];
                const float z1 = zcoords[k+1];
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y1;
                *( coord++ ) = z0;
                // v2
                *( coord++ ) = x;
                *( coord++ ) = y1;
                *( coord++ ) = z1;
                // v1
                *( coord++ ) = x;
                *( coord++ ) = y0;
                *( coord++ ) = z1;

                // v1
                *( coord++ ) = x;
                *( coord++ ) = y0;
                *( coord++ ) = z1;
                // v0
                *( coord++ ) = x;
                *( coord++ ) = y0;
                *( coord++ ) = z0;
                // v3
                *( coord++ ) = x;
                *( coord++ ) = y1;
                *( coord++ ) = z0;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // XZ (Y=ycoords[0]) plane.
    {
        const float y = ycoords[0];
        const kvs::Vec3 n( 0.0f, -1.0f, 0.0f );
        for ( size_t k = 0; k < dimz - 1; k++ )
        {
            const float z0 = zcoords[k];
            const float z1 = zcoords[k+1];
            for ( size_t i = 0; i < dimx - 1; i++ )
            {
                const float x0 = xcoords[i];
                const float x1 = xcoords[i+1];
                // v0
                *( coord++ ) = x0;
                *( coord++ ) = y;
                *( coord++ ) = z0;
                // v1
                *( coord++ ) = x1;
                *( coord++ ) = y;
                *( coord++ ) = z0;
                // v2
                *( coord++ ) = x1;
                *( coord++ ) = y;
                *( coord++ ) = z1;

                // v2
                *( coord++ ) = x1;
                *( coord++ ) = y;
                *( coord++ ) = z1;
                // v3
                *( coord++ ) = x0;
                *( coord++ ) = y;
                *( coord++ ) = z1;
                // v0
                *( coord++ ) = x0;
                *( coord++ ) = y;
                *( coord++ ) = z0;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    // XZ (Y=ycoords[dimy-1]) plane.
    {
        const float y = ycoords[dimy-1];
        const kvs::Vec3 n( 0.0f, 1.0f, 0.0f );
        for ( size_t k = 0; k < dimz - 1; k++ )
        {
            const float z0 = zcoords[k];
            const float z1 = zcoords[k+1];
            for ( size_t i = 0; i < dimx - 1; i++ )
            {
                const float x0 = xcoords[i];
                const float x1 = xcoords[i+1];
                // v3
                *( coord++ ) = x0;
                *( coord++ ) = y;
                *( coord++ ) = z1;
                // v2
                *( coord++ ) = x1;
                *( coord++ ) = y;
                *( coord++ ) = z1;
                // v1
                *( coord++ ) = x1;
                *( coord++ ) = y;
                *( coord++ ) = z0;

                // v1
                *( coord++ ) = x1;
                *( coord++ ) = y;
                *( coord++ ) = z0;
                // v0
                *( coord++ ) = x0;
                *( coord++ ) = y;
                *( coord++ ) = z0;
                // v3
                *( coord++ ) = x0;
                *( coord++ ) = y;
                *( coord++ ) = z1;

                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
                // n0
                *( normal++ ) = n.x();
                *( normal++ ) = n.y();
                *( normal++ ) = n.z();
            }
        }
    }

    SuperClass::setCoords( coords );
    SuperClass::setNormals( normals );
}

/*===========================================================================*/
/**
 *  @brief  Calculates vertex values of the curvilinear volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExternalFaces::calculate_curvilinear_coords( const kvs::StructuredVolumeObject* volume )
{
    kvs::IgnoreUnusedVariable( volume );

    BaseClass::setSuccess( false );
    kvsMessageError("Curvilinear volume has not yet support.");
}

/*===========================================================================*/
/**
 *  @brief  Calculates color values.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
void ExternalFaces::calculate_colors( const kvs::StructuredVolumeObject* volume )
{
    // Parameters of the volume data.
    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real64 min_value = volume->minValue();
    const kvs::Real64 max_value = volume->maxValue();
    const size_t veclen = volume->veclen();
    const size_t nnodes_per_line = volume->numberOfNodesPerLine();
    const size_t nnodes_per_slice = volume->numberOfNodesPerSlice();
    const kvs::Vec3u resolution( volume->resolution() );
    const kvs::Vec3u ngrids( resolution - kvs::Vec3u( 1, 1, 1 ) );
    const T* value = reinterpret_cast<const T*>( volume->values().data() );

    const size_t nexternal_faces =
        ( 2 * ngrids.x() * ngrids.y() +
          2 * ngrids.y() * ngrids.z() +
          2 * ngrids.z() * ngrids.x() ) * 2;
//    const size_t nexternal_vertices = nexternal_faces * 4;
    const size_t nexternal_vertices = nexternal_faces * 3;

    const kvs::ColorMap cmap( BaseClass::colorMap() );

    kvs::ValueArray<kvs::UInt8> colors( 3 * nexternal_vertices );
    kvs::UInt8* color = colors.data();

    kvs::UInt32 node_index[4];
    kvs::UInt32 color_level[4];

    // XY (Z=0) plane.
    {
        const size_t k = 0;
        const size_t offset0 = k * nnodes_per_line;
        for ( size_t j = 0, offset = offset0; j < ngrids.y(); j++, offset = offset0 + j * nnodes_per_line )
        {
            for ( size_t i = 0; i < ngrids.x(); i++, offset += 1 )
            {
                node_index[0] = offset;
                node_index[1] = node_index[0] + 1;
                node_index[2] = node_index[1] + nnodes_per_line;
                node_index[3] = node_index[0] + nnodes_per_line;
                ::GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();
                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();

                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
            }
        }
    }

    // XY (Z=ngrids.z()) plane.
    {
        const size_t k = ngrids.z();
        const size_t offset0 = k * nnodes_per_slice;
        for ( size_t j = 0, offset = offset0; j < ngrids.y(); j++, offset = offset0 + j * nnodes_per_line )
        {
            for ( size_t i = 0; i < ngrids.x(); i++, offset += 1 )
            {
                node_index[0] = offset;
                node_index[1] = node_index[0] + 1;
                node_index[2] = node_index[1] + nnodes_per_line;
                node_index[3] = node_index[0] + nnodes_per_line;
                ::GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();

                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
            }
        }
    }

    // YZ (X=0) plane.
    {
        const size_t i = 0;
        const size_t offset0 = i;
        for ( size_t j = 0, offset = offset0; j < ngrids.y(); j++, offset = offset0 + j * nnodes_per_line )
        {
            for ( size_t k = 0; k < ngrids.z(); k++, offset += nnodes_per_slice )
            {
                node_index[0] = offset;
                node_index[1] = node_index[0] + nnodes_per_slice;
                node_index[2] = node_index[1] + nnodes_per_line;
                node_index[3] = node_index[0] + nnodes_per_line;
                ::GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();

                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
            }
        }
    }

    // YZ (X=ngrids.x()) plane.
    {
        const size_t i = ngrids.x();
        const size_t offset0 = i;
        for ( size_t j = 0, offset = offset0; j < ngrids.y(); j++, offset = offset0 + j * nnodes_per_line )
        {
            for ( size_t k = 0; k < ngrids.z(); k++, offset += nnodes_per_slice )
            {
                node_index[0] = offset;
                node_index[1] = node_index[0] + nnodes_per_slice;
                node_index[2] = node_index[1] + nnodes_per_line;
                node_index[3] = node_index[0] + nnodes_per_line;
                ::GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();
                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();

                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
            }
        }
    }

    // XZ (Y=0) plane.
    {
        const size_t j = 0;
        const size_t offset0 = j * nnodes_per_line;
        for ( size_t k = 0, offset = offset0; k < ngrids.z(); k++, offset =  offset0 + k * nnodes_per_slice )
        {
            for ( size_t i = 0; i < ngrids.x(); i++, offset += 1 )
            {
                node_index[0] = offset;
                node_index[1] = node_index[0] + 1;
                node_index[2] = node_index[1] + nnodes_per_slice;
                node_index[3] = node_index[0] + nnodes_per_slice;
                ::GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();

                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
            }
        }
    }

    // XZ (Y=ngrids.y()) plane.
    {
        const size_t j = ngrids.y();
        const size_t offset0 = j * nnodes_per_line;
        for ( size_t k = 0, offset = offset0; k < ngrids.z(); k++, offset = offset0 + k * nnodes_per_slice )
        {
            for ( size_t i = 0; i < ngrids.x(); i++, offset += 1 )
            {
                node_index[0] = offset;
                node_index[1] = node_index[0] + 1;
                node_index[2] = node_index[1] + nnodes_per_slice;
                node_index[3] = node_index[0] + nnodes_per_slice;
                ::GetColorIndices<4>( value, min_value, max_value, veclen, cmap.resolution(), node_index, &color_level );
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
                // v2
                *( color++ ) = cmap[ color_level[2] ].r();
                *( color++ ) = cmap[ color_level[2] ].g();
                *( color++ ) = cmap[ color_level[2] ].b();
                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();

                // v1
                *( color++ ) = cmap[ color_level[1] ].r();
                *( color++ ) = cmap[ color_level[1] ].g();
                *( color++ ) = cmap[ color_level[1] ].b();
                // v0
                *( color++ ) = cmap[ color_level[0] ].r();
                *( color++ ) = cmap[ color_level[0] ].g();
                *( color++ ) = cmap[ color_level[0] ].b();
                // v3
                *( color++ ) = cmap[ color_level[3] ].r();
                *( color++ ) = cmap[ color_level[3] ].g();
                *( color++ ) = cmap[ color_level[3] ].b();
            }
        }
    }

    SuperClass::setColors( colors );
}

/*===========================================================================*/
/**
 *  @brief  Mapping for a unstructured volume object.
 *  @param  volume [in] pointer to the unstrctured volume object
 */
/*===========================================================================*/
void ExternalFaces::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        const std::type_info& type = volume->values().typeInfo()->type();
        if (      type == typeid( kvs::Int8   ) ) { this->calculate_tetrahedral_faces<kvs::Int8  >( volume ); }
        else if ( type == typeid( kvs::Int16  ) ) { this->calculate_tetrahedral_faces<kvs::Int16 >( volume ); }
        else if ( type == typeid( kvs::Int32  ) ) { this->calculate_tetrahedral_faces<kvs::Int32 >( volume ); }
        else if ( type == typeid( kvs::Int64  ) ) { this->calculate_tetrahedral_faces<kvs::Int64 >( volume ); }
        else if ( type == typeid( kvs::UInt8  ) ) { this->calculate_tetrahedral_faces<kvs::UInt8 >( volume ); }
        else if ( type == typeid( kvs::UInt16 ) ) { this->calculate_tetrahedral_faces<kvs::UInt16>( volume ); }
        else if ( type == typeid( kvs::UInt32 ) ) { this->calculate_tetrahedral_faces<kvs::UInt32>( volume ); }
        else if ( type == typeid( kvs::UInt64 ) ) { this->calculate_tetrahedral_faces<kvs::UInt64>( volume ); }
        else if ( type == typeid( kvs::Real32 ) ) { this->calculate_tetrahedral_faces<kvs::Real32>( volume ); }
        else if ( type == typeid( kvs::Real64 ) ) { this->calculate_tetrahedral_faces<kvs::Real64>( volume ); }
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        const std::type_info& type = volume->values().typeInfo()->type();
        if (      type == typeid( kvs::Int8   ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::Int8  >( volume ); }
        else if ( type == typeid( kvs::Int16  ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::Int16 >( volume ); }
        else if ( type == typeid( kvs::Int32  ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::Int32 >( volume ); }
        else if ( type == typeid( kvs::Int64  ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::Int64 >( volume ); }
        else if ( type == typeid( kvs::UInt8  ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::UInt8 >( volume ); }
        else if ( type == typeid( kvs::UInt16 ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::UInt16>( volume ); }
        else if ( type == typeid( kvs::UInt32 ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::UInt32>( volume ); }
        else if ( type == typeid( kvs::UInt64 ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::UInt64>( volume ); }
        else if ( type == typeid( kvs::Real32 ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::Real32>( volume ); }
        else if ( type == typeid( kvs::Real64 ) ) { this->calculate_quadratic_tetrahedral_faces<kvs::Real64>( volume ); }
        break;
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        const std::type_info& type = volume->values().typeInfo()->type();
        if (      type == typeid( kvs::Int8   ) ) { this->calculate_hexahedral_faces<kvs::Int8  >( volume ); }
        else if ( type == typeid( kvs::Int16  ) ) { this->calculate_hexahedral_faces<kvs::Int16 >( volume ); }
        else if ( type == typeid( kvs::Int32  ) ) { this->calculate_hexahedral_faces<kvs::Int32 >( volume ); }
        else if ( type == typeid( kvs::Int64  ) ) { this->calculate_hexahedral_faces<kvs::Int64 >( volume ); }
        else if ( type == typeid( kvs::UInt8  ) ) { this->calculate_hexahedral_faces<kvs::UInt8 >( volume ); }
        else if ( type == typeid( kvs::UInt16 ) ) { this->calculate_hexahedral_faces<kvs::UInt16>( volume ); }
        else if ( type == typeid( kvs::UInt32 ) ) { this->calculate_hexahedral_faces<kvs::UInt32>( volume ); }
        else if ( type == typeid( kvs::UInt64 ) ) { this->calculate_hexahedral_faces<kvs::UInt64>( volume ); }
        else if ( type == typeid( kvs::Real32 ) ) { this->calculate_hexahedral_faces<kvs::Real32>( volume ); }
        else if ( type == typeid( kvs::Real64 ) ) { this->calculate_hexahedral_faces<kvs::Real64>( volume ); }
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        const std::type_info& type = volume->values().typeInfo()->type();
        if (      type == typeid( kvs::Int8   ) ) { this->calculate_quadratic_hexahedral_faces<kvs::Int8  >( volume ); }
        else if ( type == typeid( kvs::Int16  ) ) { this->calculate_quadratic_hexahedral_faces<kvs::Int16 >( volume ); }
        else if ( type == typeid( kvs::Int32  ) ) { this->calculate_quadratic_hexahedral_faces<kvs::Int32 >( volume ); }
        else if ( type == typeid( kvs::Int64  ) ) { this->calculate_quadratic_hexahedral_faces<kvs::Int64 >( volume ); }
        else if ( type == typeid( kvs::UInt8  ) ) { this->calculate_quadratic_hexahedral_faces<kvs::UInt8 >( volume ); }
        else if ( type == typeid( kvs::UInt16 ) ) { this->calculate_quadratic_hexahedral_faces<kvs::UInt16>( volume ); }
        else if ( type == typeid( kvs::UInt32 ) ) { this->calculate_quadratic_hexahedral_faces<kvs::UInt32>( volume ); }
        else if ( type == typeid( kvs::UInt64 ) ) { this->calculate_quadratic_hexahedral_faces<kvs::UInt64>( volume ); }
        else if ( type == typeid( kvs::Real32 ) ) { this->calculate_quadratic_hexahedral_faces<kvs::Real32>( volume ); }
        else if ( type == typeid( kvs::Real64 ) ) { this->calculate_quadratic_hexahedral_faces<kvs::Real64>( volume ); }
        break;
    }
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Not supported cell type.");
        break;
    }
    }

    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
    if ( SuperClass::numberOfOpacities() == 0 ) SuperClass::setOpacity( 255 );
}

/*===========================================================================*/
/**
 *  @brief  Calculates external faces for the tetrahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
template <typename T>
void ExternalFaces::calculate_tetrahedral_faces( const kvs::UnstructuredVolumeObject* volume )
{
    ::TriangleFaceMap face_map( volume->numberOfNodes() );
    CreateTetrahedraFaceMap( volume, &face_map );

    kvs::ValueArray<kvs::Real32> coords;
    kvs::ValueArray<kvs::UInt8> colors;
    kvs::ValueArray<kvs::Real32> normals;
    ::CalculateFaces<T>( volume, BaseClass::colorMap(), face_map, &coords, &colors, &normals );

    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setCoords( coords );
    SuperClass::setColors( colors );
    SuperClass::setNormals( normals );
}

/*===========================================================================*/
/**
 *  @brief  Calculates external faces for the quadratic tetrahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
template <typename T>
void ExternalFaces::calculate_quadratic_tetrahedral_faces( const kvs::UnstructuredVolumeObject* volume )
{
    ::TriangleFaceMap face_map( volume->numberOfNodes() );
    CreateQuadraticTetrahedraFaceMap( volume, &face_map );

    kvs::ValueArray<kvs::Real32> coords;
    kvs::ValueArray<kvs::UInt8> colors;
    kvs::ValueArray<kvs::Real32> normals;
    ::CalculateFaces<T>( volume, BaseClass::colorMap(), face_map, &coords, &colors, &normals );

    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setCoords( coords );
    SuperClass::setColors( colors );
    SuperClass::setNormals( normals );
}

/*===========================================================================*/
/**
 *  @brief  Calculates external faces for the hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
template <typename T>
void ExternalFaces::calculate_hexahedral_faces( const kvs::UnstructuredVolumeObject* volume )
{
    ::QuadrangleFaceMap face_map( volume->numberOfNodes() );
    CreateHexahedraFaceMap( volume, &face_map );

    kvs::ValueArray<kvs::Real32> coords;
    kvs::ValueArray<kvs::UInt8> colors;
    kvs::ValueArray<kvs::Real32> normals;
    ::CalculateFaces<T>( volume, BaseClass::colorMap(), face_map, &coords, &colors, &normals );

//    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setCoords( coords );
    SuperClass::setColors( colors );
    SuperClass::setNormals( normals );
}

/*===========================================================================*/
/**
 *  @brief  Calculates external faces for the quadratic hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
template <typename T>
void ExternalFaces::calculate_quadratic_hexahedral_faces( const kvs::UnstructuredVolumeObject* volume )
{
    ::QuadrangleFaceMap face_map( volume->numberOfNodes() );
    CreateQuadraticHexahedraFaceMap( volume, &face_map );

    kvs::ValueArray<kvs::Real32> coords;
    kvs::ValueArray<kvs::UInt8> colors;
    kvs::ValueArray<kvs::Real32> normals;
    ::CalculateFaces<T>( volume, BaseClass::colorMap(), face_map, &coords, &colors, &normals );

//    SuperClass::setPolygonType( kvs::PolygonObject::Quadrangle );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setCoords( coords );
    SuperClass::setColors( colors );
    SuperClass::setNormals( normals );
}

} // end of namespace kvs
