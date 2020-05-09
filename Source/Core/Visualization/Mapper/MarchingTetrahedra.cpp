/****************************************************************************/
/**
 *  @file   MarchingTetrahedra.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MarchingTetrahedra.cpp 1761 2014-05-07 04:18:15Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MarchingTetrahedra.h"
#include "MarchingTetrahedraTable.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new MarchingTetrahedra class.
 */
/*==========================================================================*/
MarchingTetrahedra::MarchingTetrahedra():
    kvs::MapperBase(),
    kvs::PolygonObject(),
    m_isolevel( 0 ),
    m_duplication( true )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs and creates a polygon object.
 *  @param  volume [in] pointer to the volume object
 *  @param  isolevel [in] level of the isosurfaces
 *  @param  normal_type [in] type of the normal vector
 *  @param  duplication [in] duplication flag
 *  @param  transfer_function [in] transfer function
 */
/*==========================================================================*/
MarchingTetrahedra::MarchingTetrahedra(
    const kvs::UnstructuredVolumeObject* volume,
    const double isolevel,
    const NormalType normal_type,
    const bool duplication,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PolygonObject(),
    m_isolevel( isolevel ),
    m_duplication( duplication )
{
    SuperClass::setNormalType( normal_type );

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( normal_type == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
    }

    // Extract the surfaces.
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Destroys the MarchingTetrahedra class.
 */
/*==========================================================================*/
MarchingTetrahedra::~MarchingTetrahedra()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the input volume object
 *  @return pointer to the polygon object
 */
/*===========================================================================*/
MarchingTetrahedra::SuperClass* MarchingTetrahedra::exec( const kvs::ObjectBase* object )
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
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    this->mapping( volume );

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces.
 *  @param  volume [in] pointer to the volume object
 */
/*==========================================================================*/
void MarchingTetrahedra::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 1 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not sclar field data.");
        return;
    }

    if ( volume->cellType() != kvs::UnstructuredVolumeObject::Tetrahedra )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not tetrahedra cell data.");
        return;
    }

    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    const kvs::Real64 min_value = BaseClass::volume()->minValue();
    const kvs::Real64 max_value = BaseClass::volume()->maxValue();
    if ( kvs::Math::Equal( min_value, max_value ) ) { return; }

    // Extract surfaces.
    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) this->extract_surfaces<kvs::Int8>( volume );
    else if ( type == typeid( kvs::Int16  ) ) this->extract_surfaces<kvs::Int16>( volume );
    else if ( type == typeid( kvs::Int32  ) ) this->extract_surfaces<kvs::Int32>( volume );
    else if ( type == typeid( kvs::Int64  ) ) this->extract_surfaces<kvs::Int64>( volume );
    else if ( type == typeid( kvs::UInt8  ) ) this->extract_surfaces<kvs::UInt8>( volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->extract_surfaces<kvs::UInt16>( volume );
    else if ( type == typeid( kvs::UInt32 ) ) this->extract_surfaces<kvs::UInt32>( volume );
    else if ( type == typeid( kvs::UInt64 ) ) this->extract_surfaces<kvs::UInt64>( volume );
    else if ( type == typeid( kvs::Real32 ) ) this->extract_surfaces<kvs::Real32>( volume );
    else if ( type == typeid( kvs::Real64 ) ) this->extract_surfaces<kvs::Real64>( volume );
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
    }
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingTetrahedra::extract_surfaces( const kvs::UnstructuredVolumeObject* volume )
{
    if ( m_duplication ) this->extract_surfaces_with_duplication<T>( volume );
    else                 this->extract_surfaces_without_duplication<T>( volume );
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces with duplication.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingTetrahedra::extract_surfaces_with_duplication(
    const kvs::UnstructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;

    // Refer the unstructured volume object.
    const kvs::UInt32* connections =
        static_cast<const kvs::UInt32*>( volume->connections().data() );

    const size_t ncells = volume->numberOfCells();

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[4];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 4 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = connections[ index ];
        local_index[1] = connections[ index + 1 ];
        local_index[2] = connections[ index + 2 ];
        local_index[3] = connections[ index + 3 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_table_index<T>( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 15 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingTetrahedraTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable by using the table_index.
            const int e0 = MarchingTetrahedraTable::TriangleID[table_index][i];
            const int e1 = MarchingTetrahedraTable::TriangleID[table_index][i+1];
            const int e2 = MarchingTetrahedraTable::TriangleID[table_index][i+2];

            // Determine vertices for each edge.
            const int v0 = local_index[ MarchingTetrahedraTable::VertexID[e0][0] ];
            const int v1 = local_index[ MarchingTetrahedraTable::VertexID[e0][1] ];

            const int v2 = local_index[ MarchingTetrahedraTable::VertexID[e1][0] ];
            const int v3 = local_index[ MarchingTetrahedraTable::VertexID[e1][1] ];

            const int v4 = local_index[ MarchingTetrahedraTable::VertexID[e2][0] ];
            const int v5 = local_index[ MarchingTetrahedraTable::VertexID[e2][1] ];

            // Calculate coordinates of the vertices which are composed
            // of the triangle polygon.
            const kvs::Vec3 vertex0( this->interpolate_vertex<T>( v0, v1 ) );
            coords.push_back( vertex0.x() );
            coords.push_back( vertex0.y() );
            coords.push_back( vertex0.z() );

            const kvs::Vec3 vertex1( this->interpolate_vertex<T>( v2, v3 ) );
            coords.push_back( vertex1.x() );
            coords.push_back( vertex1.y() );
            coords.push_back( vertex1.z() );

            const kvs::Vec3 vertex2( this->interpolate_vertex<T>( v4, v5 ) );
            coords.push_back( vertex2.x() );
            coords.push_back( vertex2.y() );
            coords.push_back( vertex2.z() );

            // Calculate a normal vector for the triangle polygon.
            const kvs::Vec3 normal( ( vertex1 - vertex0 ).cross( vertex2 - vertex0 ) );
            normals.push_back( normal.x() );
            normals.push_back( normal.y() );
            normals.push_back( normal.z() );
        } // end of loop-triangle
    } // end of loop-cell

    // Calculate the polygon color for the isolevel.
    const kvs::RGBColor color = this->calculate_color<T>();

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColor( color );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::PolygonColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces without duplication.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingTetrahedra::extract_surfaces_without_duplication(
    const kvs::UnstructuredVolumeObject* volume )
{
    kvs::IgnoreUnusedVariable( volume );

#if NOT_YET_IMPLEMENTED
    const size_t nedges     = volume->adjacency()->nedges();
    const size_t byte_size  = sizeof( size_t ) * nedges;
    kvs::UInt32* vertex_map = static_cast<kvs::UInt32*>( malloc( byte_size ) );
    if ( !vertex_map )
    {
        kvsMessageError("Cannot allocate memory for the vertex map.");
        return;
    }
    memset( vertex_map, 0, byte_size );

    std::vector<kvs::Real32> coords;
    this->calculate_isopoints<T>( vertex_map, coords );

    std::vector<kvs::UInt32> connections;
    this->connect_isopoints<T>( vertex_map, connections );

    free( vertex_map );

    std::vector<kvs::Real32> normals;
    if ( SuperClass::normalType() == kvs::PolygonObject::PolygonNormal )
    {
        this->calculate_normals_on_polygon( coords, connections, normals );
    }
    else
    {
        this->calculate_normals_on_vertex( coords, connections, normals );
    }

    // Calculate the polygon color for the isolevel.
    const kvs::RGBColor color = this->calculate_color<T>();

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setColor( color );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::PolygonColor );
#endif // NOT_YET_IMPLEMENTED
}

/*==========================================================================*/
/**
 *  @brief  Calculate a index of the marching tetrahedra table.
 *  @param  local_index [in] indices of the cell
 *  @return table index
 */
/*==========================================================================*/
template <typename T>
size_t MarchingTetrahedra::calculate_table_index( const size_t* local_index ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const double isolevel = m_isolevel;

    size_t table_index = 0;
    if ( static_cast<double>( values[ local_index[0] ] ) > isolevel ) { table_index |=   1; }
    if ( static_cast<double>( values[ local_index[1] ] ) > isolevel ) { table_index |=   2; }
    if ( static_cast<double>( values[ local_index[2] ] ) > isolevel ) { table_index |=   4; }
    if ( static_cast<double>( values[ local_index[3] ] ) > isolevel ) { table_index |=   8; }

    return table_index;
}

/*==========================================================================*/
/**
 *  @brief  Interpolates a coordinate value of the intersected vertex.
 *  @param  vertex0 [in] index of the vertex of the end point #0
 *  @param  vertex1 [in] index of the vertex of the end point #1
 *  @return interpolated vertex coordinate
 */
/*==========================================================================*/
template <typename T>
const kvs::Vec3 MarchingTetrahedra::interpolate_vertex(
    const int vertex0,
    const int vertex1 ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const kvs::Real32* const coords = BaseClass::volume()->coords().data();

    const size_t coord0_index = 3 * vertex0;
    const size_t coord1_index = 3 * vertex1;

    const double v0 = static_cast<double>( values[ vertex0 ] );
    const double v1 = static_cast<double>( values[ vertex1 ] );
    const float ratio = static_cast<float>( kvs::Math::Abs( ( m_isolevel - v0 ) / ( v1 - v0 ) ) );

    const float x = coords[coord0_index]   + ratio * ( coords[coord1_index]   - coords[coord0_index] );
    const float y = coords[coord0_index+1] + ratio * ( coords[coord1_index+1] - coords[coord0_index+1] );
    const float z = coords[coord0_index+2] + ratio * ( coords[coord1_index+2] - coords[coord0_index+2] );

    return kvs::Vec3( x, y, z );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a color of the surfaces from the isolevel.
 *  @return surface color
 */
/*==========================================================================*/
template <typename T>
const kvs::RGBColor MarchingTetrahedra::calculate_color()
{
    // Calculate the min/max values of the node data.
    if ( !BaseClass::volume()->hasMinMaxValues() )
    {
        BaseClass::volume()->updateMinMaxValues();
    }

    const kvs::Real64 min_value = BaseClass::volume()->minValue();
    const kvs::Real64 max_value = BaseClass::volume()->maxValue();
    const kvs::Real64 normalize_factor = 255.0 / ( max_value - min_value );
    const kvs::UInt8  index = static_cast<kvs::UInt8>( normalize_factor * ( m_isolevel - min_value ) );

    return BaseClass::transferFunction().colorMap()[ index ];
}

#if NOT_YET_IMPLEMENTED
/*==========================================================================*/
/**
 *  @brief  Calculates the coordinates on the surfaces.
 *  @param  vertex_map [in/out] pointer to the vertex map
 *  @param  coords [in/out] coordinate array
 */
/*==========================================================================*/
template <typename T>
void MarchingTetrahedra::calculate_isopoints(
    kvs::UInt32*&             vertex_map,
    std::vector<kvs::Real32>& coords )
{
    const kvs::UnstructuredVolumeObject* volume =
        reinterpret_cast<const kvs::UnstructuredVolumeObject*>( BaseClass::volume() );

    const T* const values = static_cast<const T*>( volume->values().data() );
    const kvs::EdgeConnections* const edge_connections = volume->adjacency()->edgeConnections();
    const size_t nedges = volume->adjacency()->nedges();
    const double isolevel = m_isolevel;

    for ( size_t edge = 0; edge < nedges ; ++edge )
    {
        const size_t vertex0 = edge_connections->vertex( edge, 0 );
        const size_t vertex1 = edge_connections->vertex( edge, 1 );

        if ( ( values[ vertex0 ] > isolevel ) == ( values[ vertex1 ] > isolevel ) ) { continue; }

        const kvs::Vec3 isopoint( this->interpolate_vertex( vertex0, vertex1 ) );

        coords.push_back( isopoint.x() );
        coords.push_back( isopoint.y() );
        coords.push_back( isopoint.z() );

        vertex_map[ edge ] = nisopoints++;
    }
}
#endif // NOT_YET_IMPLEMENTED

#if NOT_YET_IMPLEMENTED
/*==========================================================================*/
/**
 *  @brief  Connects the coordinates.
 *  @param  vertex_map [in/out] pointer to the vertex map
 *  @param  connections [in/out] connection array
 */
/*==========================================================================*/
template <typename T>
void MarchingTetrahedra::connect_isopoints(
    kvs::UInt32*&             vertex_map,
    std::vector<kvs::UInt32>& connections )
{
    const kvs::UnstructuredVolumeObject* volume =
        reinterpret_cast<const kvs::UnstructuredVolumeObject*>( BaseClass::volume() );

    const kvs::VertexGraph* const vertex_graph = volume->adjacency()->vertexGraph();
    const kvs::UInt32* const volume_connections = volum->connections()->data();
    const size_t ncells = volume->numberOfCells();

    size_t index = 0;
    size_t local_index[4];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 4 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = volume_connections[ index     ];
        local_index[1] = volume_connections[ index + 1 ];
        local_index[2] = volume_connections[ index + 2 ];
        local_index[3] = volume_connections[ index + 3 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_table_index<T>( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 15 ) continue;

        for ( size_t i = 0; MarchingTetrahedraTable::TriangleID[table_index][i] != -1; i += 3 )
        {
            const int local_e0 = MarchingTetrahedraTable::TriangleID[table_index][i];
            const int local_e1 = MarchingTetrahedraTable::TriangleID[table_index][i+1];
            const int local_e2 = MarchingTetrahedraTable::TriangleID[table_index][i+2];

            const size_t e0 = vertex_graph->edge(
                local_index[ MarchingTetrahedraTable::VertexID[0][0] ],
                local_index[ MarchingTetrahedraTable::VertexID[0][1] ]);
            const size_t e1 = vertex_graph->edge(
                local_index[ MarchingTetrahedraTable::VertexID[1][0] ],
                local_index[ MarchingTetrahedraTable::VertexID[1][1] ]);
            const size_t e2 = vertex_graph->edge(
                local_index[ MarchingTetrahedraTable::VertexID[2][0] ],
                local_index[ MarchingTetrahedraTable::VertexID[2][1] ]);

            connections.push_back( vertex_map[e0] );
            connections.push_back( vertex_map[e1] );
            connections.push_back( vertex_map[e2] );
        }
    }
}
#endif // NOT_YET_IMPLEMENTED

#if NOT_YET_IMPLEMENTED
/*==========================================================================*/
/**
 *  @brief  Calculates a normal vector array on the polygon.
 *  @param  coords [in] coordinate array
 *  @param  connections [in] connection array
 *  @param  normals [out] normal vector array
 */
/*==========================================================================*/
void MarchingTetrahedra::calculate_normals_on_polygon(
    const std::vector<kvs::Real32>& coords,
    const std::vector<kvs::UInt32>& connections,
    std::vector<kvs::Real32>&       normals )
{
    if ( coords.empty() ) return;

    normals.resize( connections.size() );
    std::fill( normals.begin(), normals.end(), 0.0f );

    const kvs::Real32* const coords_ptr = &coords[ 0 ];

    const size_t size = connections.size();
    for ( kvs::UInt32 index = 0; index < size; index += 3 )
    {
        const kvs::UInt32 coord0_index = 3 * connections[ index     ];
        const kvs::UInt32 coord1_index = 3 * connections[ index + 1 ];
        const kvs::UInt32 coord2_index = 3 * connections[ index + 2 ];

        const kvs::Vec3 v0( coords_ptr + coord0_index );
        const kvs::Vec3 v1( coords_ptr + coord1_index );
        const kvs::Vec3 v2( coords_ptr + coord2_index );

        const kvs::Vec3 normal( ( v1 - v0 ).cross( v2 - v0 ) );

        normals[ index     ] = normal.x();
        normals[ index + 1 ] = normal.y();
        normals[ index + 2 ] = normal.z();
    }
}
#endif // NOT_YET_IMPLEMENTED

#if NOT_YET_IMPLEMENTED
/*==========================================================================*/
/**
 *  @brief  Calculates a normal vector array on the vertex.
 *  @param  coords [in] coordinate array
 *  @param  connections [in] connection array
 *  @param  normals [out] normal vector array
 */
/*==========================================================================*/
void MarchingTetrahedra::calculate_normals_on_vertex(
    const std::vector<kvs::Real32>& coords,
    const std::vector<kvs::UInt32>& connections,
    std::vector<kvs::Real32>&       normals )
{
    if ( coords.empty() ) return;

    normals.resize( coords.size() );
    std::fill( normals.begin(), normals.end(), 0.0f );

    const kvs::Real32* const coords_ptr = &coords[ 0 ];

    const size_t size = connections.size();
    for ( kvs::UInt32 index = 0; index < size; index += 3 )
    {
        const kvs::UInt32 coord0_index = 3 * connections[ index     ];
        const kvs::UInt32 coord1_index = 3 * connections[ index + 1 ];
        const kvs::UInt32 coord2_index = 3 * connections[ index + 2 ];

        const kvs::Vec3 v0( coords_ptr + coord0_index );
        const kvs::Vec3 v1( coords_ptr + coord1_index );
        const kvs::Vec3 v2( coords_ptr + coord2_index );

        const kvs::Vec3 normal( ( v1 - v0 ).cross( v2 - v0 ) );

        normals[ coord0_index     ] += normal.x();
        normals[ coord0_index + 1 ] += normal.y();
        normals[ coord0_index + 2 ] += normal.z();

        normals[ coord1_index     ] += normal.x();
        normals[ coord1_index + 1 ] += normal.y();
        normals[ coord1_index + 2 ] += normal.z();

        normals[ coord2_index     ] += normal.x();
        normals[ coord2_index + 1 ] += normal.y();
        normals[ coord2_index + 2 ] += normal.z();
    }
}
#endif // NOT_YET_IMPLEMENTED

} // end of namesapce kvs
