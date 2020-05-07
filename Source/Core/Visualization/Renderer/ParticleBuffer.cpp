/****************************************************************************/
/**
 *  @file   ParticleBuffer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBuffer.cpp 1642 2013-09-15 12:41:47Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ParticleBuffer.h"
#include <kvs/Type>
#include <kvs/Math>
#include <kvs/PointObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParticleBuffer class.
 */
/*===========================================================================*/
ParticleBuffer::ParticleBuffer():
    m_ref_shader( NULL ),
    m_ref_point_object( NULL )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param width [in] window width
 *  @param height [in] window height
 *  @param subpixel_level [in] subpixel level
 *  @param device_pixel_ratio [in] device pixel ratio
 */
/*==========================================================================*/
ParticleBuffer::ParticleBuffer(
    const size_t width,
    const size_t height,
    const size_t subpixel_level,
    const size_t device_pixel_ratio ):
    m_ref_shader( NULL )
{
    this->create( width, height, subpixel_level, device_pixel_ratio );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
ParticleBuffer::~ParticleBuffer()
{
    this->clear();
}

/*==========================================================================*/
/**
 *  Create.
 *  @param width [in] window width
 *  @param height [in] window height
 *  @param subpixel_level [in] subpixel level
 *  @param device_pixel_ratio [in] device pixel ratio
 *  @return true/false
 */
/*==========================================================================*/
bool ParticleBuffer::create(
    const size_t width,
    const size_t height,
    const size_t subpixel_level,
    const size_t device_pixel_ratio )
{
    const size_t npixels = width * height;

    m_width = width;
    m_height = height;
    m_subpixel_level = subpixel_level;
    m_size = npixels * 4;
    m_num_of_projected_particles = 0;
    m_num_of_stored_particles = 0;
    m_enable_shading = true;
    m_extended_width = m_width * m_subpixel_level;
    m_device_pixel_ratio = device_pixel_ratio;

    if( m_width == 0 || m_height == 0 )
    {
        kvsMessageError("Cannot create the pixel data for the particle buffer.");
        return false;
    }

    const size_t subpixeled_npixels = npixels * subpixel_level * subpixel_level;
    m_index_buffer.allocate( subpixeled_npixels );
    m_depth_buffer.allocate( subpixeled_npixels );

    m_index_buffer.fill( 0 );
    m_depth_buffer.fill( 0 );

    return true;
}

/*==========================================================================*/
/**
 *  Clean.
 */
/*==========================================================================*/
void ParticleBuffer::clean()
{
    m_index_buffer.fill( 0 );
    m_depth_buffer.fill( 0 );

    m_num_of_projected_particles = 0;
    m_num_of_stored_particles  = 0;
}

/*==========================================================================*/
/**
 *  Clear.
 */
/*==========================================================================*/
void ParticleBuffer::clear()
{
    m_index_buffer.release();
    m_depth_buffer.release();
}

/*==========================================================================*/
/**
 *  Create the rendering image.
 *  @param  color [out] pointer to the color array
 *  @param  depth [out] pointer to the depth array
 */
/*==========================================================================*/
void ParticleBuffer::createImage(
    kvs::ValueArray<kvs::UInt8>* color,
    kvs::ValueArray<kvs::Real32>* depth )
{
    if ( m_enable_shading ) this->create_image_with_shading( color, depth );
    else this->create_image_without_shading( color, depth );
}

/*===========================================================================*/
/**
 *  @brief  Creates the rendering image with shading.
 *  @param  color [in] pointer to color data
 *  @param  depth [in] pointer to depth data
 */
/*===========================================================================*/
void ParticleBuffer::create_image_with_shading(
    kvs::ValueArray<kvs::UInt8>* color,
    kvs::ValueArray<kvs::Real32>* depth )
{
    const kvs::Real32* point_coords = m_ref_point_object->coords().data();
    const kvs::UInt8* point_color = m_ref_point_object->colors().data();
    const kvs::Real32* point_normal = m_ref_point_object->normals().data();

    const float inv_ssize = 1.0f / ( m_subpixel_level * m_subpixel_level );
    const float normalize_alpha = 255.0f * inv_ssize;

    size_t pindex = 0;
    size_t pindex4 = 0;
    const size_t bw = m_extended_width;
    const size_t dpr = m_device_pixel_ratio;
    for ( size_t py = 0; py < m_height * dpr; py++ )
    {
        const size_t by_start = ( py / dpr ) * m_subpixel_level;
        for ( size_t px = 0; px < m_width * dpr; px++, pindex++, pindex4 += 4 )
        {
            const size_t bx_start = ( px / dpr ) * m_subpixel_level;
            float R = 0.0f;
            float G = 0.0f;
            float B = 0.0f;
            float D = 0.0f;
            size_t npoints = 0;
            for ( size_t by = by_start; by < by_start + m_subpixel_level; by++ )
            {
                const size_t bindex_start = bw * by;
                for ( size_t bx = bx_start; bx < bx_start + m_subpixel_level; bx++ )
                {
                    const size_t bindex = bindex_start + bx;
                    if ( m_depth_buffer[bindex] > 0.0f )
                    {
                        const size_t point_index3 = 3 * m_index_buffer[ bindex ];
                        const kvs::Vec3 vertex( point_coords + point_index3 );
                        const kvs::Vec3 normal( point_normal + point_index3 );
                        kvs::RGBColor color( point_color + point_index3 );
                        color = m_ref_shader->shadedColor( color, vertex, normal );
                        R += color.r();
                        G += color.g();
                        B += color.b();
                        D = kvs::Math::Max( D, m_depth_buffer[ bindex ] );
                        npoints++;
                    }
                }
            }

            R *= inv_ssize;
            G *= inv_ssize;
            B *= inv_ssize;

            (*color)[ pindex4 + 0 ] = static_cast<kvs::UInt8>( kvs::Math::Min( R, 255.0f ) + 0.5f );
            (*color)[ pindex4 + 1 ] = static_cast<kvs::UInt8>( kvs::Math::Min( G, 255.0f ) + 0.5f );
            (*color)[ pindex4 + 2 ] = static_cast<kvs::UInt8>( kvs::Math::Min( B, 255.0f ) + 0.5f );
            (*color)[ pindex4 + 3 ] = static_cast<kvs::UInt8>( npoints * normalize_alpha );
            (*depth)[ pindex ] = ( npoints == 0 ) ? 1.0f : D;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates the rendering image without shading.
 *  @param  color [in] pointer to color data
 *  @param  depth [in] pointer to depth data
 */
/*===========================================================================*/
void ParticleBuffer::create_image_without_shading(
    kvs::ValueArray<kvs::UInt8>* color,
    kvs::ValueArray<kvs::Real32>* depth )
{
    const kvs::UInt8* point_color = m_ref_point_object->colors().data();

    const float inv_ssize = 1.0f / ( m_subpixel_level * m_subpixel_level );
    const float normalize_alpha = 255.0f * inv_ssize;

    size_t pindex = 0;
    size_t pindex4 = 0;
    const size_t bw = m_extended_width;
    const size_t dpr = m_device_pixel_ratio;
    for ( size_t py = 0; py < m_height * dpr; py++ )
    {
        const size_t by_start = ( py / dpr ) * m_subpixel_level;
        for ( size_t px = 0; px < m_width * dpr; px++, pindex++, pindex4 += 4 )
        {
            const size_t bx_start = ( px / dpr ) * m_subpixel_level;
            float R = 0.0f;
            float G = 0.0f;
            float B = 0.0f;
            float D = 0.0f;
            size_t npoints = 0;
            for ( size_t by = by_start; by < by_start + m_subpixel_level; by++ )
            {
                const size_t bindex_start = bw * by;
                for ( size_t bx = bx_start; bx < bx_start + m_subpixel_level; bx++ )
                {
                    const size_t bindex = bindex_start + bx;
                    if ( m_depth_buffer[bindex] > 0.0f )
                    {
                        const size_t point_index3 = 3 * m_index_buffer[ bindex ];
                        R += point_color[ point_index3 + 0 ];
                        G += point_color[ point_index3 + 1 ];
                        B += point_color[ point_index3 + 2 ];
                        D = kvs::Math::Max( D, m_depth_buffer[ bindex ] );
                        npoints++;
                    }
                }
            }

            R *= inv_ssize;
            G *= inv_ssize;
            B *= inv_ssize;

            (*color)[ pindex4 + 0 ] = static_cast<kvs::UInt8>(R);
            (*color)[ pindex4 + 1 ] = static_cast<kvs::UInt8>(G);
            (*color)[ pindex4 + 2 ] = static_cast<kvs::UInt8>(B);
            (*color)[ pindex4 + 3 ] = static_cast<kvs::UInt8>( npoints * normalize_alpha );
            (*depth)[ pindex ] = ( npoints == 0 ) ? 1.0f : D;
        }
    }
}

} // end of namesapce kvs
