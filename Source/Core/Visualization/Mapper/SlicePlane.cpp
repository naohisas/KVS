/****************************************************************************/
/**
 *  @file   SlicePlane.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SlicePlane.cpp 1761 2014-05-07 04:18:15Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "SlicePlane.h"
#include <kvs/MarchingCubesTable>
#include <kvs/MarchingTetrahedraTable>
#include <kvs/MarchingHexahedraTable>
#include <kvs/MarchingPyramidTable>
#include <kvs/MarchingPrismTable>

namespace kvs
{

/*==========================================================================*/
/**
 *  Constructs a new SlicePlane class.
 */
/*==========================================================================*/
SlicePlane::SlicePlane():
    kvs::MapperBase(),
    kvs::PolygonObject()
{
}

/*==========================================================================*/
/**
 *  Constructs and creates a polygon object.
 *  @param volume [in] pointer to the volume object
 *  @param coefficients [in] coefficients of the plane
 *  @param transfer_function [in] transfer function
 */
/*==========================================================================*/
SlicePlane::SlicePlane(
    const kvs::VolumeObjectBase* volume,
    const kvs::Vector4f&         coefficients,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PolygonObject()
{
    this->setPlane( coefficients );
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  Constructs and creates a polygon object.
 *  @param volume [in] pointer to the volume object
 *  @param point [in] point on the plane
 *  @param normal [in] normal vector of the plane
 *  @param transfer_function [in] transfer function
 */
/*==========================================================================*/
SlicePlane::SlicePlane(
    const kvs::VolumeObjectBase* volume,
    const kvs::Vector3f&         point,
    const kvs::Vector3f&         normal,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PolygonObject()
{
    this->setPlane( point, normal );
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  Destructs.
 */
/*==========================================================================*/
SlicePlane::~SlicePlane()
{
}

/*===========================================================================*/
/**
 *  @brief  Sets the plane by coeffients of the plane equation.
 *  @param  coefficients [in] coefficients of the plane equation
 */
/*===========================================================================*/
void SlicePlane::setPlane( const kvs::Vector4f& coefficients )
{
    m_coefficients = coefficients;
}

/*===========================================================================*/
/**
 *  @brief  Sets the plane by a point and a normal vector.
 *  @param  point  [in] position of any point on the plane
 *  @param  normal [in] normal vector of the plane
 */
/*===========================================================================*/
void SlicePlane::setPlane( const kvs::Vector3f& point, const kvs::Vector3f& normal )
{
    m_coefficients = kvs::Vector4f( normal, -point.dot( normal ) );
}

/*===========================================================================*/
/**
 *  @brief  Executes the slice plane.
 *  @param  object [in] pointer to the object (volume object)
 *  @return pointer to the sliced plane (polygon object)
 */
/*===========================================================================*/
SlicePlane::SuperClass* SlicePlane::exec( const kvs::ObjectBase* object )
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

    this->mapping( volume );

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Extracts the plane.
 *  @param  volume [in] pointer to the volume object
 */
/*==========================================================================*/
void SlicePlane::mapping( const kvs::VolumeObjectBase* volume )
{
    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 1 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not a sclar field data.");
        return;
    }

    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    if ( volume->volumeType() == kvs::VolumeObjectBase::Structured )
    {
        const kvs::StructuredVolumeObject* structured_volume =
            kvs::StructuredVolumeObject::DownCast( volume );

        const std::type_info& type = structured_volume->values().typeInfo()->type();
        if (      type == typeid( kvs::Int8   ) ) this->extract_plane<kvs::Int8>( structured_volume );
        else if ( type == typeid( kvs::Int16  ) ) this->extract_plane<kvs::Int16>( structured_volume );
        else if ( type == typeid( kvs::Int32  ) ) this->extract_plane<kvs::Int32>( structured_volume );
        else if ( type == typeid( kvs::Int64  ) ) this->extract_plane<kvs::Int64>( structured_volume );
        else if ( type == typeid( kvs::UInt8  ) ) this->extract_plane<kvs::UInt8>( structured_volume );
        else if ( type == typeid( kvs::UInt16 ) ) this->extract_plane<kvs::UInt16>( structured_volume );
        else if ( type == typeid( kvs::UInt32 ) ) this->extract_plane<kvs::UInt32>( structured_volume );
        else if ( type == typeid( kvs::UInt64 ) ) this->extract_plane<kvs::UInt64>( structured_volume );
        else if ( type == typeid( kvs::Real32 ) ) this->extract_plane<kvs::Real32>( structured_volume );
        else if ( type == typeid( kvs::Real64 ) ) this->extract_plane<kvs::Real64>( structured_volume );
        else
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Unsupported data type '%s'.", structured_volume->values().typeInfo()->typeName() );
        }
    }
    else // volume->volumeType() == kvs::VolumeObjectBase::Unstructured
    {
        const kvs::UnstructuredVolumeObject* unstructured_volume =
            kvs::UnstructuredVolumeObject::DownCast( volume );

        const std::type_info& type = unstructured_volume->values().typeInfo()->type();
        if (      type == typeid( kvs::Int8   ) ) this->extract_plane<kvs::Int8>( unstructured_volume );
        else if ( type == typeid( kvs::Int16  ) ) this->extract_plane<kvs::Int16>( unstructured_volume );
        else if ( type == typeid( kvs::Int32  ) ) this->extract_plane<kvs::Int32>( unstructured_volume );
        else if ( type == typeid( kvs::Int64  ) ) this->extract_plane<kvs::Int64>( unstructured_volume );
        else if ( type == typeid( kvs::UInt8  ) ) this->extract_plane<kvs::UInt8>( unstructured_volume );
        else if ( type == typeid( kvs::UInt16 ) ) this->extract_plane<kvs::UInt16>( unstructured_volume );
        else if ( type == typeid( kvs::UInt32 ) ) this->extract_plane<kvs::UInt32>( unstructured_volume );
        else if ( type == typeid( kvs::UInt64 ) ) this->extract_plane<kvs::UInt64>( unstructured_volume );
        else if ( type == typeid( kvs::Real32 ) ) this->extract_plane<kvs::Real32>( unstructured_volume );
        else if ( type == typeid( kvs::Real64 ) ) this->extract_plane<kvs::Real64>( unstructured_volume );
        else
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Unsupported data type '%s'.", unstructured_volume->values().typeInfo()->typeName() );
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Extract a slice plane for a structured volume.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void SlicePlane::extract_plane(
    const kvs::StructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;
    std::vector<kvs::UInt8>  colors;

    // Calculate min/max values of the node data.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    // Calculate a normalize_factor.
    const kvs::Real64 min_value( volume->minValue() );
    const kvs::Real64 max_value( volume->maxValue() );
    const kvs::Real64 normalize_factor( 255.0 / ( max_value - min_value ) );

    const kvs::Vector3ui ncells( volume->resolution() - kvs::Vector3ui::Constant(1) );
    const kvs::UInt32    line_size( volume->numberOfNodesPerLine() );
    const kvs::ColorMap& color_map( BaseClass::transferFunction().colorMap() );

    // Extract surfaces.
    size_t index = 0;
    for ( kvs::UInt32 z = 0; z < ncells.z(); ++z )
    {
        for ( kvs::UInt32 y = 0; y < ncells.y(); ++y )
        {
            for ( kvs::UInt32 x = 0; x < ncells.x(); ++x )
            {
                // Calculate the index of the reference table.
                const size_t table_index = this->calculate_table_index( x, y, z );
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
                    const kvs::Vector3f v0(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e0][0][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e0][0][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e0][0][2] ) );

                    const kvs::Vector3f v1(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e0][1][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e0][1][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e0][1][2] ) );

                    const kvs::Vector3f v2(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e1][0][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e1][0][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e1][0][2] ) );

                    const kvs::Vector3f v3(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e1][1][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e1][1][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e1][1][2] ) );

                    const kvs::Vector3f v4(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e2][0][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e2][0][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e2][0][2] ) );

                    const kvs::Vector3f v5(
                        static_cast<float>( x + MarchingCubesTable::VertexID[e2][1][0] ),
                        static_cast<float>( y + MarchingCubesTable::VertexID[e2][1][1] ),
                        static_cast<float>( z + MarchingCubesTable::VertexID[e2][1][2] ) );

                    // Calculate coordinates of the vertices which are composed
                    // of the triangle polygon.
                    const kvs::Vector3f vertex0( this->interpolate_vertex( v0, v1 ) );
                    coords.push_back( vertex0.x() );
                    coords.push_back( vertex0.y() );
                    coords.push_back( vertex0.z() );

                    const kvs::Vector3f vertex1( this->interpolate_vertex( v2, v3 ) );
                    coords.push_back( vertex1.x() );
                    coords.push_back( vertex1.y() );
                    coords.push_back( vertex1.z() );

                    const kvs::Vector3f vertex2( this->interpolate_vertex( v4, v5 ) );
                    coords.push_back( vertex2.x() );
                    coords.push_back( vertex2.y() );
                    coords.push_back( vertex2.z() );

                    const double value0 = this->interpolate_value<T>( volume, v0, v1 );
                    const double value1 = this->interpolate_value<T>( volume, v2, v3 );
                    const double value2 = this->interpolate_value<T>( volume, v4, v5 );

                    const kvs::UInt8 color0 =
                        static_cast<kvs::UInt8>( normalize_factor * ( value0 - min_value ) );
                    colors.push_back( color_map[ color0 ].r() );
                    colors.push_back( color_map[ color0 ].g() );
                    colors.push_back( color_map[ color0 ].b() );

                    const kvs::UInt8 color1 =
                        static_cast<kvs::UInt8>( normalize_factor * ( value1 - min_value ) );
                    colors.push_back( color_map[ color1 ].r() );
                    colors.push_back( color_map[ color1 ].g() );
                    colors.push_back( color_map[ color1 ].b() );

                    const kvs::UInt8 color2 =
                        static_cast<kvs::UInt8>( normalize_factor * ( value2 - min_value ) );
                    colors.push_back( color_map[ color2 ].r() );
                    colors.push_back( color_map[ color2 ].g() );
                    colors.push_back( color_map[ color2 ].b() );

                    // Calculate a normal vector for the triangle polygon.
                    const kvs::Vector3f normal( -( vertex2 - vertex0 ).cross( vertex1 - vertex0 ) );
                    normals.push_back( normal.x() );
                    normals.push_back( normal.y() );
                    normals.push_back( normal.z() );
                } // end of loop-triangle
            } // end of loop-x
            ++index;
        } // end of loop-y
        index += line_size;
    } // end of loop-z

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*==========================================================================*/
/**
 *  @brief  Extract a slice plane for a unstructured volume.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void SlicePlane::extract_plane(
    const kvs::UnstructuredVolumeObject* volume )
{
    switch ( volume->cellType() )
    {
        case kvs::UnstructuredVolumeObject::Tetrahedra:
        {
            this->extract_tetrahedra_plane<T>( volume );
            break;
        }
        case kvs::UnstructuredVolumeObject::Hexahedra:
        {
            this->extract_hexahedra_plane<T>( volume );
            break;
        }
        case kvs::UnstructuredVolumeObject::Pyramid:
        {
            this->extract_pyramid_plane<T>( volume );
            break;
        }
        case kvs::UnstructuredVolumeObject::Prism:
        {
            this->extract_prism_plane<T>( volume );
            break;
        }
        default: break;
    }
}
/*==========================================================================*/
/**
 *  @brief  Extract a slice plane for a unstructured volume.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void SlicePlane::extract_tetrahedra_plane(
    const kvs::UnstructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;
    std::vector<kvs::UInt8>  colors;

    // Calculate min/max values of the node data.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    // Calculate a normalize factor.
    const kvs::Real64 min_value( volume->minValue() );
    const kvs::Real64 max_value( volume->maxValue() );
    const kvs::Real64 normalize_factor( 255.0 / ( max_value - min_value ) );

    // Refer the parameters of the unstructured volume object.
    const kvs::Real32* volume_coords      = volume->coords().data();
    const kvs::UInt32* volume_connections = volume->connections().data();
    const size_t       ncells             = volume->numberOfCells();

    const kvs::ColorMap& color_map( BaseClass::transferFunction().colorMap() );

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[4];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 4 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = volume_connections[ index ];
        local_index[1] = volume_connections[ index + 1 ];
        local_index[2] = volume_connections[ index + 2 ];
        local_index[3] = volume_connections[ index + 3 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_tetrahedra_table_index( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 15 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingTetrahedraTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable using the table_index.
            const int e0 = MarchingTetrahedraTable::TriangleID[table_index][i];
            const int e1 = MarchingTetrahedraTable::TriangleID[table_index][i+1];
            const int e2 = MarchingTetrahedraTable::TriangleID[table_index][i+2];

            // Refer indices of the coordinate array from the VertexTable using the edgeIDs.
            const size_t c0 = local_index[ MarchingTetrahedraTable::VertexID[e0][0] ];
            const size_t c1 = local_index[ MarchingTetrahedraTable::VertexID[e0][1] ];
            const size_t c2 = local_index[ MarchingTetrahedraTable::VertexID[e1][0] ];
            const size_t c3 = local_index[ MarchingTetrahedraTable::VertexID[e1][1] ];
            const size_t c4 = local_index[ MarchingTetrahedraTable::VertexID[e2][0] ];
            const size_t c5 = local_index[ MarchingTetrahedraTable::VertexID[e2][1] ];

            // Determine vertices for each edge.
            const kvs::Vector3f v0( volume_coords + 3 * c0 );
            const kvs::Vector3f v1( volume_coords + 3 * c1 );

            const kvs::Vector3f v2( volume_coords + 3 * c2 );
            const kvs::Vector3f v3( volume_coords + 3 * c3 );

            const kvs::Vector3f v4( volume_coords + 3 * c4 );
            const kvs::Vector3f v5( volume_coords + 3 * c5 );

            // Calculate coordinates of the vertices which are composed
            // of the triangle polygon.
            const kvs::Vector3f vertex0( this->interpolate_vertex( v0, v1 ) );
            coords.push_back( vertex0.x() );
            coords.push_back( vertex0.y() );
            coords.push_back( vertex0.z() );

            const kvs::Vector3f vertex1( this->interpolate_vertex( v2, v3 ) );
            coords.push_back( vertex1.x() );
            coords.push_back( vertex1.y() );
            coords.push_back( vertex1.z() );

            const kvs::Vector3f vertex2( this->interpolate_vertex( v4, v5 ) );
            coords.push_back( vertex2.x() );
            coords.push_back( vertex2.y() );
            coords.push_back( vertex2.z() );

            const double value0 = this->interpolate_value<T>( volume, c0, c1 );
            const double value1 = this->interpolate_value<T>( volume, c2, c3 );
            const double value2 = this->interpolate_value<T>( volume, c4, c5 );

            const kvs::UInt8 color0 =
                static_cast<kvs::UInt8>( normalize_factor * ( value0 - min_value ) );
            colors.push_back( color_map[ color0 ].r() );
            colors.push_back( color_map[ color0 ].g() );
            colors.push_back( color_map[ color0 ].b() );

            const kvs::UInt8 color1 =
                static_cast<kvs::UInt8>( normalize_factor * ( value1 - min_value ) );
            colors.push_back( color_map[ color1 ].r() );
            colors.push_back( color_map[ color1 ].g() );
            colors.push_back( color_map[ color1 ].b() );

            const kvs::UInt8 color2 =
                static_cast<kvs::UInt8>( normalize_factor * ( value2 - min_value ) );
            colors.push_back( color_map[ color2 ].r() );
            colors.push_back( color_map[ color2 ].g() );
            colors.push_back( color_map[ color2 ].b() );

            // Calculate a normal vector for the triangle polygon.
            const kvs::Vector3f normal( -( vertex2 - vertex0 ).cross( vertex1 - vertex0 ) );
            normals.push_back( normal.x() );
            normals.push_back( normal.y() );
            normals.push_back( normal.z() );
        } // end of loop-triangle
    } // end of loop-cell

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*==========================================================================*/
/**
 *  @brief  Extract a slice plane for a unstructured volume.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void SlicePlane::extract_hexahedra_plane(
    const kvs::UnstructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;
    std::vector<kvs::UInt8>  colors;

    // Calculate min/max values of the node data.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    // Calculate a normalize factor.
    const kvs::Real64 min_value( volume->minValue() );
    const kvs::Real64 max_value( volume->maxValue() );
    const kvs::Real64 normalize_factor( 255.0 / ( max_value - min_value ) );

    // Refer the parameters of the unstructured volume object.
    const kvs::Real32* volume_coords      = volume->coords().data();
    const kvs::UInt32* volume_connections = volume->connections().data();
    const size_t       ncells             = volume->numberOfCells();

    const kvs::ColorMap& color_map( BaseClass::transferFunction().colorMap() );

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[8];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 8 )
    {
        // Calculate the indices of the target cell.
        local_index[4] = volume_connections[ index ];
        local_index[5] = volume_connections[ index + 1 ];
        local_index[6] = volume_connections[ index + 2 ];
        local_index[7] = volume_connections[ index + 3 ];
        local_index[0] = volume_connections[ index + 4 ];
        local_index[1] = volume_connections[ index + 5 ];
        local_index[2] = volume_connections[ index + 6 ];
        local_index[3] = volume_connections[ index + 7 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_hexahedra_table_index( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 255 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingHexahedraTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable using the table_index.
            const int e0 = MarchingHexahedraTable::TriangleID[table_index][i];
            const int e1 = MarchingHexahedraTable::TriangleID[table_index][i+1];
            const int e2 = MarchingHexahedraTable::TriangleID[table_index][i+2];

            // Refer indices of the coordinate array from the VertexTable using the edgeIDs.
            const size_t c0 = local_index[ MarchingHexahedraTable::VertexID[e0][0] ];
            const size_t c1 = local_index[ MarchingHexahedraTable::VertexID[e0][1] ];
            const size_t c2 = local_index[ MarchingHexahedraTable::VertexID[e1][0] ];
            const size_t c3 = local_index[ MarchingHexahedraTable::VertexID[e1][1] ];
            const size_t c4 = local_index[ MarchingHexahedraTable::VertexID[e2][0] ];
            const size_t c5 = local_index[ MarchingHexahedraTable::VertexID[e2][1] ];

            // Determine vertices for each edge.
            const kvs::Vector3f v0( volume_coords + 3 * c0 );
            const kvs::Vector3f v1( volume_coords + 3 * c1 );

            const kvs::Vector3f v2( volume_coords + 3 * c2 );
            const kvs::Vector3f v3( volume_coords + 3 * c3 );

            const kvs::Vector3f v4( volume_coords + 3 * c4 );
            const kvs::Vector3f v5( volume_coords + 3 * c5 );

            // Calculate coordinates of the vertices which are composed
            // of the triangle polygon.
            const kvs::Vector3f vertex0( this->interpolate_vertex( v0, v1 ) );
            coords.push_back( vertex0.x() );
            coords.push_back( vertex0.y() );
            coords.push_back( vertex0.z() );

            const kvs::Vector3f vertex1( this->interpolate_vertex( v2, v3 ) );
            coords.push_back( vertex1.x() );
            coords.push_back( vertex1.y() );
            coords.push_back( vertex1.z() );

            const kvs::Vector3f vertex2( this->interpolate_vertex( v4, v5 ) );
            coords.push_back( vertex2.x() );
            coords.push_back( vertex2.y() );
            coords.push_back( vertex2.z() );

            const double value0 = this->interpolate_value<T>( volume, c0, c1 );
            const double value1 = this->interpolate_value<T>( volume, c2, c3 );
            const double value2 = this->interpolate_value<T>( volume, c4, c5 );

            const kvs::UInt8 color0 =
                static_cast<kvs::UInt8>( normalize_factor * ( value0 - min_value ) );
            colors.push_back( color_map[ color0 ].r() );
            colors.push_back( color_map[ color0 ].g() );
            colors.push_back( color_map[ color0 ].b() );

            const kvs::UInt8 color1 =
                static_cast<kvs::UInt8>( normalize_factor * ( value1 - min_value ) );
            colors.push_back( color_map[ color1 ].r() );
            colors.push_back( color_map[ color1 ].g() );
            colors.push_back( color_map[ color1 ].b() );

            const kvs::UInt8 color2 =
                static_cast<kvs::UInt8>( normalize_factor * ( value2 - min_value ) );
            colors.push_back( color_map[ color2 ].r() );
            colors.push_back( color_map[ color2 ].g() );
            colors.push_back( color_map[ color2 ].b() );

            // Calculate a normal vector for the triangle polygon.
            const kvs::Vector3f normal( -( vertex2 - vertex0 ).cross( vertex1 - vertex0 ) );
            normals.push_back( normal.x() );
            normals.push_back( normal.y() );
            normals.push_back( normal.z() );
        } // end of loop-triangle
    } // end of loop-cell

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*==========================================================================*/
/**
 *  @brief  Extract a slice plane for a unstructured volume.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void SlicePlane::extract_pyramid_plane(
    const kvs::UnstructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;
    std::vector<kvs::UInt8>  colors;

    // Calculate min/max values of the node data.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    // Calculate a normalize factor.
    const kvs::Real64 min_value( volume->minValue() );
    const kvs::Real64 max_value( volume->maxValue() );
    const kvs::Real64 normalize_factor( 255.0 / ( max_value - min_value ) );

    // Refer the parameters of the unstructured volume object.
    const kvs::Real32* volume_coords      = volume->coords().data();
    const kvs::UInt32* volume_connections = volume->connections().data();
    const size_t       ncells             = volume->numberOfCells();

    const kvs::ColorMap& color_map( BaseClass::transferFunction().colorMap() );

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[5];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 5 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = volume_connections[ index ];
        local_index[1] = volume_connections[ index + 1 ];
        local_index[2] = volume_connections[ index + 2 ];
        local_index[3] = volume_connections[ index + 3 ];
        local_index[4] = volume_connections[ index + 4 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_pyramid_table_index( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 31 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingPyramidTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable using the table_index.
            const int e0 = MarchingPyramidTable::TriangleID[table_index][i];
            const int e1 = MarchingPyramidTable::TriangleID[table_index][i+1];
            const int e2 = MarchingPyramidTable::TriangleID[table_index][i+2];

            // Refer indices of the coordinate array from the VertexTable using the edgeIDs.
            const size_t c0 = local_index[ MarchingPyramidTable::VertexID[e0][0] ];
            const size_t c1 = local_index[ MarchingPyramidTable::VertexID[e0][1] ];
            const size_t c2 = local_index[ MarchingPyramidTable::VertexID[e1][0] ];
            const size_t c3 = local_index[ MarchingPyramidTable::VertexID[e1][1] ];
            const size_t c4 = local_index[ MarchingPyramidTable::VertexID[e2][0] ];
            const size_t c5 = local_index[ MarchingPyramidTable::VertexID[e2][1] ];

            // Determine vertices for each edge.
            const kvs::Vector3f v0( volume_coords + 3 * c0 );
            const kvs::Vector3f v1( volume_coords + 3 * c1 );

            const kvs::Vector3f v2( volume_coords + 3 * c2 );
            const kvs::Vector3f v3( volume_coords + 3 * c3 );

            const kvs::Vector3f v4( volume_coords + 3 * c4 );
            const kvs::Vector3f v5( volume_coords + 3 * c5 );

            // Calculate coordinates of the vertices which are composed
            // of the triangle polygon.
            const kvs::Vector3f vertex0( this->interpolate_vertex( v0, v1 ) );
            coords.push_back( vertex0.x() );
            coords.push_back( vertex0.y() );
            coords.push_back( vertex0.z() );

            const kvs::Vector3f vertex1( this->interpolate_vertex( v2, v3 ) );
            coords.push_back( vertex1.x() );
            coords.push_back( vertex1.y() );
            coords.push_back( vertex1.z() );

            const kvs::Vector3f vertex2( this->interpolate_vertex( v4, v5 ) );
            coords.push_back( vertex2.x() );
            coords.push_back( vertex2.y() );
            coords.push_back( vertex2.z() );

            const double value0 = this->interpolate_value<T>( volume, c0, c1 );
            const double value1 = this->interpolate_value<T>( volume, c2, c3 );
            const double value2 = this->interpolate_value<T>( volume, c4, c5 );

            const kvs::UInt8 color0 =
                static_cast<kvs::UInt8>( normalize_factor * ( value0 - min_value ) );
            colors.push_back( color_map[ color0 ].r() );
            colors.push_back( color_map[ color0 ].g() );
            colors.push_back( color_map[ color0 ].b() );

            const kvs::UInt8 color1 =
                static_cast<kvs::UInt8>( normalize_factor * ( value1 - min_value ) );
            colors.push_back( color_map[ color1 ].r() );
            colors.push_back( color_map[ color1 ].g() );
            colors.push_back( color_map[ color1 ].b() );

            const kvs::UInt8 color2 =
                static_cast<kvs::UInt8>( normalize_factor * ( value2 - min_value ) );
            colors.push_back( color_map[ color2 ].r() );
            colors.push_back( color_map[ color2 ].g() );
            colors.push_back( color_map[ color2 ].b() );

            // Calculate a normal vector for the triangle polygon.
            const kvs::Vector3f normal( -( vertex2 - vertex0 ).cross( vertex1 - vertex0 ) );
            normals.push_back( normal.x() );
            normals.push_back( normal.y() );
            normals.push_back( normal.z() );
        } // end of loop-triangle
    } // end of loop-cell

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}


/*==========================================================================*/
/**
 *  @brief  Extract a slice plane for a unstructured volume.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void SlicePlane::extract_prism_plane(
    const kvs::UnstructuredVolumeObject* volume )
{
    // Calculated the coordinate data array and the normal vector array.
    std::vector<kvs::Real32> coords;
    std::vector<kvs::Real32> normals;
    std::vector<kvs::UInt8>  colors;

    // Calculate min/max values of the node data.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    // Calculate a normalize factor.
    const kvs::Real64 min_value( volume->minValue() );
    const kvs::Real64 max_value( volume->maxValue() );
    const kvs::Real64 normalize_factor( 255.0 / ( max_value - min_value ) );

    // Refer the parameters of the unstructured volume object.
    const kvs::Real32* volume_coords      = volume->coords().data();
    const kvs::UInt32* volume_connections = volume->connections().data();
    const size_t       ncells             = volume->numberOfCells();

    const kvs::ColorMap& color_map( BaseClass::transferFunction().colorMap() );

    // Extract surfaces.
    size_t index = 0;
    size_t local_index[6];
    for ( kvs::UInt32 cell = 0; cell < ncells; ++cell, index += 6 )
    {
        // Calculate the indices of the target cell.
        local_index[0] = volume_connections[ index + 0 ];
        local_index[1] = volume_connections[ index + 1 ];
        local_index[2] = volume_connections[ index + 2 ];
        local_index[3] = volume_connections[ index + 3 ];
        local_index[4] = volume_connections[ index + 4 ];
        local_index[5] = volume_connections[ index + 5 ];

        // Calculate the index of the reference table.
        const size_t table_index = this->calculate_prism_table_index( local_index );
        if ( table_index == 0 ) continue;
        if ( table_index == 63 ) continue;

        // Calculate the triangle polygons.
        for ( size_t i = 0; MarchingPrismTable::TriangleID[ table_index ][i] != -1; i += 3 )
        {
            // Refer the edge IDs from the TriangleTable using the table_index.
            const int e0 = MarchingPrismTable::TriangleID[table_index][i+0];
            const int e1 = MarchingPrismTable::TriangleID[table_index][i+1];
            const int e2 = MarchingPrismTable::TriangleID[table_index][i+2];

            // Refer indices of the coordinate array from the VertexTable using the edgeIDs.
            const size_t c0 = local_index[ MarchingPrismTable::VertexID[e0][0] ];
            const size_t c1 = local_index[ MarchingPrismTable::VertexID[e0][1] ];
            const size_t c2 = local_index[ MarchingPrismTable::VertexID[e1][0] ];
            const size_t c3 = local_index[ MarchingPrismTable::VertexID[e1][1] ];
            const size_t c4 = local_index[ MarchingPrismTable::VertexID[e2][0] ];
            const size_t c5 = local_index[ MarchingPrismTable::VertexID[e2][1] ];

            // Determine vertices for each edge.
            const kvs::Vec3 v0( volume_coords + 3 * c0 );
            const kvs::Vec3 v1( volume_coords + 3 * c1 );

            const kvs::Vec3 v2( volume_coords + 3 * c2 );
            const kvs::Vec3 v3( volume_coords + 3 * c3 );

            const kvs::Vec3 v4( volume_coords + 3 * c4 );
            const kvs::Vec3 v5( volume_coords + 3 * c5 );

            // Calculate coordinates of the vertices which are composed
            // of the triangle polygon.
            const kvs::Vec3 vertex0( this->interpolate_vertex( v0, v1 ) );
            coords.push_back( vertex0.x() );
            coords.push_back( vertex0.y() );
            coords.push_back( vertex0.z() );

            const kvs::Vec3 vertex1( this->interpolate_vertex( v2, v3 ) );
            coords.push_back( vertex1.x() );
            coords.push_back( vertex1.y() );
            coords.push_back( vertex1.z() );

            const kvs::Vec3 vertex2( this->interpolate_vertex( v4, v5 ) );
            coords.push_back( vertex2.x() );
            coords.push_back( vertex2.y() );
            coords.push_back( vertex2.z() );

            const double value0 = this->interpolate_value<T>( volume, c0, c1 );
            const double value1 = this->interpolate_value<T>( volume, c2, c3 );
            const double value2 = this->interpolate_value<T>( volume, c4, c5 );

            const kvs::UInt8 color0 =
                static_cast<kvs::UInt8>( normalize_factor * ( value0 - min_value ) );
            colors.push_back( color_map[ color0 ].r() );
            colors.push_back( color_map[ color0 ].g() );
            colors.push_back( color_map[ color0 ].b() );

            const kvs::UInt8 color1 =
                static_cast<kvs::UInt8>( normalize_factor * ( value1 - min_value ) );
            colors.push_back( color_map[ color1 ].r() );
            colors.push_back( color_map[ color1 ].g() );
            colors.push_back( color_map[ color1 ].b() );

            const kvs::UInt8 color2 =
                static_cast<kvs::UInt8>( normalize_factor * ( value2 - min_value ) );
            colors.push_back( color_map[ color2 ].r() );
            colors.push_back( color_map[ color2 ].g() );
            colors.push_back( color_map[ color2 ].b() );

            // Calculate a normal vector for the triangle polygon.
            const kvs::Vec3 normal( -( vertex2 - vertex0 ).cross( vertex1 - vertex0 ) );
            normals.push_back( normal.x() );
            normals.push_back( normal.y() );
            normals.push_back( normal.z() );
        } // end of loop-triangle
    } // end of loop-cell

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setOpacity( 255 );
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
}

/*===========================================================================*/
/**
 *  @brief  Calculate a table index.
 *  @param  x [in] x coordinate of the grid
 *  @param  y [in] y coordinate of the grid
 *  @param  z [in] z coordinate of the grid
 *  @return table index
 */
/*===========================================================================*/
size_t SlicePlane::calculate_table_index(
    const size_t x,
    const size_t y,
    const size_t z ) const
{
    size_t table_index = 0;
    if ( this->substitute_plane_equation( x  , y  , z   ) > 0.0f ) { table_index |=   1; }
    if ( this->substitute_plane_equation( x+1, y  , z   ) > 0.0f ) { table_index |=   2; }
    if ( this->substitute_plane_equation( x+1, y+1, z   ) > 0.0f ) { table_index |=   4; }
    if ( this->substitute_plane_equation( x  , y+1, z   ) > 0.0f ) { table_index |=   8; }
    if ( this->substitute_plane_equation( x  , y  , z+1 ) > 0.0f ) { table_index |=  16; }
    if ( this->substitute_plane_equation( x+1, y  , z+1 ) > 0.0f ) { table_index |=  32; }
    if ( this->substitute_plane_equation( x+1, y+1, z+1 ) > 0.0f ) { table_index |=  64; }
    if ( this->substitute_plane_equation( x  , y+1, z+1 ) > 0.0f ) { table_index |= 128; }

    return table_index;
}


/*==========================================================================*/
/**
 *  @brief  Caluclate a table index.
 *  @param  local_index [in] indices of a target cell
 *  @return table index
 */
/*==========================================================================*/
size_t SlicePlane::calculate_tetrahedra_table_index(
    const size_t* local_index ) const
{
    const kvs::Real32* const coords = BaseClass::volume()->coords().data();

    const kvs::Vector3f vertex0( coords + 3 * local_index[0] );
    const kvs::Vector3f vertex1( coords + 3 * local_index[1] );
    const kvs::Vector3f vertex2( coords + 3 * local_index[2] );
    const kvs::Vector3f vertex3( coords + 3 * local_index[3] );

    size_t table_index = 0;
    if ( this->substitute_plane_equation( vertex0 ) > 0.0 ) { table_index |= 1; }
    if ( this->substitute_plane_equation( vertex1 ) > 0.0 ) { table_index |= 2; }
    if ( this->substitute_plane_equation( vertex2 ) > 0.0 ) { table_index |= 4; }
    if ( this->substitute_plane_equation( vertex3 ) > 0.0 ) { table_index |= 8; }

    return table_index;
}

/*==========================================================================*/
/**
 *  @brief  Caluclate a table index.
 *  @param  local_index [in] indices of a target cell
 *  @return table index
 */
/*==========================================================================*/
size_t SlicePlane::calculate_hexahedra_table_index(
    const size_t* local_index ) const
{
    const kvs::Real32* const coords = BaseClass::volume()->coords().data();

    const kvs::Vector3f vertex0( coords + 3 * local_index[0] );
    const kvs::Vector3f vertex1( coords + 3 * local_index[1] );
    const kvs::Vector3f vertex2( coords + 3 * local_index[2] );
    const kvs::Vector3f vertex3( coords + 3 * local_index[3] );
    const kvs::Vector3f vertex4( coords + 3 * local_index[4] );
    const kvs::Vector3f vertex5( coords + 3 * local_index[5] );
    const kvs::Vector3f vertex6( coords + 3 * local_index[6] );
    const kvs::Vector3f vertex7( coords + 3 * local_index[7] );

    size_t table_index = 0;
    if ( this->substitute_plane_equation( vertex0 ) > 0.0 ) { table_index |=   1; }
    if ( this->substitute_plane_equation( vertex1 ) > 0.0 ) { table_index |=   2; }
    if ( this->substitute_plane_equation( vertex2 ) > 0.0 ) { table_index |=   4; }
    if ( this->substitute_plane_equation( vertex3 ) > 0.0 ) { table_index |=   8; }
    if ( this->substitute_plane_equation( vertex4 ) > 0.0 ) { table_index |=  16; }
    if ( this->substitute_plane_equation( vertex5 ) > 0.0 ) { table_index |=  32; }
    if ( this->substitute_plane_equation( vertex6 ) > 0.0 ) { table_index |=  64; }
    if ( this->substitute_plane_equation( vertex7 ) > 0.0 ) { table_index |= 128; }

    return table_index;
}

/*==========================================================================*/
/**
 *  @brief  Caluclate a table index.
 *  @param  local_index [in] indices of a target cell
 *  @return table index
 */
/*==========================================================================*/
size_t SlicePlane::calculate_pyramid_table_index(
    const size_t* local_index ) const
{
    const kvs::Real32* const coords = BaseClass::volume()->coords().data();

    const kvs::Vector3f vertex0( coords + 3 * local_index[0] );
    const kvs::Vector3f vertex1( coords + 3 * local_index[1] );
    const kvs::Vector3f vertex2( coords + 3 * local_index[2] );
    const kvs::Vector3f vertex3( coords + 3 * local_index[3] );
    const kvs::Vector3f vertex4( coords + 3 * local_index[4] );

    size_t table_index = 0;
    if ( this->substitute_plane_equation( vertex0 ) > 0.0 ) { table_index |=  1; }
    if ( this->substitute_plane_equation( vertex1 ) > 0.0 ) { table_index |=  2; }
    if ( this->substitute_plane_equation( vertex2 ) > 0.0 ) { table_index |=  4; }
    if ( this->substitute_plane_equation( vertex3 ) > 0.0 ) { table_index |=  8; }
    if ( this->substitute_plane_equation( vertex4 ) > 0.0 ) { table_index |= 16; }

    return table_index;
}


/*==========================================================================*/
/**
 *  @brief  Caluclate a table index.
 *  @param  local_index [in] indices of a target cell
 *  @return table index
 */
/*==========================================================================*/
size_t SlicePlane::calculate_prism_table_index(
    const size_t* local_index ) const
{
    const kvs::Real32* const coords = BaseClass::volume()->coords().data();

    const kvs::Vec3 vertex0( coords + 3 * local_index[0] );
    const kvs::Vec3 vertex1( coords + 3 * local_index[1] );
    const kvs::Vec3 vertex2( coords + 3 * local_index[2] );
    const kvs::Vec3 vertex3( coords + 3 * local_index[3] );
    const kvs::Vec3 vertex4( coords + 3 * local_index[4] );
    const kvs::Vec3 vertex5( coords + 3 * local_index[5] );

    size_t table_index = 0;
    if ( this->substitute_plane_equation( vertex0 ) > 0.0 ) { table_index |=  1; }
    if ( this->substitute_plane_equation( vertex1 ) > 0.0 ) { table_index |=  2; }
    if ( this->substitute_plane_equation( vertex2 ) > 0.0 ) { table_index |=  4; }
    if ( this->substitute_plane_equation( vertex3 ) > 0.0 ) { table_index |=  8; }
    if ( this->substitute_plane_equation( vertex4 ) > 0.0 ) { table_index |= 16; }
    if ( this->substitute_plane_equation( vertex5 ) > 0.0 ) { table_index |= 32; }

    return table_index;
}

/*==========================================================================*/
/**
 *  @brief  Calculate a plane equation.
 *  @param  x [in] x value of vertex
 *  @param  y [in] y value of vertex
 *  @param  z [in] z value of vertex
 *  @return value of a plane equation
 */
/*==========================================================================*/
float SlicePlane::substitute_plane_equation(
    const size_t x,
    const size_t y,
    const size_t z ) const
{
    return
        m_coefficients.x() * x +
        m_coefficients.y() * y +
        m_coefficients.z() * z +
        m_coefficients.w();
}

/*==========================================================================*/
/**
 *  @brief  Calculate a plane equation.
 *  @param  vertex [in] coordinate of vertex
 *  @return value of a plane equation
 */
/*==========================================================================*/
float SlicePlane::substitute_plane_equation(
    const kvs::Vector3f& vertex ) const
{
    return
        m_coefficients.x() * vertex.x() +
        m_coefficients.y() * vertex.y() +
        m_coefficients.z() * vertex.z() +
        m_coefficients.w();
}

/*==========================================================================*/
/**
 *  @brief  Interpolate a coordinate value of a intersected vertex.
 *  @param  vertex0 [in] vertex coordinate of the end point #0
 *  @param  vertex1 [in] vertex coordinate of the end point #1
 *  @return interpolated vertex coordinate
 */
/*==========================================================================*/
const kvs::Vector3f SlicePlane::interpolate_vertex(
    const kvs::Vector3f& vertex0,
    const kvs::Vector3f& vertex1 ) const
{
    const float value0 = this->substitute_plane_equation( vertex0 );
    const float value1 = this->substitute_plane_equation( vertex1 );
    const float ratio = kvs::Math::Abs( value0 / ( value1 - value0 ) );

    return ( 1.0f - ratio ) * vertex0 + ratio * vertex1;
}

/*==========================================================================*/
/**
 *  @brief  Interpolate a value of a intersected vertex.
 *  @param  volume [in] pointer to a structured volume
 *  @param  vertex0 [in] vertex coordinate of the end point #0
 *  @param  vertex1 [in] vertex coordinate of the end point #1
 *  @return interpolated value
 */
/*==========================================================================*/
template <typename T>
double SlicePlane::interpolate_value(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Vector3f&               vertex0,
    const kvs::Vector3f&               vertex1 ) const
{
    const T* const values = static_cast<const T*>( volume->values().data() );

    const size_t line_size  = volume->numberOfNodesPerLine();
    const size_t slice_size = volume->numberOfNodesPerSlice();

    const float value0 = this->substitute_plane_equation( vertex0 );
    const float value1 = this->substitute_plane_equation( vertex1 );
    const float ratio = kvs::Math::Abs( value0 / ( value1 - value0 ) );

    const double x0 = vertex0.x();
    const double y0 = vertex0.y();
    const double z0 = vertex0.z();
    const double x1 = vertex1.x();
    const double y1 = vertex1.y();
    const double z1 = vertex1.z();
    const size_t index0 = static_cast<size_t>( x0 + y0 * line_size + z0 * slice_size );
    const size_t index1 = static_cast<size_t>( x1 + y1 * line_size + z1 * slice_size );

    return values[ index0 ] + ratio * ( values[ index1 ] - values[ index0 ] );
}

/*==========================================================================*/
/**
 *  @brief  Interpolate a value of a intersected vertex.
 *  @param  volume [in] pointer to a unstructured volume
 *  @param  index0 [in] index of the end point #0
 *  @param  index1 [in] index of the end point #1
 *  @return interpolated value
 */
/*==========================================================================*/
template <typename T>
double SlicePlane::interpolate_value(
    const kvs::UnstructuredVolumeObject* volume,
    const size_t                         index0,
    const size_t                         index1 ) const
{
    const T* const values = static_cast<const T*>( volume->values().data() );
    const kvs::Real32* const coords = volume->coords().data();

    const float value0 = this->substitute_plane_equation( kvs::Vector3f( coords + 3 * index0 ) );
    const float value1 = this->substitute_plane_equation( kvs::Vector3f( coords + 3 * index1 ) );
    const float ratio = kvs::Math::Abs( value0 / ( value1 - value0 ) );

    return values[ index0 ] + ratio * ( values[ index1 ] - values[ index0 ] );
}

} // end of namespace kvs
