/*****************************************************************************/
/**
 *  @file   GlyphBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GlyphBase.cpp 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "GlyphBase.h"
#include <kvs/OpenGL>
#include <kvs/Quaternion>
#include <kvs/StructuredVolumeObject>


namespace
{
const kvs::Vec3 DefaultDirection = kvs::Vec3( 0.0, 1.0, 0.0 );
const kvs::Real32 DefaultSize = 1.0f;
const kvs::RGBColor DefaultColor = kvs::RGBColor( 255, 255, 255 );
const kvs::UInt8 DefaultOpacity = 255;
};


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new GlyphBase class.
 */
/*===========================================================================*/
GlyphBase::GlyphBase():
    m_size_mode( GlyphBase::SizeByDefault ),
    m_direction_mode( GlyphBase::DirectionByDefault ),
    m_color_mode( GlyphBase::ColorByMagnitude ),
    m_opacity_mode( GlyphBase::OpacityByDefault ),
    m_scale( 1.0f, 1.0f, 1.0f ),
    m_tfunc()
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the GlyphBase class.
 */
/*===========================================================================*/
GlyphBase::~GlyphBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Transform the glyph object.
 *  @param  position [in] object position
 *  @param  size [in] object size
 */
/*===========================================================================*/
void GlyphBase::transform( const kvs::Vector3f& position, const kvs::Real32 size )
{
    kvs::OpenGL::Translate( position.x(), position.y(), position.z() );
    kvs::OpenGL::Scale( m_scale.x(), m_scale.y(), m_scale.z() );
    kvs::OpenGL::Scale( size, size, size );
}

/*===========================================================================*/
/**
 *  @brief  Transform the glyph object.
 *  @param  position [in] object position
 *  @param  direction [in] object direction
 *  @param  size [in] object size
 */
/*===========================================================================*/
void GlyphBase::transform(
    const kvs::Vec3& position,
    const kvs::Vec3& direction,
    const kvs::Real32 size )
{
    const kvs::Vec3 v = direction.normalized();
    const kvs::Vec3 c = ::DefaultDirection.cross( v );
    const float d = ::DefaultDirection.dot( v );
    const float s = static_cast<float>( std::sqrt( ( 1.0 + d ) * 2.0 ) );
    const kvs::Quaternion q( c.x()/s, c.y()/s, c.z()/s, s/2.0f );
    const kvs::Mat3 rot = q.toMatrix();
    const kvs::Xform xform( position, m_scale * size, rot );

    float array[16];
    xform.toArray( array );
    kvs::OpenGL::MultMatrix( array );
}

/*===========================================================================*/
/**
 *  @brief  Calculates the coordinate value array.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void GlyphBase::calculateCoords( const kvs::VolumeObjectBase* volume )
{
    const kvs::VolumeObjectBase::VolumeType type = volume->volumeType();
    if ( type == kvs::VolumeObjectBase::Structured )
    {
        this->calculateCoords( kvs::StructuredVolumeObject::DownCast( volume ) );
    }
    else // type == kvs::VolumeObjectBase::Unstructured
    {
        this->setCoords( volume->coords() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates the coordinate value array of the structured volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
void GlyphBase::calculateCoords( const kvs::StructuredVolumeObject* volume )
{
    const kvs::StructuredVolumeObject::GridType type = volume->gridType();
    if ( type == kvs::StructuredVolumeObject::Uniform )
    {
       this->calculateUniformCoords( volume );
    }
    else if ( type == kvs::StructuredVolumeObject::Rectilinear )
    {
        this->calculateRectilinearCoords( volume );
    }
    else
    {
        this->setCoords( volume->coords() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates the coordinate value array of the uniform type volume object.
 *  @param  volume [in] pointer to the structrued volume object
 */
/*===========================================================================*/
void GlyphBase::calculateUniformCoords( const kvs::StructuredVolumeObject* volume )
{
    kvs::ValueArray<kvs::Real32> coords( 3 * volume->numberOfNodes() );
    kvs::Real32* coord = coords.data();

    const kvs::Vec3ui resolution( volume->resolution() );
    const kvs::Vec3 volume_size( volume->maxExternalCoord() - volume->minExternalCoord() );
    const kvs::Vec3ui ngrids( resolution - kvs::Vec3ui( 1, 1, 1 ) );
    const kvs::Vec3 grid_size(
        ( ngrids.x() == 0 ) ? 0.0f : volume_size.x() / static_cast<float>( ngrids.x() ),
        ( ngrids.y() == 0 ) ? 0.0f : volume_size.y() / static_cast<float>( ngrids.y() ),
        ( ngrids.z() == 0 ) ? 0.0f : volume_size.z() / static_cast<float>( ngrids.z() ) );

    for ( size_t k = 0; k < resolution.z(); ++k )
    {
        const float z = grid_size.z() * static_cast<float>( k );
        for ( size_t j = 0; j < resolution.y(); ++j )
        {
            const float y = grid_size.y() * static_cast<float>( j );
            for ( size_t i = 0; i < resolution.x(); ++i )
            {
                const float x = grid_size.x() * static_cast<float>( i );
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
            }
        }
    }

    this->setCoords( coords );
}

/*===========================================================================*/
/**
 *  @brief  Calculates the coordinate value array of the rectilinear type volume object.
 *  @param  volume [in] pointer to the structrued volume object
 */
/*===========================================================================*/
void GlyphBase::calculateRectilinearCoords( const kvs::StructuredVolumeObject* volume )
{
    kvs::ValueArray<kvs::Real32> coords( 3 * volume->numberOfNodes() );
    kvs::Real32* coord = coords.data();

    const size_t dimx = volume->resolution().x();
    const size_t dimy = volume->resolution().y();
    const size_t dimz = volume->resolution().z();

    const kvs::Real32* xcoord = volume->coords().data();
    const kvs::Real32* ycoord = xcoord + dimx;
    const kvs::Real32* zcoord = ycoord + dimy;

    for ( size_t k = 0; k < dimz; ++k )
    {
        const float z = zcoord[k];
        for ( size_t j = 0; j < dimy; ++j )
        {
            const float y = ycoord[j];
            for ( size_t i = 0; i < dimx; ++i )
            {
                const float x = xcoord[i];
                *( coord++ ) = x;
                *( coord++ ) = y;
                *( coord++ ) = z;
            }
        }
    }

    this->setCoords( coords );
}

/*===========================================================================*/
/**
 *  @brief  Calculates the size value array.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
void GlyphBase::calculateSizes( const kvs::VolumeObjectBase* volume )
{
    const T* value = reinterpret_cast<const T*>( volume->values().data() );
    const size_t veclen = volume->veclen();
    const size_t nnodes = volume->numberOfNodes();

    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real32 min_value = static_cast<kvs::Real32>(volume->minValue());
    const kvs::Real32 max_value = static_cast<kvs::Real32>(volume->maxValue());
    const kvs::Real32 normalize = 1.0f / ( max_value - min_value );

    kvs::ValueArray<kvs::Real32> sizes( nnodes );
    kvs::Real32* size = sizes.data();

    switch( m_size_mode )
    {
    case GlyphBase::SizeByDefault:
        for( size_t i = 0; i < nnodes; i++ ) size[i] = ::DefaultSize;
        break;
    case GlyphBase::SizeByMagnitude:
        if ( veclen == 1 )
        {
            for( size_t i = 0; i < nnodes; i++ )
            {
                size[i] = ::DefaultSize * normalize * ( static_cast<float>(value[i]) - min_value );
            }
        }
        else if ( veclen == 3 )
        {
            for( size_t i = 0, index = 0; i < nnodes; i++, index += 3 )
            {
                const kvs::Vector3f v(
                    static_cast<float>(value[index]),
                    static_cast<float>(value[index+1]),
                    static_cast<float>(value[index+2]));
                size[i] = ::DefaultSize * normalize * ( static_cast<float>(v.length()) - min_value );
            }
        }
        break;
    default:
        break;
    }

    this->setSizes( sizes );
}

template void GlyphBase::calculateSizes<kvs::Int8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::Int16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::Int32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::Int64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::UInt8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::UInt16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::UInt32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::UInt64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::Real32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateSizes<kvs::Real64>( const kvs::VolumeObjectBase* volume );

/*===========================================================================*/
/**
 *  @brief  Calculates the direction vector array.
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
template <typename T>
void GlyphBase::calculateDirections( const kvs::VolumeObjectBase* volume )
{
    const T* value = reinterpret_cast<const T*>( volume->values().data() );
    const size_t veclen = volume->veclen();
    const size_t nnodes = volume->numberOfNodes();
    if ( veclen == 3 )
    {
        kvs::ValueArray<kvs::Real32> directions( nnodes * veclen );
        kvs::Real32* direction = directions.data();

        for ( size_t i = 0; i < directions.size(); i++ )
        {
            direction[i] = static_cast<kvs::Real32>( value[i] );
        }

        this->setDirections( directions );
    }
}

template void GlyphBase::calculateDirections<kvs::Int8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::Int16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::Int32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::Int64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::UInt8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::UInt16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::UInt32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::UInt64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::Real32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateDirections<kvs::Real64>( const kvs::VolumeObjectBase* volume );

/*===========================================================================*/
/**
 *  @brief  Calculates the color value array.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
void GlyphBase::calculateColors( const kvs::VolumeObjectBase* volume )
{
    const T* value = reinterpret_cast<const T*>( volume->values().data() );
    const size_t veclen = volume->veclen();
    const size_t nnodes = volume->numberOfNodes();

    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real32 min_value = static_cast<kvs::Real32>(volume->minValue());
    const kvs::Real32 max_value = static_cast<kvs::Real32>(volume->maxValue());
    const kvs::Real32 normalize = 1.0f / ( max_value - min_value );

    kvs::ValueArray<kvs::UInt8> colors( 3 * nnodes );
    kvs::UInt8* color = colors.data();

    switch( m_color_mode )
    {
    case GlyphBase::ColorByDefault:
        for ( size_t i = 0; i < nnodes; i++ )
        {
            *( color++ ) = ::DefaultColor.r();
            *( color++ ) = ::DefaultColor.r();
            *( color++ ) = ::DefaultColor.r();
        }
        break;
    case GlyphBase::ColorByMagnitude:
    {
        const kvs::ColorMap color_map( m_tfunc.colorMap() );
        if ( veclen == 1 )
        {
            for ( size_t i = 0; i < nnodes; i++ )
            {
                const float d = normalize * ( static_cast<float>(value[i]) - min_value );
                const size_t level = static_cast<size_t>( 255.0f * d );
                const kvs::RGBColor c = color_map[ level ];
                *( color++ ) = c.r();
                *( color++ ) = c.g();
                *( color++ ) = c.b();
            }
        }
        else if ( veclen == 3 )
        {
            for ( size_t i = 0, index = 0; i < nnodes; i++, index += 3 )
            {
                const kvs::Vector3f v(
                    static_cast<float>(value[index]),
                    static_cast<float>(value[index+1]),
                    static_cast<float>(value[index+2]));
                const float d = normalize * ( static_cast<float>(v.length()) - min_value );
                const size_t level = static_cast<size_t>( 255.0f * d );
                const kvs::RGBColor c = color_map[ level ];
                *( color++ ) = c.r();
                *( color++ ) = c.g();
                *( color++ ) = c.b();
            }
        }
        break;
    }
    default:
        break;
    }

    this->setColors( colors );
}

template void GlyphBase::calculateColors<kvs::Int8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::Int16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::Int32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::Int64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::UInt8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::UInt16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::UInt32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::UInt64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::Real32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateColors<kvs::Real64>( const kvs::VolumeObjectBase* volume );

/*===========================================================================*/
/**
 *  @brief  Calculates the opacity value array.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
void GlyphBase::calculateOpacities( const kvs::VolumeObjectBase* volume )
{
    const T* value = reinterpret_cast<const T*>( volume->values().data() );
    const size_t veclen = volume->veclen();
    const size_t nnodes = volume->numberOfNodes();

    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    const kvs::Real32 min_value = static_cast<kvs::Real32>(volume->minValue());
    const kvs::Real32 max_value = static_cast<kvs::Real32>(volume->maxValue());
    const kvs::Real32 normalize = 255.0f / ( max_value - min_value );

    kvs::ValueArray<kvs::UInt8> opacities( nnodes );
    kvs::UInt8* opacity = opacities.data();

    switch( m_opacity_mode )
    {
    case GlyphBase::OpacityByDefault:
        for( size_t i = 0; i < nnodes; i++ ) opacity[i] = ::DefaultOpacity;
        break;
    case GlyphBase::OpacityByMagnitude:
        if ( veclen == 1 )
        {
            for( size_t i = 0; i < nnodes; i++ )
            {
                opacity[i] = static_cast<kvs::UInt8>( normalize * ( static_cast<float>(value[i]) - min_value ) );
            }
        }
        else if ( veclen == 3 )
        {
            for( size_t i = 0, index = 0; i < nnodes; i++, index += 3 )
            {
                const kvs::Vector3f v(
                    static_cast<float>(value[index]),
                    static_cast<float>(value[index+1]),
                    static_cast<float>(value[index+2]));
                opacity[i] = static_cast<kvs::UInt8>( normalize * ( v.length() - min_value ) );
            }
        }
        break;
    default:
        break;
    }

    this->setOpacities( opacities );
}

template void GlyphBase::calculateOpacities<kvs::Int8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::Int16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::Int32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::Int64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::UInt8>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::UInt16>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::UInt32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::UInt64>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::Real32>( const kvs::VolumeObjectBase* volume );
template void GlyphBase::calculateOpacities<kvs::Real64>( const kvs::VolumeObjectBase* volume );

} // end of namespace kvs
