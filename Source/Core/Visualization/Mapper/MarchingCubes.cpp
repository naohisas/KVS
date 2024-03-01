/****************************************************************************/
/**
 *  @file   MarchingCubes.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "MarchingCubes.h"
#include "MarchingCubesTable.h"
#include <cstring>


namespace kvs
{

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
    m_isolevel( isolevel ),
    m_duplication( duplication )
{
    SuperClass::setNormalType( normal_type );
    this->exec( volume );
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
    BaseClass::setSuccess( false );

    if ( !object )
    {
        kvsMessageError("Input object is NULL.");
        return nullptr;
    }

    const auto* volume = kvs::StructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        kvsMessageError("Input object is not structured volume object.");
        return nullptr;
    }

    if ( volume->veclen() != 1 )
    {
        kvsMessageError("Input volume is not sclar field data.");
        return nullptr;
    }

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( SuperClass::normalType() == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
    }

    this->mapping( volume );

    BaseClass::setSuccess( true );
    return this;
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces.
 *  @param  volume [in] pointer to the volume object
 */
/*==========================================================================*/
void MarchingCubes::mapping( const Volume* volume )
{
    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    if ( m_duplication )
    {
        SuperClass::setPolygonTypeToTriangle();
        SuperClass::setNormalTypeToPolygon();
        SuperClass::setColorTypeToPolygon();
    }
    else
    {
        SuperClass::setPolygonTypeToTriangle();
        SuperClass::setColorTypeToPolygon();
    }

    // Isosurfaces will not be generated from the volume data with the same
    // min. and max. values of the physical quantity.
    const auto min_value = BaseClass::volume()->minValue();
    const auto max_value = BaseClass::volume()->maxValue();
    if ( kvs::Math::Equal( min_value, max_value ) ) { return; }

    // Extract surfaces.
    switch ( volume->values().typeID() )
    {
    case kvs::Type::TypeInt8:   { this->extract_surfaces<kvs::Int8>  ( volume ); break; }
    case kvs::Type::TypeInt16:  { this->extract_surfaces<kvs::Int16> ( volume ); break; }
    case kvs::Type::TypeInt32:  { this->extract_surfaces<kvs::Int32> ( volume ); break; }
    case kvs::Type::TypeInt64:  { this->extract_surfaces<kvs::Int64> ( volume ); break; }
    case kvs::Type::TypeUInt8:  { this->extract_surfaces<kvs::UInt8> ( volume ); break; }
    case kvs::Type::TypeUInt16: { this->extract_surfaces<kvs::UInt16>( volume ); break; }
    case kvs::Type::TypeUInt32: { this->extract_surfaces<kvs::UInt32>( volume ); break; }
    case kvs::Type::TypeUInt64: { this->extract_surfaces<kvs::UInt64>( volume ); break; }
    case kvs::Type::TypeReal32: { this->extract_surfaces<kvs::Real32>( volume ); break; }
    case kvs::Type::TypeReal64: { this->extract_surfaces<kvs::Real64>( volume ); break; }
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
        break;
    }
    }
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingCubes::extract_surfaces( const Volume* volume )
{
    if ( m_duplication )
        this->extract_surfaces_with_duplication<T>( volume );
    else
        this->extract_surfaces_without_duplication<T>( volume );
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces with duplication.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingCubes::extract_surfaces_with_duplication( const Volume* volume )
{
    // Variables wille store calculated the coordinate values and the normal vectors.
    Coords coords;
    Normals normals;

    const auto ncells = volume->resolution() - kvs::Vec3u::Constant(1);
    const auto line_size = kvs::UInt32( volume->numberOfNodesPerLine() );
    const auto slice_size = kvs::UInt32( volume->numberOfNodesPerSlice() );

    const auto min_coord = volume->minObjectCoord();
    const auto max_coord = volume->maxObjectCoord();
    const auto scale_factor = ( max_coord - min_coord ) / kvs::Vec3{ ncells };
    auto interpolate = [&] ( const kvs::Vec3& v1, const kvs::Vec3& v2 )
    {
        const auto p = this->interpolate_vertex<T>( v1, v2 );
        return ( p + min_coord ) * scale_factor;
    };

    // Extract surfaces.
    auto Edge = MarchingCubesTable::TriangleID;
    auto Vert = MarchingCubesTable::VertexID;
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
                for ( size_t i = 0; Edge[ table_index ][i] != -1; i += 3 )
                {
                    // Refer the edge IDs from the TriangleTable by using the table_index.
                    const int e0 = Edge[table_index][i];
                    const int e1 = Edge[table_index][i+2];
                    const int e2 = Edge[table_index][i+1];

                    // Determine vertices for each edge.
                    const auto v0 = kvs::Vec3{ x + Vert[e0][0][0], y + Vert[e0][0][1], z + Vert[e0][0][2] };
                    const auto v1 = kvs::Vec3{ x + Vert[e0][1][0], y + Vert[e0][1][1], z + Vert[e0][1][2] };
                    const auto v2 = kvs::Vec3{ x + Vert[e1][0][0], y + Vert[e1][0][1], z + Vert[e1][0][2] };
                    const auto v3 = kvs::Vec3{ x + Vert[e1][1][0], y + Vert[e1][1][1], z + Vert[e1][1][2] };
                    const auto v4 = kvs::Vec3{ x + Vert[e2][0][0], y + Vert[e2][0][1], z + Vert[e2][0][2] };
                    const auto v5 = kvs::Vec3{ x + Vert[e2][1][0], y + Vert[e2][1][1], z + Vert[e2][1][2] };

                    // Calculate coordinates of the vertices which are composed of the triangle polygon.
                    const auto v01 = interpolate( v0, v1 );
                    const auto v23 = interpolate( v2, v3 );
                    const auto v45 = interpolate( v4, v5 );
                    coords.push_back( v01.x() ); coords.push_back( v01.y() ); coords.push_back( v01.z() );
                    coords.push_back( v23.x() ); coords.push_back( v23.y() ); coords.push_back( v23.z() );
                    coords.push_back( v45.x() ); coords.push_back( v45.y() ); coords.push_back( v45.z() );

                    // Calculate a normal vector for the triangle polygon.
                    const auto n = ( v23 - v01 ).cross( v45 - v01 );
                    normals.push_back( n.x() ); normals.push_back( n.y() ); normals.push_back( n.z() );
                }
            } // end of loop-x
            ++index;
        } // end of loop-y
        index += line_size;
    } // end of loop-z

    if ( coords.size() > 0 )
    {
        SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
        SuperClass::setColor( BaseClass::transferFunction().colorMap().at( m_isolevel ) );
        SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
        SuperClass::setOpacity( 255 );
    }
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces without duplication.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingCubes::extract_surfaces_without_duplication( const Volume* volume )
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

    if ( coords.size() > 0 )
    {
        SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
        SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
        SuperClass::setColor( BaseClass::transferFunction().colorMap().at( m_isolevel ) );
        SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
        SuperClass::setOpacity( 255 );
    }
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
    const auto* volume = kvs::StructuredVolumeObject::DownCast( BaseClass::volume() );

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

    auto v0 = static_cast<double>( values[ v0_index ] );
    auto v1 = static_cast<double>( values[ v1_index ] );
    v0 = kvs::Math::Clamp( v0, BaseClass::volume()->minValue(), BaseClass::volume()->maxValue() );
    v1 = kvs::Math::Clamp( v1, BaseClass::volume()->minValue(), BaseClass::volume()->maxValue() );
    const auto ratio = kvs::Math::Abs( ( m_isolevel - v0 ) / ( v1 - v0 ) );

    const auto x = ( 1.0f - ratio ) * vertex0.x() + ratio * vertex1.x();
    const auto y = ( 1.0f - ratio ) * vertex0.y() + ratio * vertex1.y();
    const auto z = ( 1.0f - ratio ) * vertex0.z() + ratio * vertex1.z();

    return { float(x), float(y), float(z) };
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
    kvs::UInt32*& vertex_map,
    std::vector<kvs::Real32>& coords )
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const auto* volume = kvs::StructuredVolumeObject::DownCast( BaseClass::volume() );

    const kvs::Vec3u resolution( volume->resolution() );
    const kvs::Vec3u ncells( resolution - kvs::Vec3u::Constant(1) );
    const kvs::UInt32 line_size( volume->numberOfNodesPerLine() );
    const kvs::UInt32 slice_size( volume->numberOfNodesPerSlice() );
    const double isolevel = m_isolevel;

    const auto min_coord = volume->minObjectCoord();
    const auto max_coord = volume->maxObjectCoord();
    const auto scale_factor = ( max_coord - min_coord ) / kvs::Vec3{ ncells };
    auto interpolate = [&] ( const kvs::Vec3& v1, const kvs::Vec3& v2 )
    {
        const auto p = this->interpolate_vertex<T>( v1, v2 );
        return ( p + min_coord ) * scale_factor;
    };

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
                        const auto isopoint = interpolate( {x, y, z}, {x+1, y, z} );
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
                        const auto isopoint = interpolate( {x, y, z}, {x, y+1, z} );
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
                        const auto isopoint = interpolate( {x, y, z}, {x, y, z+1} );
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
    kvs::UInt32*& vertex_map,
    std::vector<kvs::UInt32>& connections )
{
    const auto* volume = kvs::StructuredVolumeObject::DownCast( BaseClass::volume() );

    const kvs::Vec3u resolution( volume->resolution() );
    const kvs::Vec3u ncells( resolution - kvs::Vec3u::Constant(1) );
    const kvs::UInt32 line_size( volume->numberOfNodesPerLine() );
    const kvs::UInt32 slice_size( volume->numberOfNodesPerSlice() );

    auto Edge = MarchingCubesTable::TriangleID;
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

                for ( size_t i = 0; Edge[table_index][i] != -1; i += 3 )
                {
                    const int e0 = local_edge[ Edge[table_index][i]   ];
                    const int e1 = local_edge[ Edge[table_index][i+2] ];
                    const int e2 = local_edge[ Edge[table_index][i+1] ];

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
    const Coords& coords,
    const Connects& connections,
    Normals& normals )
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
    const Coords& coords,
    const Connects& connections,
    Normals& normals )
{
    if ( coords.empty() ) { return; }

    normals.resize( coords.size() );
    std::fill( normals.begin(), normals.end(), 0.0f );

    const auto* const coords_ptr = &coords[ 0 ];
    const auto size = connections.size();
    for ( kvs::UInt32 index = 0; index < size; index += 3 )
    {
        const auto coord0_index = 3 * connections[ index     ];
        const auto coord1_index = 3 * connections[ index + 1 ];
        const auto coord2_index = 3 * connections[ index + 2 ];

        const auto v0 = kvs::Vec3( coords_ptr + coord0_index );
        const auto v1 = kvs::Vec3( coords_ptr + coord1_index );
        const auto v2 = kvs::Vec3( coords_ptr + coord2_index );

        const auto normal = kvs::Vec3( ( v1 - v0 ).cross( v2 - v0 ) );

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
