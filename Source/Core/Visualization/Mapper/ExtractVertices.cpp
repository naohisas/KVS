/****************************************************************************/
/**
 *  @file   ExtractVertices.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractVertices.cpp 1761 2014-05-07 04:18:15Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ExtractVertices.h"
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new empty ExtractVertices.
 */
/*==========================================================================*/
ExtractVertices::ExtractVertices():
    MapperBase(),
    PointObject()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ExtractVertices.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
ExtractVertices::ExtractVertices(
    const kvs::VolumeObjectBase* volume ):
    MapperBase(),
    PointObject()
{
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ExtractVertices.
 *  @param  volume [in] pointer to the structured volume object
 *  @param  transfer_function [in] transfer function
 */
/*==========================================================================*/
ExtractVertices::ExtractVertices(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    MapperBase( transfer_function ),
    PointObject()
{
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  Destroys the ExtractVertices.
 */
/*==========================================================================*/
ExtractVertices::~ExtractVertices()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return pointer to the point object
 */
/*===========================================================================*/
ExtractVertices::SuperClass* ExtractVertices::exec( const kvs::ObjectBase* object )
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

/*===========================================================================*/
/**
 *  @brief  Mapping for the volume object.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void ExtractVertices::mapping(
    const kvs::VolumeObjectBase* volume )
{
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    this->calculate_coords();

    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) { this->calculate_colors<kvs::Int8  >(); }
    else if ( type == typeid( kvs::Int16  ) ) { this->calculate_colors<kvs::Int16 >(); }
    else if ( type == typeid( kvs::Int32  ) ) { this->calculate_colors<kvs::Int32 >(); }
    else if ( type == typeid( kvs::Int64  ) ) { this->calculate_colors<kvs::Int64 >(); }
    else if ( type == typeid( kvs::UInt8  ) ) { this->calculate_colors<kvs::UInt8 >(); }
    else if ( type == typeid( kvs::UInt16 ) ) { this->calculate_colors<kvs::UInt16>(); }
    else if ( type == typeid( kvs::UInt32 ) ) { this->calculate_colors<kvs::UInt32>(); }
    else if ( type == typeid( kvs::UInt64 ) ) { this->calculate_colors<kvs::UInt64>(); }
    else if ( type == typeid( kvs::Real32 ) ) { this->calculate_colors<kvs::Real32>(); }
    else if ( type == typeid( kvs::Real64 ) ) { this->calculate_colors<kvs::Real64>(); }
}

/*===========================================================================*/
/**
 *  @brief  Calculates the coordinate values.
 */
/*===========================================================================*/
void ExtractVertices::calculate_coords()
{
    KVS_ASSERT( BaseClass::volume()->volumeType() == VolumeObjectBase::Structured );
    typedef kvs::StructuredVolumeObject Volume;
    const Volume* volume = Volume::DownCast( BaseClass::volume() );

    const StructuredVolumeObject::GridType& type = volume->gridType();
    if ( type == StructuredVolumeObject::Uniform )
    {
       this->calculate_uniform_coords();
    }
    else if ( type == StructuredVolumeObject::Rectilinear )
    {
        this->calculate_rectiliner_coords();
    }
    else
    {
        SuperClass::setCoords( BaseClass::volume()->coords() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates the coordinate values of the uniform grid.
 */
/*===========================================================================*/
void ExtractVertices::calculate_uniform_coords()
{
    const kvs::StructuredVolumeObject* volume
        = dynamic_cast<const kvs::StructuredVolumeObject*>( BaseClass::volume() );

    kvs::ValueArray<float> coords( 3 * volume->numberOfNodes() );
    float*                 coord = coords.data();

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
 *  @brief  Calculates the coordinate values of the rectilinear grid.
 */
/*===========================================================================*/
void ExtractVertices::calculate_rectiliner_coords()
{
    BaseClass::setSuccess( false );
    kvsMessageError("Rectilinear volume has not yet supportted.");
}

/*===========================================================================*/
/**
 *  @brief  Calculates the color values.
 */
/*===========================================================================*/
template <typename T>
void ExtractVertices::calculate_colors()
{
    const kvs::VolumeObjectBase* volume = BaseClass::volume();

    const T*       value = reinterpret_cast<const T*>( volume->values().data() );
    const T* const end   = value + volume->values().size();

    kvs::ValueArray<kvs::UInt8> colors( 3 * volume->numberOfNodes() );
    kvs::UInt8*                 color = colors.data();

    kvs::ColorMap cmap( BaseClass::colorMap() );

    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }

    const kvs::Real64 min_value = volume->minValue();
    const kvs::Real64 max_value = volume->maxValue();

    const kvs::Real64 normalize_factor =
        static_cast<kvs::Real64>( cmap.resolution() - 1 ) / ( max_value - min_value );

    const size_t veclen = BaseClass::volume()->veclen();

    if ( veclen == 1 )
    {
        while( value < end )
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
