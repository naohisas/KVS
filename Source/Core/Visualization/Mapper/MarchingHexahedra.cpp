/****************************************************************************/
/**
 *  @file MarchingHexahedra.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MarchingHexahedra.cpp 1403 2012-12-07 07:35:18Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MarchingHexahedra.h"
#include "MarchingHexahedraTable.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructs a new MarchingHexahedra class.
 */
/*==========================================================================*/
MarchingHexahedra::MarchingHexahedra():
    kvs::MapperBase(),
    kvs::PolygonObject(),
    m_isolevel( 0 ),
    m_duplication( true )
{
}

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
MarchingHexahedra::MarchingHexahedra(
    const kvs::UnstructuredVolumeObject* volume,
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

    // Extract the surfaces.
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  Destructs.
 */
/*==========================================================================*/
MarchingHexahedra::~MarchingHexahedra()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the input volume object
 *  @return pointer to the polygon object
 */
/*===========================================================================*/
kvs::ObjectBase* MarchingHexahedra::exec( const kvs::ObjectBase* object )
{
    const kvs::ObjectBase::ObjectType object_type = object->objectType();
    if ( object_type == kvs::ObjectBase::Geometry )
    {
        kvsMessageError("Geometry object is not supported.");
        return NULL;
    }

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( SuperClass::normalType() == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
    }

    const kvs::VolumeObjectBase* volume = reinterpret_cast<const kvs::VolumeObjectBase*>( object );
    const kvs::VolumeObjectBase::VolumeType volume_type = volume->volumeType();
    if ( volume_type == kvs::VolumeObjectBase::Unstructured )
    {
        this->mapping( reinterpret_cast<const kvs::UnstructuredVolumeObject*>( object ) );
    }
    else // volume_type == kvs::VolumeObjectBase::Unstructured
    {
        kvsMessageError("Unstructured volume object is not supported.");
        return NULL;
    }

    return this;
}

/*==========================================================================*/
/**
 *  Extracts the surfaces.
 *  @param volume [in] pointer to the volume object
 */
/*==========================================================================*/
void MarchingHexahedra::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 1 )
    {
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
void MarchingHexahedra::extract_surfaces( const kvs::UnstructuredVolumeObject* volume )
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
void MarchingHexahedra::extract_surfaces_with_duplication(
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
    size_t local_index[8];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 8 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = connections[ index + 4 ];
        local_index[1] = connections[ index + 5 ];
        local_index[2] = connections[ index + 6 ];
        local_index[3] = connections[ index + 7 ];
        local_index[4] = connections[ index + 0 ];
        local_index[5] = connections[ index + 1 ];
        local_index[6] = connections[ index + 2 ];
        local_index[7] = connections[ index + 3 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_table_index<T>( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 255 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingHexahedraTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable by using the table_index.
            const int e0 = MarchingHexahedraTable::TriangleID[table_index][i];
            const int e1 = MarchingHexahedraTable::TriangleID[table_index][i+2];
            const int e2 = MarchingHexahedraTable::TriangleID[table_index][i+1];

            // Determine vertices for each edge.
            const int v0 = local_index[MarchingHexahedraTable::VertexID[e0][0]];
            const int v1 = local_index[MarchingHexahedraTable::VertexID[e0][1]];

            const int v2 = local_index[MarchingHexahedraTable::VertexID[e1][0]];
            const int v3 = local_index[MarchingHexahedraTable::VertexID[e1][1]];

            const int v4 = local_index[MarchingHexahedraTable::VertexID[e2][0]];
            const int v5 = local_index[MarchingHexahedraTable::VertexID[e2][1]];

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

    if( coords.size() > 0 ){
        SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
        SuperClass::setColor( color );
        SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
        SuperClass::setOpacity( 255 );
        SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
        SuperClass::setColorType( kvs::PolygonObject::PolygonColor );
        SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
    }
}

/*==========================================================================*/
/**
 *  Calculate a index of the marching hexahedra table.
 *  @param local_index [in] indices of the cell
 *  @return table index
 */
/*==========================================================================*/
template <typename T>
size_t MarchingHexahedra::calculate_table_index( const size_t* local_index ) const
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
 *  Interpolates a coordinate value of the intersected vertex.
 *  @param vertex0 [in] vertex coordinate of the end point #0
 *  @param vertex1 [in] vertex coordinate of the end point #1
 *  @return interpolated vertex coordinate
 */
/*==========================================================================*/
template <typename T>
const kvs::Vec3 MarchingHexahedra::interpolate_vertex(
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
 *  Calculates a color of the surfaces from the isolevel.
 *  @return surface color
 */
/*==========================================================================*/
template <typename T>
const kvs::RGBColor MarchingHexahedra::calculate_color()
{
    const kvs::Real64 min_value = BaseClass::volume()->minValue();
    const kvs::Real64 max_value = BaseClass::volume()->maxValue();
    const kvs::Real64 normalize_factor = 255.0 / ( max_value - min_value );
    const kvs::UInt8  index = static_cast<kvs::UInt8>( normalize_factor * ( m_isolevel - min_value ) );

    return BaseClass::transferFunction().colorMap()[ index ];
}

} // end of namesapce kvs
