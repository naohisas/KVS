/*****************************************************************************/
/**
 *  @file   ExtractEdges.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractEdges.cpp 1766 2014-05-10 15:22:50Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ExtractEdges.h"
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/TransferFunction>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Timer>
#include <map>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Edge map class (hash map table for the edge data).
 */
/*===========================================================================*/
class EdgeMap
{
public:

    typedef kvs::UInt32 Key;
    typedef std::pair<kvs::UInt32,kvs::UInt32> Value;
    typedef std::multimap<Key,Value> Bucket;

private:

    size_t m_nvertices; ///< number of vertices of the original data
    Bucket m_bucket; ///< bucket for the edge data

public:

    EdgeMap( const size_t nvertices );

public:

    void insert( const kvs::UInt32 v0, const kvs::UInt32 v1 );

    const kvs::ValueArray<kvs::UInt32> serialize();
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new EdgeMap class.
 *  @param  nvertices [in] number of vertices of the orignal volume data
 */
/*===========================================================================*/
EdgeMap::EdgeMap( const size_t nvertices ):
    m_nvertices( nvertices )
{
}

/*===========================================================================*/
/**
 *  @brief  Inserts an edge information (both end vertex)
 *  @param  v0 [in] vertex id 0
 *  @param  v1 [in] vertex id 1
 */
/*===========================================================================*/
void EdgeMap::insert( const kvs::UInt32 v0, const kvs::UInt32 v1 )
{
    const Key key = ( v0 + v1 ) % kvs::UInt32( m_nvertices );

    Bucket::iterator e = m_bucket.find( key );
    Bucket::const_iterator last = m_bucket.end();
    if ( e != last )
    {
        Bucket::const_iterator upper = m_bucket.upper_bound( key );
        while ( e != upper )
        {
            if ( ( e->second.first == v0 && e->second.second == v1 ) ||
                 ( e->second.first == v1 && e->second.second == v0 ) )
            {
                // The edge has been already inserted in the bucket.
                return;
            }
            e++;
        }
    }

    m_bucket.insert( std::make_pair( key, std::make_pair( v0, v1 ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Serializes the edge map.
 *  @return serialized indices of the end vertices of the edges in the edge map
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::UInt32> EdgeMap::serialize()
{
    kvs::ValueArray<kvs::UInt32> connections( 2 * m_bucket.size() );

    Bucket::const_iterator e = m_bucket.begin();
    Bucket::const_iterator last = m_bucket.end();
    size_t connection_index = 0;
    while ( e != last )
    {
        connections[ connection_index++ ] = e->second.first;
        connections[ connection_index++ ] = e->second.second;
        e++;
    }

    return connections;
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExtractEdges class.
 */
/*===========================================================================*/
ExtractEdges::ExtractEdges():
    kvs::MapperBase(),
    kvs::LineObject()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExtractEdges class.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
ExtractEdges::ExtractEdges( const kvs::VolumeObjectBase* volume ):
    kvs::MapperBase(),
    kvs::LineObject()
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExtractEdges class.
 *  @param  volume [in] pointer to the structured volume object
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
ExtractEdges::ExtractEdges(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::LineObject()
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ExtractEdges class.
 */
/*===========================================================================*/
ExtractEdges::~ExtractEdges()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the edge extraction.
 *  @param  object [in] pointer of the volume object
 *  @return pointer of the line object
 */
/*===========================================================================*/
ExtractEdges::SuperClass* ExtractEdges::exec( const kvs::ObjectBase* object )
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
void ExtractEdges::mapping( const kvs::StructuredVolumeObject* volume )
{
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    this->calculate_coords( volume );
    this->calculate_connections( volume );

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

    SuperClass::setLineType( kvs::LineObject::Segment );
    SuperClass::setColorType( kvs::LineObject::VertexColor );
    if ( SuperClass::numberOfSizes() == 0 ) SuperClass::setSize( 1.0f );
}

/*===========================================================================*/
/**
 *  @brief  Calculates coordinate values.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_coords( const kvs::StructuredVolumeObject* volume )
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
    else
    {
        SuperClass::setCoords( volume->coords() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates coordinate values of the uniform volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_uniform_coords( const kvs::StructuredVolumeObject* volume )
{
    kvs::ValueArray<kvs::Real32> coords( 3 * volume->numberOfNodes() );
    kvs::Real32* coord = coords.data();

    const kvs::Vector3ui resolution( volume->resolution() );
    const kvs::Vector3f  volume_size( volume->maxObjectCoord() - volume->minObjectCoord() );
    const kvs::Vector3ui ngrids( resolution - kvs::Vector3ui( 1, 1, 1 ) );
    const kvs::Vector3f  grid_size(
        volume_size.x() / static_cast<float>( ngrids.x() ),
        volume_size.y() / static_cast<float>( ngrids.y() ),
        volume_size.z() / static_cast<float>( ngrids.z() ) );

    for ( size_t k = 0; k < resolution.z(); ++k )
    {
        const float z =
            grid_size.z() * static_cast<float>( k );
        for ( size_t j = 0; j < resolution.y(); ++j )
        {
            const float y =
                grid_size.y() * static_cast<float>( j );
            for ( size_t i = 0; i < resolution.x(); ++i )
            {
                const float x =
                    grid_size.x() * static_cast<float>( i );

                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
            }
        }
    }

    SuperClass::setCoords( coords );
}

/*===========================================================================*/
/**
 *  @brief  Calculates coordinate values of the rectiliear volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_rectilinear_coords( const kvs::StructuredVolumeObject* volume )
{
    kvs::IgnoreUnusedVariable( volume );

    BaseClass::setSuccess( false );
    kvsMessageError("Rectilinear volume has not yet supportted.");
}

/*===========================================================================*/
/**
 *  @brief  Calculates connection values.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_connections( const kvs::StructuredVolumeObject* volume )
{
    const size_t line_size = volume->numberOfNodesPerLine();
    const size_t slice_size = volume->numberOfNodesPerSlice();
    const Vector3ui resolution( volume->resolution() );
    const size_t nedges =
        3 * ( resolution.x() - 1 ) * ( resolution.y() - 1 ) * ( resolution.z() - 1 ) +
        2 * ( resolution.x() - 1 ) * ( resolution.y() - 1 ) +
        2 * ( resolution.y() - 1 ) * ( resolution.z() - 1 ) +
        2 * ( resolution.z() - 1 ) * ( resolution.x() - 1 ) +
        resolution.x() - 1 + resolution.y() - 1 + resolution.z() - 1;

    kvs::ValueArray<kvs::UInt32> connections( 2 * nedges );
    kvs::UInt32* connection = connections.data();

    kvs::UInt32 volume_vertex = 0;
    kvs::UInt32 connection_index = 0;
    for ( size_t z = 0; z < resolution.z(); ++z )
    {
        for ( size_t y = 0; y < resolution.y(); ++y )
        {
            for ( size_t x = 0; x < resolution.x(); ++x )
            {
                if ( x != resolution.x() - 1 )
                {
                    connection[ connection_index++ ] = volume_vertex;
                    connection[ connection_index++ ] = volume_vertex + 1;
                }

                if ( y != resolution.y() - 1 )
                {
                    connection[ connection_index++ ] = volume_vertex;
                    connection[ connection_index++ ] = volume_vertex + line_size;
                }

                if ( z != resolution.z() - 1 )
                {
                    connection[ connection_index++ ] = volume_vertex;
                    connection[ connection_index++ ] = volume_vertex + slice_size;
                }

                ++volume_vertex;
            }
        }
    }

    SuperClass::setConnections( connections );
}

/*===========================================================================*/
/**
 *  @brief  Mapping for a unstructured volume object.
 *  @param  volume [in] pointer to the unstrctured volume object
 */
/*===========================================================================*/
void ExtractEdges::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    this->calculate_coords( volume );
    this->calculate_connections( volume );

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

    SuperClass::setLineType( kvs::LineObject::Segment );
    SuperClass::setColorType( kvs::LineObject::VertexColor );
    if ( SuperClass::numberOfSizes() == 0 ) SuperClass::setSize( 1.0f );
}

/*===========================================================================*/
/**
 *  @brief  Calculates coordinate values.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_coords( const kvs::UnstructuredVolumeObject* volume )
{
    SuperClass::setCoords( volume->coords() );
}

/*===========================================================================*/
/**
 *  @brief  Calculates connection values of the unsturctured volume object.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_connections( const kvs::UnstructuredVolumeObject* volume )
{
    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
        this->calculate_tetrahedra_connections( volume );
        break;
    case kvs::UnstructuredVolumeObject::Hexahedra:
        this->calculate_hexahedra_connections( volume );
        break;
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
        this->calculate_quadratic_tetrahedra_connections( volume );
        break;
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
        this->calculate_quadratic_hexahedra_connections( volume );
        break;
    case kvs::UnstructuredVolumeObject::Prism:
        this->calculate_prism_connections( volume );
        break;
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unknown cell type.");
        break;
    }
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates connection values for the tetrahedra cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_tetrahedra_connections(
    const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();

    ::EdgeMap edge_map( nnodes );
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        const kvs::UInt32 local_vertex0 = connections[ connection_index     ];
        const kvs::UInt32 local_vertex1 = connections[ connection_index + 1 ];
        const kvs::UInt32 local_vertex2 = connections[ connection_index + 2 ];
        const kvs::UInt32 local_vertex3 = connections[ connection_index + 3 ];
        connection_index += 4;

        edge_map.insert( local_vertex0, local_vertex1 );
        edge_map.insert( local_vertex0, local_vertex2 );
        edge_map.insert( local_vertex0, local_vertex3 );
        edge_map.insert( local_vertex1, local_vertex2 );
        edge_map.insert( local_vertex2, local_vertex3 );
        edge_map.insert( local_vertex3, local_vertex1 );
    }

    SuperClass::setConnections( edge_map.serialize() );
}

/*===========================================================================*/
/**
 *  @brief  Calculates connection values for the hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_hexahedra_connections(
    const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();

    ::EdgeMap edge_map( nnodes );
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        const kvs::UInt32 local_vertex0 = connections[ connection_index     ];
        const kvs::UInt32 local_vertex1 = connections[ connection_index + 1 ];
        const kvs::UInt32 local_vertex2 = connections[ connection_index + 2 ];
        const kvs::UInt32 local_vertex3 = connections[ connection_index + 3 ];
        const kvs::UInt32 local_vertex4 = connections[ connection_index + 4 ];
        const kvs::UInt32 local_vertex5 = connections[ connection_index + 5 ];
        const kvs::UInt32 local_vertex6 = connections[ connection_index + 6 ];
        const kvs::UInt32 local_vertex7 = connections[ connection_index + 7 ];
        connection_index += 8;

        edge_map.insert( local_vertex0, local_vertex1 );
        edge_map.insert( local_vertex1, local_vertex2 );
        edge_map.insert( local_vertex2, local_vertex3 );
        edge_map.insert( local_vertex3, local_vertex0 );
        edge_map.insert( local_vertex4, local_vertex5 );
        edge_map.insert( local_vertex5, local_vertex6 );
        edge_map.insert( local_vertex6, local_vertex7 );
        edge_map.insert( local_vertex7, local_vertex4 );
        edge_map.insert( local_vertex0, local_vertex4 );
        edge_map.insert( local_vertex1, local_vertex5 );
        edge_map.insert( local_vertex2, local_vertex6 );
        edge_map.insert( local_vertex3, local_vertex7 );
    }

    SuperClass::setConnections( edge_map.serialize() );
}

/*===========================================================================*/
/**
 *  @brief  Calculates connection values for the quadratic tetrahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_quadratic_tetrahedra_connections(
    const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();

    ::EdgeMap edge_map( nnodes );
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        const kvs::UInt32 local_vertex0 = connections[ connection_index     ];
        const kvs::UInt32 local_vertex1 = connections[ connection_index + 1 ];
        const kvs::UInt32 local_vertex2 = connections[ connection_index + 2 ];
        const kvs::UInt32 local_vertex3 = connections[ connection_index + 3 ];
        const kvs::UInt32 local_vertex4 = connections[ connection_index + 4 ];
        const kvs::UInt32 local_vertex5 = connections[ connection_index + 5 ];
        const kvs::UInt32 local_vertex6 = connections[ connection_index + 6 ];
        const kvs::UInt32 local_vertex7 = connections[ connection_index + 7 ];
        const kvs::UInt32 local_vertex8 = connections[ connection_index + 8 ];
        const kvs::UInt32 local_vertex9 = connections[ connection_index + 9 ];
        connection_index += 10;

        edge_map.insert( local_vertex0, local_vertex4 );
        edge_map.insert( local_vertex4, local_vertex1 );
        edge_map.insert( local_vertex0, local_vertex5 );
        edge_map.insert( local_vertex5, local_vertex2 );
        edge_map.insert( local_vertex0, local_vertex6 );
        edge_map.insert( local_vertex6, local_vertex3 );
        edge_map.insert( local_vertex1, local_vertex7 );
        edge_map.insert( local_vertex7, local_vertex2 );
        edge_map.insert( local_vertex2, local_vertex8 );
        edge_map.insert( local_vertex8, local_vertex3 );
        edge_map.insert( local_vertex3, local_vertex9 );
        edge_map.insert( local_vertex9, local_vertex1 );
    }

    SuperClass::setConnections( edge_map.serialize() );
}

/*===========================================================================*/
/**
 *  @brief  Calculates connection values for the quadratic hexahedral cells.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
void ExtractEdges::calculate_quadratic_hexahedra_connections(
    const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();

    ::EdgeMap edge_map( nnodes );
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        const kvs::UInt32 local_vertex0  = connections[ connection_index      ];
        const kvs::UInt32 local_vertex1  = connections[ connection_index +  1 ];
        const kvs::UInt32 local_vertex2  = connections[ connection_index +  2 ];
        const kvs::UInt32 local_vertex3  = connections[ connection_index +  3 ];
        const kvs::UInt32 local_vertex4  = connections[ connection_index +  4 ];
        const kvs::UInt32 local_vertex5  = connections[ connection_index +  5 ];
        const kvs::UInt32 local_vertex6  = connections[ connection_index +  6 ];
        const kvs::UInt32 local_vertex7  = connections[ connection_index +  7 ];
        const kvs::UInt32 local_vertex8  = connections[ connection_index +  8 ];
        const kvs::UInt32 local_vertex9  = connections[ connection_index +  9 ];
        const kvs::UInt32 local_vertex10 = connections[ connection_index + 10 ];
        const kvs::UInt32 local_vertex11 = connections[ connection_index + 11 ];
        const kvs::UInt32 local_vertex12 = connections[ connection_index + 12 ];
        const kvs::UInt32 local_vertex13 = connections[ connection_index + 13 ];
        const kvs::UInt32 local_vertex14 = connections[ connection_index + 14 ];
        const kvs::UInt32 local_vertex15 = connections[ connection_index + 15 ];
        const kvs::UInt32 local_vertex16 = connections[ connection_index + 16 ];
        const kvs::UInt32 local_vertex17 = connections[ connection_index + 17 ];
        const kvs::UInt32 local_vertex18 = connections[ connection_index + 18 ];
        const kvs::UInt32 local_vertex19 = connections[ connection_index + 19 ];
        connection_index += 20;

        edge_map.insert( local_vertex0,  local_vertex8  );
        edge_map.insert( local_vertex8,  local_vertex1  );
        edge_map.insert( local_vertex1,  local_vertex9  );
        edge_map.insert( local_vertex9,  local_vertex2  );
        edge_map.insert( local_vertex2,  local_vertex10 );
        edge_map.insert( local_vertex10, local_vertex3  );
        edge_map.insert( local_vertex3,  local_vertex11 );
        edge_map.insert( local_vertex11, local_vertex0  );
        edge_map.insert( local_vertex4,  local_vertex12 );
        edge_map.insert( local_vertex12, local_vertex5  );
        edge_map.insert( local_vertex5,  local_vertex13 );
        edge_map.insert( local_vertex13, local_vertex6  );
        edge_map.insert( local_vertex6,  local_vertex14 );
        edge_map.insert( local_vertex14, local_vertex7  );
        edge_map.insert( local_vertex7,  local_vertex15 );
        edge_map.insert( local_vertex15, local_vertex4  );
        edge_map.insert( local_vertex0,  local_vertex16 );
        edge_map.insert( local_vertex16, local_vertex4  );
        edge_map.insert( local_vertex1,  local_vertex17 );
        edge_map.insert( local_vertex17, local_vertex5  );
        edge_map.insert( local_vertex2,  local_vertex18 );
        edge_map.insert( local_vertex18, local_vertex6  );
        edge_map.insert( local_vertex3,  local_vertex19 );
        edge_map.insert( local_vertex19, local_vertex7  );
    }

    SuperClass::setConnections( edge_map.serialize() );
}

void ExtractEdges::calculate_prism_connections( const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();

    ::EdgeMap edge_map( nnodes );
    for ( size_t cell_index = 0, connection_index = 0; cell_index < ncells; cell_index++ )
    {
        const kvs::UInt32 local_vertex0 = connections[ connection_index     ];
        const kvs::UInt32 local_vertex1 = connections[ connection_index + 1 ];
        const kvs::UInt32 local_vertex2 = connections[ connection_index + 2 ];
        const kvs::UInt32 local_vertex3 = connections[ connection_index + 3 ];
        const kvs::UInt32 local_vertex4 = connections[ connection_index + 4 ];
        const kvs::UInt32 local_vertex5 = connections[ connection_index + 5 ];
        connection_index += 6;

        edge_map.insert( local_vertex0, local_vertex1 );
        edge_map.insert( local_vertex1, local_vertex2 );
        edge_map.insert( local_vertex2, local_vertex0 );
        edge_map.insert( local_vertex3, local_vertex4 );
        edge_map.insert( local_vertex4, local_vertex5 );
        edge_map.insert( local_vertex5, local_vertex3 );
        edge_map.insert( local_vertex0, local_vertex3 );
        edge_map.insert( local_vertex1, local_vertex4 );
        edge_map.insert( local_vertex2, local_vertex5 );
    }

    SuperClass::setConnections( edge_map.serialize() );
}

/*===========================================================================*/
/**
 *  @brief  Calculates color values.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
void ExtractEdges::calculate_colors( const kvs::VolumeObjectBase* volume )
{
    const T* value = reinterpret_cast<const T*>( volume->values().data() );
    const T* const end = value + volume->values().size();

    kvs::ValueArray<kvs::UInt8> colors( 3 * volume->numberOfNodes() );
    kvs::UInt8* color = colors.data();

    kvs::ColorMap cmap( BaseClass::colorMap() );

    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real64 min_value = volume->minValue();
    const kvs::Real64 max_value = volume->maxValue();

    const kvs::Real64 normalize_factor =
        static_cast<kvs::Real64>( cmap.resolution() - 1 ) / ( max_value - min_value );

    const size_t veclen = BaseClass::volume()->veclen();
    if ( veclen == 1 )
    {
        while ( value < end )
        {
            const kvs::UInt32 color_level =
                static_cast<kvs::UInt32>( normalize_factor * ( static_cast<kvs::Real64>( *( value++ ) ) - min_value ) );

            *( color++ ) = cmap[ color_level ].r();
            *( color++ ) = cmap[ color_level ].g();
            *( color++ ) = cmap[ color_level ].b();
        }
    }
    else
    {
        while( value < end )
        {
            kvs::Real64 magnitude = 0.0;
            for ( size_t i = 0; i < veclen; ++i )
            {
                magnitude += kvs::Math::Square( static_cast<kvs::Real64>( *value ) );
                ++value;
            }
            magnitude = std::sqrt( magnitude );

            const kvs::UInt32 color_level =
                static_cast<kvs::UInt32>( normalize_factor * ( magnitude - min_value ) );

            *( color++ ) = cmap[ color_level ].r();
            *( color++ ) = cmap[ color_level ].g();
            *( color++ ) = cmap[ color_level ].b();
        }
    }

    SuperClass::setColors( colors );
}

} // end of namespace kvs
