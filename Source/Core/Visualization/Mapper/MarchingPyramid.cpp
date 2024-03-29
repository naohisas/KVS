/****************************************************************************/
/**
 *  @file   MarchingPyramid.cpp
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/****************************************************************************/
#include "MarchingPyramid.h"
#include "MarchingPyramidTable.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructs and creates a polygon object.
 *  @param volume [in] pointer to the volume object
 *  @param isolevel [in] level of the isosurfaces
 *  @param normal_type [in] type of the normal vector
 *  @param duplication [in] duplication flag
 *  @param transfer_function [in] transfer function
 */
/*==========================================================================*/
MarchingPyramid::MarchingPyramid(
    const kvs::UnstructuredVolumeObject* volume,
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
kvs::ObjectBase* MarchingPyramid::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return nullptr;
    }

    const auto* volume = kvs::UnstructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not unstructured volume object.");
        return nullptr;
    }

    if ( volume->veclen() != 1 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not sclar field data.");
        return nullptr;
    }

    if ( volume->cellType() != kvs::UnstructuredVolumeObject::Pyramid )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not pyramid-cell data.");
        return nullptr;
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
 *  Extracts the surfaces.
 *  @param volume [in] pointer to the volume object
 */
/*==========================================================================*/
void MarchingPyramid::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::PolygonColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );

    const auto min_value = BaseClass::volume()->minValue();
    const auto max_value = BaseClass::volume()->maxValue();
    if ( kvs::Math::Equal( min_value, max_value ) ) { return; }

    // Extract surfaces.
    const auto& type = volume->values().typeInfo()->type();
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
        kvsMessageError("Unsupported data type '%s' of the volume.",
                        volume->values().typeInfo()->typeName() );
    }
}

/*==========================================================================*/
/**
 *  Extracts the surfaces.
 *  @param volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingPyramid::extract_surfaces( const kvs::UnstructuredVolumeObject* volume )
{
    if ( m_duplication ) this->extract_surfaces_with_duplication<T>( volume );
}

/*==========================================================================*/
/**
 *  Extracts the surfaces with duplication.
 *  @param volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MarchingPyramid::extract_surfaces_with_duplication(
    const kvs::UnstructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;

    const kvs::UInt32 ncells( volume->numberOfCells() );
    const kvs::UInt32* connections =
        static_cast<const kvs::UInt32*>( volume->connections().data() );

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[5];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 5 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = connections[ index + 0 ];
        local_index[1] = connections[ index + 1 ];
        local_index[2] = connections[ index + 2 ];
        local_index[3] = connections[ index + 3 ];
        local_index[4] = connections[ index + 4 ];

        // Calculate the index of the reference table.
        size_t table_index = this->calculate_table_index<T>( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 10 || table_index == 11 || table_index == 20 || table_index == 21 ){
            table_index = this->calculate_special_table_index<T>( local_index, table_index );
        }
        if ( table_index == 36 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingPyramidTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable by using the table_index.
            const int e0 = MarchingPyramidTable::TriangleID[table_index][i];
            const int e1 = MarchingPyramidTable::TriangleID[table_index][i+2];
            const int e2 = MarchingPyramidTable::TriangleID[table_index][i+1];

            // Determine vertices for each edge.
            const int v0 = local_index[MarchingPyramidTable::VertexID[e0][0]];
            const int v1 = local_index[MarchingPyramidTable::VertexID[e0][1]];

            const int v2 = local_index[MarchingPyramidTable::VertexID[e1][0]];
            const int v3 = local_index[MarchingPyramidTable::VertexID[e1][1]];

            const int v4 = local_index[MarchingPyramidTable::VertexID[e2][0]];
            const int v5 = local_index[MarchingPyramidTable::VertexID[e2][1]];

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
 *  Calculate a index of the marching pyramid table.
 *  @param local_index [in] indices of the cell
 *  @return table index
 */
/*==========================================================================*/
template <typename T>
size_t MarchingPyramid::calculate_table_index( const size_t* local_index ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const double isolevel = m_isolevel;

    size_t table_index = 0;
    if ( static_cast<double>( values[ local_index[0] ] ) > isolevel ) { table_index |=   1; }
    if ( static_cast<double>( values[ local_index[1] ] ) > isolevel ) { table_index |=   2; }
    if ( static_cast<double>( values[ local_index[2] ] ) > isolevel ) { table_index |=   4; }
    if ( static_cast<double>( values[ local_index[3] ] ) > isolevel ) { table_index |=   8; }
    if ( static_cast<double>( values[ local_index[4] ] ) > isolevel ) { table_index |=  16; }

    return table_index;
}

/*==========================================================================*/
/**
 *  Interpolates a coordinate value of the intersected vertex.
 *  @param vertex0 [in] vertex coordinate of the end point #0
 *  @param vertex1 [in] vertex coordinate of the end point #1
 *  @return interpolated vertex coordinate
 */
/*==========================================================================*/
template <typename T>
const kvs::Vec3 MarchingPyramid::interpolate_vertex(
    const int vertex0,
    const int vertex1 ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const kvs::Real32* const coords = BaseClass::volume()->coords().data();

    const size_t coord0_index = 3 * vertex0;
    const size_t coord1_index = 3 * vertex1;

    auto v0 = static_cast<double>( values[ vertex0 ] );
    auto v1 = static_cast<double>( values[ vertex1 ] );
    v0 = kvs::Math::Clamp( v0, BaseClass::volume()->minValue(), BaseClass::volume()->maxValue() );
    v1 = kvs::Math::Clamp( v1, BaseClass::volume()->minValue(), BaseClass::volume()->maxValue() );
    const auto ratio = kvs::Math::Abs( ( m_isolevel - v0 ) / ( v1 - v0 ) );

    const auto x = coords[coord0_index]   + ratio * ( coords[coord1_index]   - coords[coord0_index] );
    const auto y = coords[coord0_index+1] + ratio * ( coords[coord1_index+1] - coords[coord0_index+1] );
    const auto z = coords[coord0_index+2] + ratio * ( coords[coord1_index+2] - coords[coord0_index+2] );

    return { float(x), float(y), float(z) };
}

/*==========================================================================*/
/**
 *  Calculate a special index of the marching pyramid table.
 *  @param local_index [in] indices of the cell
 *  @return table index
 */
/*==========================================================================*/
template <typename T>
size_t MarchingPyramid::calculate_special_table_index( const size_t* local_index, const size_t index ) const
{
    const T* const values = static_cast<const T*>( BaseClass::volume()->values().data() );
    const double isolevel = m_isolevel;
    double v1 = static_cast<double>( values[ local_index[1] ] );
    double v2 = static_cast<double>( values[ local_index[2] ] );
    double v3 = static_cast<double>( values[ local_index[3] ] );
    double v4 = static_cast<double>( values[ local_index[4] ] );

    size_t table_index = index;

    double A = (   v1 + v2 + v3 + v4 ) / 4;
    double B = ( - v1 + v2 + v3 - v4 ) / 4;
    double C = ( - v1 - v2 + v3 + v4 ) / 4;
    double D = (   v1 - v2 + v3 - v4 ) / 4;
    double E = ( isolevel - A ) * D + B * C;
    if( index == 10 && E < 0 ) table_index = 32;
    if( index == 11 && E >= 0 ) table_index = 33;
    if( index == 20 && E >= 0 ) table_index = 34;
    if( index == 21 && E < 0 ) table_index = 35;

    return table_index;
}

} // end of namesapce kvs
