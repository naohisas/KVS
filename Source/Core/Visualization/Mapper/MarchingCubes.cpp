/****************************************************************************/
/**
 *  @file   MarchingCubes.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MarchingCubes.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MarchingCubes.h"
#include "MarchingCubesTable.h"
#include <cstring>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new MarchingCubes class.
 */
/*==========================================================================*/
MarchingCubes::MarchingCubes():
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
MarchingCubes::MarchingCubes(
    const kvs::StructuredVolumeObject* volume,
    const double isolevel,
    const NormalType normal_type,
    const bool duplication,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PolygonObject(),
    m_duplication( duplication )
{
    SuperClass::setNormalType( normal_type );
    this->setIsolevel( isolevel );
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Destroys the MarchingCubes class.
 */
/*==========================================================================*/
MarchingCubes::~MarchingCubes()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the input volume object
 *  @return pointer to the polygon object
 */
/*===========================================================================*/
MarchingCubes::SuperClass* MarchingCubes::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( SuperClass::normalType() == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
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
void MarchingCubes::mapping( const kvs::StructuredVolumeObject* volume )
{
    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 1 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("The input volume is not a sclar field data.");
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
void MarchingCubes::extract_surfaces( const kvs::StructuredVolumeObject* volume )
{
    if ( m_duplication ) this->extract_surfaces_with_duplication<T>( volume );
    else                 this->extract_surfaces_without_duplication<T>( volume );
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces with duplication.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingCubes::extract_surfaces_with_duplication(
    const kvs::StructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;

    const kvs::Vec3u ncells( volume->resolution() - kvs::Vec3u::Constant(1) );
    const kvs::UInt32 line_size( volume->numberOfNodesPerLine() );
    const kvs::UInt32 slice_size( volume->numberOfNodesPerSlice() );

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[8];
    for ( kvs::UInt32 z = 0; z < ncells.z(); ++z )
    {
        for ( kvs::UInt32 y = 0; y < ncells.y(); ++y )
        {
            for ( kvs::UInt32 x = 0; x < ncells.x(); ++x )
            {
                // Calculate the indices of the target cell.
                local_index[0] = index;
                local_index[1] = local_index[0] + 1;
                local_index[2] = local_index[1] + line_size;
                local_index[3] = local_index[0] + line_size;
                local_index[4] = local_index[0] + slice_size;
                local_index[5] = local_index[1] + slice_size;
                local_index[6] = local_index[2] + slice_size;
                local_index[7] = local_index[3] + slice_size;
                index++;

                // Calculate the index of the reference table.
                const size_t table_index = this->calculate_table_index<T>( local_index );
                if ( table_index == 0 ) continue;
                if ( table_index == 255 ) continue;

                // Calculate the triangle polygons.
                for ( size_t i = 0; MarchingCubesTable::TriangleID[ table_index ][i] != -1; i += 3 )
                {
                    // Refer the edge IDs from the TriangleTable by using the table_index.
                    const int e0 = MarchingCubesTable::TriangleID[table_index][i];
                    const int e1 = MarchingCubesTable::TriangleID[table_index][i+2];
                    const int e2 = MarchingCubesTable::TriangleID[table_index][i+1];

                    // Determine vertices for each edge.
                    const kvs::Vec3 v0(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e0][0][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e0][0][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e0][0][2] ) );

                    const kvs::Vec3 v1(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e0][1][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e0][1][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e0][1][2] ) );

                    const kvs::Vec3 v2(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e1][0][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e1][0][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e1][0][2] ) );

                    const kvs::Vec3 v3(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e1][1][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e1][1][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e1][1][2] ) );

                    const kvs::Vec3 v4(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e2][0][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e2][0][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e2][0][2] ) );

                    const kvs::Vec3 v5(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e2][1][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e2][1][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e2][1][2] ) );

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
            } // end of loop-x
            ++index;
        } // end of loop-y
        index += line_size;
    } // end of loop-z

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
void MarchingCubes::extract_surfaces_without_duplication(
    const kvs::StructuredVolumeObject* volume )
{
    const size_t volume_size = volume->numberOfNodes();
    const size_t byte_size   = sizeof( kvs::UInt32 ) * 3 * volume_size;
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
}

/*==========================================================================*/
/**
 *  @brief  Calculate a index of the marching cubes table.
 *  @param  local_index [in] indices of the cell
 *  @return table index
 */
/*==========================================================================*/
template <typename T>
size_t MarchingCubes::calculate_table_index( const size_t* local_index ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const double isolevel = m_isolevel;

    size_t table_index = 0;
    if ( static_cast<double>( values[ local_index[0] ] ) > isolevel ) { table_index |=   1; }
    if ( static_cast<double>( values[ local_index[1] ] ) > isolevel ) { table_index |=   2; }
    if ( static_cast<double>( values[ local_index[2] ] ) > isolevel ) { table_index |=   4; }
    if ( static_cast<double>( values[ local_index[3] ] ) > isolevel ) { table_index |=   8; }
    if ( static_cast<double>( values[ local_index[4] ] ) > isolevel ) { table_index |=  16; }
    if ( static_cast<double>( values[ local_index[5] ] ) > isolevel ) { table_index |=  32; }
    if ( static_cast<double>( values[ local_index[6] ] ) > isolevel ) { table_index |=  64; }
    if ( static_cast<double>( values[ local_index[7] ] ) > isolevel ) { table_index |= 128; }

    return table_index;
}

/*==========================================================================*/
/**
 *  @brief  Interpolates a coordinate value of the intersected vertex.
 *  @param  vertex0 [in] vertex coordinate of the end point #0
 *  @param  vertex1 [in] vertex coordinate of the end point #1
 *  @return interpolated vertex coordinate
 */
/*==========================================================================*/
template <typename T>
const kvs::Vec3 MarchingCubes::interpolate_vertex(
    const kvs::Vec3& vertex0,
    const kvs::Vec3& vertex1 ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const kvs::StructuredVolumeObject* volume =
        reinterpret_cast<const kvs::StructuredVolumeObject*>( BaseClass::volume() );

    const double x0 = vertex0.x();
    const double y0 = vertex0.y();
    const double z0 = vertex0.z();
    const double x1 = vertex1.x();
    const double y1 = vertex1.y();
    const double z1 = vertex1.z();
    const kvs::UInt32 line_size  = volume->numberOfNodesPerLine();
    const kvs::UInt32 slice_size = volume->numberOfNodesPerSlice();
    const size_t v0_index = static_cast<size_t>( x0 + y0 * line_size + z0 * slice_size );
    const size_t v1_index = static_cast<size_t>( x1 + y1 * line_size + z1 * slice_size );

    const double v0 = static_cast<double>( values[ v0_index ] );
    const double v1 = static_cast<double>( values[ v1_index ] );
    const float ratio = static_cast<float>( kvs::Math::Abs( ( m_isolevel - v0 ) / ( v1 - v0 ) ) );

    const float x = ( 1.0f - ratio ) * vertex0.x() + ratio * vertex1.x();
    const float y = ( 1.0f - ratio ) * vertex0.y() + ratio * vertex1.y();
    const float z = ( 1.0f - ratio ) * vertex0.z() + ratio * vertex1.z();

    return kvs::Vec3( x, y, z );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a color of the surfaces from the isolevel.
 *  @return surface color
 */
/*==========================================================================*/
template <typename T>
const kvs::RGBColor MarchingCubes::calculate_color()
{
    const kvs::Real64 min_value = BaseClass::volume()->minValue();
    const kvs::Real64 max_value = BaseClass::volume()->maxValue();
    const kvs::Real64 normalize_factor = 255.0 / ( max_value - min_value );
    const kvs::UInt8  index = static_cast<kvs::UInt8>( normalize_factor * ( m_isolevel - min_value ) );

    return BaseClass::transferFunction().colorMap()[ index ];
}

/*==========================================================================*/
/**
 *  @brief  Calculates the coordinates on the surfaces.
 *  @param  vertex_map [in/out] pointer to the vertex map
 *  @param  coords [in/out] coordinate array
 */
/*==========================================================================*/
template <typename T>
void MarchingCubes::calculate_isopoints(
    kvs::UInt32*&             vertex_map,
    std::vector<kvs::Real32>& coords )
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const kvs::StructuredVolumeObject* volume =
        reinterpret_cast<const kvs::StructuredVolumeObject*>( BaseClass::volume() );

    const kvs::Vec3u resolution( volume->resolution() );
    const kvs::Vec3u ncells( resolution - kvs::Vec3u::Constant(1) );
    const kvs::UInt32 line_size( volume->numberOfNodesPerLine() );
    const kvs::UInt32 slice_size( volume->numberOfNodesPerSlice() );
    const double isolevel = m_isolevel;

    kvs::UInt32 nisopoints = 0;
    size_t index = 0;
    for ( kvs::UInt32 z = 0; z < resolution.z(); ++z )
    {
        for ( kvs::UInt32 y = 0; y < resolution.y(); ++y )
        {
            for ( kvs::UInt32 x = 0; x < resolution.x(); ++x )
            {
                const size_t id0 = index;
                const size_t id1 = id0 + 1;
                const size_t id2 = id0 + line_size;
                const size_t id3 = id0 + slice_size;

                if ( x != ncells.x() )
                {
                    if ( ( static_cast<double>( values[id0] ) > isolevel ) !=
                         ( static_cast<double>( values[id1] ) > isolevel ) )
                    {
                        const kvs::Vec3 v1( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) );
                        const kvs::Vec3 v2( static_cast<float>(x+1), static_cast<float>(y), static_cast<float>(z) );
                        const kvs::Vec3 isopoint( this->interpolate_vertex<T>( v1, v2 ) );

                        coords.push_back( isopoint.x() );
                        coords.push_back( isopoint.y() );
                        coords.push_back( isopoint.z() );

                        vertex_map[ 3 * index ] = nisopoints++;
                    }
                }

                if ( y != ncells.y() )
                {
                    if ( ( static_cast<double>( values[id0] ) > isolevel ) !=
                         ( static_cast<double>( values[id2] ) > isolevel ) )
                    {
                        const kvs::Vec3 v1( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) );
                        const kvs::Vec3 v2( static_cast<float>(x), static_cast<float>(y+1), static_cast<float>(z) );
                        const kvs::Vec3 isopoint( this->interpolate_vertex<T>( v1, v2 ) );

                        coords.push_back( isopoint.x() );
                        coords.push_back( isopoint.y() );
                        coords.push_back( isopoint.z() );

                        vertex_map[ 3 * index + 1 ] = nisopoints++;
                    }
                }

                if ( z != ncells.z() )
                {
                    if ( ( static_cast<double>( values[id0] ) > isolevel ) !=
                         ( static_cast<double>( values[id3] ) > isolevel ) )
                    {
                        const kvs::Vec3 v1( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) );
                        const kvs::Vec3 v2( static_cast<float>(x), static_cast<float>(y), static_cast<float>(z+1) );
                        const kvs::Vec3 isopoint( this->interpolate_vertex<T>( v1, v2 ) );

                        coords.push_back( isopoint.x() );
                        coords.push_back( isopoint.y() );
                        coords.push_back( isopoint.z() );

                        vertex_map[ 3 * index + 2 ] = nisopoints++;
                    }
                }
                ++index;
            } // x
        } // y
    } // z
}

/*==========================================================================*/
/**
 *  @brief  Connects the coordinates.
 *  @param  vertex_map [in/out] pointer to the vertex map
 *  @param  connections [in/out] connection array
 */
/*==========================================================================*/
template <typename T>
void MarchingCubes::connect_isopoints(
    kvs::UInt32*&             vertex_map,
    std::vector<kvs::UInt32>& connections )
{
    const kvs::StructuredVolumeObject* volume =
        reinterpret_cast<const kvs::StructuredVolumeObject*>( BaseClass::volume() );

    const kvs::Vec3u resolution( volume->resolution() );
    const kvs::Vec3u ncells( resolution - kvs::Vec3u::Constant(1) );
    const kvs::UInt32 line_size( volume->numberOfNodesPerLine() );
    const kvs::UInt32 slice_size( volume->numberOfNodesPerSlice() );

    size_t index = 0;
    size_t local_index[8];
    size_t local_edge[12];
    for ( kvs::UInt32 z = 0; z < ncells.z(); ++z )
    {
        for ( kvs::UInt32 y = 0; y < ncells.y(); ++y )
        {
            for ( kvs::UInt32 x = 0; x < ncells.x(); ++x )
            {
                // Calculate the indices of the target cell.
                local_index[0] = index;
                local_index[1] = local_index[0] + 1;
                local_index[2] = local_index[1] + line_size;
                local_index[3] = local_index[0] + line_size;
                local_index[4] = local_index[0] + slice_size;
                local_index[5] = local_index[1] + slice_size;
                local_index[6] = local_index[2] + slice_size;
                local_index[7] = local_index[3] + slice_size;
                index++;

                // Calculate the index of the reference table.
                const size_t table_index = this->calculate_table_index<T>( local_index );
                if ( table_index == 0 ) continue;
                if ( table_index == 255 ) continue;

                local_edge[ 0] = 3 * local_index[0];
                local_edge[ 1] = local_edge[0] + 3 + 1;
                local_edge[ 2] = local_edge[0] + 3 * line_size;
                local_edge[ 3] = local_edge[0] + 1;
                local_edge[ 4] = local_edge[0] + 3 * slice_size;
                local_edge[ 5] = local_edge[1] + 3 * slice_size;
                local_edge[ 6] = local_edge[2] + 3 * slice_size;
                local_edge[ 7] = local_edge[3] + 3 * slice_size;
                local_edge[ 8] = local_edge[0] + 2;
                local_edge[ 9] = local_edge[8] + 3;
                local_edge[10] = local_edge[8] + 3 + 3 * line_size;
                local_edge[11] = local_edge[8] + 3 * line_size;

                for ( size_t i = 0; MarchingCubesTable::TriangleID[table_index][i] != -1; i += 3 )
                {
                    const int e0 = local_edge[ MarchingCubesTable::TriangleID[table_index][i]   ];
                    const int e1 = local_edge[ MarchingCubesTable::TriangleID[table_index][i+2] ];
                    const int e2 = local_edge[ MarchingCubesTable::TriangleID[table_index][i+1] ];

                    connections.push_back( vertex_map[e0] );
                    connections.push_back( vertex_map[e1] );
                    connections.push_back( vertex_map[e2] );
                }
            } // x
            ++index;
        } // y
        index += line_size;
    } // z
}

/*==========================================================================*/
/**
 *  @brief  Calculates a normal vector array on the polygon.
 *  @param  coords [in] coordinate array
 *  @param  connections [in] connection array
 *  @param  normals [out] normal vector array
 */
/*==========================================================================*/
void MarchingCubes::calculate_normals_on_polygon(
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

/*==========================================================================*/
/**
 *  @brief  Calculates a normal vector array on the vertex.
 *  @param  coords [in] coordinate array
 *  @param  connections [in] connection array
 *  @param  normals [out] normal vector array
 */
/*==========================================================================*/
void MarchingCubes::calculate_normals_on_vertex(
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

} // end of namesapce kvs
