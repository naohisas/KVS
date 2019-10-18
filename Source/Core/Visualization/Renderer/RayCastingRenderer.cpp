/****************************************************************************/
/**
 *  @file   RayCastingRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RayCastingRenderer.cpp 1803 2014-08-08 05:51:05Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "RayCastingRenderer.h"
#include <cstring>
#include <kvs/Math>
#include <kvs/Type>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/TrilinearInterpolator>
#include <kvs/VolumeRayIntersector>
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new RayCastingRenderer class.
 */
/*===========================================================================*/
RayCastingRenderer::RayCastingRenderer():
    m_step( 0.5f ),
    m_opaque( 0.97f ),
    m_ray_width( 1 ),
    m_enable_lod( false )
{
    BaseClass::setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new RayCastingRenderer class.
 *  @param  trunc [in] transfer function
 */
/*===========================================================================*/
RayCastingRenderer::RayCastingRenderer( const kvs::TransferFunction& tfunc ):
    m_step( 0.5f ),
    m_opaque( 0.97f ),
    m_ray_width( 1 ),
    m_enable_lod( false )
{
    BaseClass::setTransferFunction( tfunc );
    BaseClass::setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new RayCastingRenderer class.
 *  @param  shader [in] shading model
 */
/*===========================================================================*/
template <typename ShadingType>
RayCastingRenderer::RayCastingRenderer( const ShadingType shader ):
    m_step( 0.5f ),
    m_opaque( 0.97f ),
    m_ray_width( 1 ),
    m_enable_lod( false )
{
    BaseClass::setShader( shader );
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void RayCastingRenderer::exec(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( object );
    BaseClass::startTimer();

    // Screen size changed.
    if ( BaseClass::windowWidth() != camera->windowWidth() ||
         BaseClass::windowHeight() != camera->windowHeight() )
    {
        BaseClass::setWindowSize( camera->windowWidth(), camera->windowHeight() );
        BaseClass::setDevicePixelRatio( camera->devicePixelRatio() );
        const int framebuffer_width = BaseClass::framebufferWidth();
        const int framebuffer_height = BaseClass::framebufferHeight();
        const size_t npixels = framebuffer_width * framebuffer_height;
        BaseClass::allocateColorData( npixels * 4 );
        BaseClass::allocateDepthData( npixels );
        kvs::OpenGL::GetModelViewMatrix( m_modelview );
    }

    // Initialize frame buffer.
    BaseClass::fillColorData( 0 );
    BaseClass::fillDepthData( 0 );

    // Rasterize.
    if ( !volume->hasMinMaxValues() ) volume->updateMinMaxValues();
    const float min_value = static_cast<float>( volume->minValue() );
    const float max_value = static_cast<float>( volume->maxValue() );
    const std::type_info& type = volume->values().typeInfo()->type();
    if(      type == typeid(kvs::UInt8)  )
    {
        if ( !BaseClass::transferFunction().hasRange() ) BaseClass::transferFunction().setRange( 0, 255 );
        this->rasterize<kvs::UInt8>( volume, camera, light );
    }
    else if( type == typeid(kvs::UInt16) )
    {
        if ( !BaseClass::transferFunction().hasRange() ) BaseClass::transferFunction().setRange( min_value, max_value );
        this->rasterize<kvs::UInt16>( volume, camera, light );
    }
    else if( type == typeid(kvs::Int16) )
    {
        if ( !BaseClass::transferFunction().hasRange() ) BaseClass::transferFunction().setRange( min_value, max_value );
        this->rasterize<kvs::Int16>( volume, camera, light );
    }
    else if( type == typeid(kvs::Real32) )
    {
        if ( !BaseClass::transferFunction().hasRange() ) BaseClass::transferFunction().setRange( min_value, max_value );
        this->rasterize<kvs::Real32>( volume, camera, light );
    }
    else if( type == typeid(kvs::Real64) )
    {
        if ( !BaseClass::transferFunction().hasRange() ) BaseClass::transferFunction().setRange( min_value, max_value );
        this->rasterize<kvs::Real64>( volume, camera, light );
    }
    else
    {
        kvsMessageError( "Not supported data type '%s'.",
                         volume->values().typeInfo()->typeName() );
    }

    // Draw the image.
    BaseClass::drawImage();

    BaseClass::stopTimer();
}

/*==========================================================================*/
/**
 *  @brief  Rasterization.
 *  @param  volume [in] pointer to the volume object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*==========================================================================*/
template <typename T>
void RayCastingRenderer::rasterize(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Camera* camera,
    const kvs::Light* light )
{
    // Set shader initial parameters.
    BaseClass::shader().set( camera, light, volume );

    // Readback pixels.
    BaseClass::readImage();
    kvs::UInt8* const pixel_data = BaseClass::colorData().data();
    kvs::Real32* const depth_data = BaseClass::depthData().data();

    // LOD control.
    size_t ray_width = 1;
    if ( m_enable_lod )
    {
        float modelview[16];
        kvs::OpenGL::GetModelViewMatrix( modelview );
        for ( size_t i = 0; i < 16; i++ )
        {
            if ( m_modelview[i] != modelview[i] )
            {
                ray_width = m_ray_width * BaseClass::devicePixelRatio();
                break;
            }
        }
        memcpy( m_modelview, modelview, sizeof( modelview ) );
    }

    // Set the trilinear interpolator.
    kvs::TrilinearInterpolator interpolator( volume );

    // Calculate the ray in the object coordinate system.
    float modelview[16]; kvs::OpenGL::GetModelViewMatrix( static_cast<GLfloat*>( modelview ) );
    float projection[16]; kvs::OpenGL::GetProjectionMatrix( static_cast<GLfloat*>( projection ) );
    int viewport[4]; kvs::OpenGL::GetViewport( static_cast<GLint*>( viewport ) );
    kvs::VolumeRayIntersector ray( volume, modelview, projection, viewport );

    // Execute ray casting.
    const size_t width = BaseClass::framebufferWidth();
    const size_t height = BaseClass::framebufferHeight();
    const kvs::Shader::ShadingModel& shader = BaseClass::shader();
    const kvs::ColorMap& cmap = BaseClass::transferFunction().colorMap();
    const kvs::OpacityMap& omap = BaseClass::transferFunction().opacityMap();
    const float step = m_step;
    const float opaque = m_opaque;
    size_t depth_index = 0;
    size_t pixel_index = 0;
    for ( size_t y = 0; y < height; y += ray_width )
    {
        const size_t offset = y * width;
        for ( size_t x = 0; x < width; x += ray_width, depth_index = offset + x, pixel_index = depth_index * 4 )
        {
            ray.setOrigin( x, y );

            // Intersection the ray with the bounding box.
            if ( ray.isIntersected() )
            {
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;
                float a = 0.0;

                const float depth0 = depth_data[ depth_index ];
                depth_data[ depth_index ] = ray.depth();

                do
                {
                    // Interpolation.
                    interpolator.attachPoint( ray.point() );

                    // Classification.
                    const float s = interpolator.template scalar<T>();
                    const float opacity = omap.at(s);
                    if ( !kvs::Math::IsZero( opacity ) )
                    {
                        // Shading.
                        const kvs::Vec3 vertex = ray.point();
                        const kvs::Vec3 normal = interpolator.template gradient<T>();
                        const kvs::RGBColor color = shader.shadedColor( cmap.at(s), vertex, normal );

                        // Front-to-back accumulation.
                        const float current_alpha = ( 1.0f - a ) * opacity;
                        r += current_alpha * color.r();
                        g += current_alpha * color.g();
                        b += current_alpha * color.b();
                        a += current_alpha;
                        if ( a > opaque )
                        {
                            a = 1.0f;
                            break;
                        }
                    }

                    const float depth = ray.depth();
                    if ( depth > depth0 )
                    {
                        const float current_alpha = 1.0f - a;
                        r += current_alpha * pixel_data[ pixel_index ];
                        g += current_alpha * pixel_data[ pixel_index + 1 ];
                        b += current_alpha * pixel_data[ pixel_index + 2 ];
                        a = 1.0f;
                        break;
                    }

                    ray.step( step );
                } while ( ray.isInside() );

                // Set pixel value.
                pixel_data[ pixel_index + 0 ] = static_cast<kvs::UInt8>( kvs::Math::Min( r, 255.0f ) + 0.5f );
                pixel_data[ pixel_index + 1 ] = static_cast<kvs::UInt8>( kvs::Math::Min( g, 255.0f ) + 0.5f );
                pixel_data[ pixel_index + 2 ] = static_cast<kvs::UInt8>( kvs::Math::Min( b, 255.0f ) + 0.5f );
                pixel_data[ pixel_index + 3 ] = static_cast<kvs::UInt8>( kvs::Math::Round( a * 255.0f ) );
            }
            else
            {
                depth_data[ depth_index ] = 1.0;
            }
        }
    }

    // Mosaicing by using ray_width x ray_width mask.
    if ( ray_width > 1 )
    {
        pixel_index = 0;
        depth_index = 0;
        for ( size_t y = 0; y < height; y += ray_width )
        {
            // Shift the y position of the mask by -ray_width/2.
            const size_t Y = kvs::Math::Max( int( y - ray_width / 2 ), 0 );

            const size_t offset = y * width;
            for ( size_t x = 0; x < width; x += ray_width, depth_index = offset + x, pixel_index = depth_index * 4 )
            {
                // Shift the x position of the mask by -ray_width/2.
                const size_t X = kvs::Math::Max( int( x - ray_width / 2 ), 0 );

                const kvs::UInt8  r = pixel_data[ pixel_index ];
                const kvs::UInt8  g = pixel_data[ pixel_index + 1 ];
                const kvs::UInt8  b = pixel_data[ pixel_index + 2 ];
                const kvs::UInt8  a = pixel_data[ pixel_index + 3 ];
                const kvs::Real32 d = depth_data[ depth_index ];
                for ( size_t j = 0; j < ray_width && Y + j < height; j++ )
                {
                    const size_t J = Y + j;
                    for ( size_t i = 0; i < ray_width && X + i < width; i++ )
                    {
                        const size_t I = X + i;
                        const size_t index = J * width + I;
                        const size_t index4 = index * 4;
                        pixel_data[ index4 ] = r;
                        pixel_data[ index4 + 1 ] = g;
                        pixel_data[ index4 + 2 ] = b;
                        pixel_data[ index4 + 3 ] = a;
                        depth_data[ index ] = d;
                    }
                }
            }
        }
    }

    kvs::OpenGL::Finish();
}

template
void RayCastingRenderer::rasterize<kvs::UInt8>(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Camera* camera,
    const kvs::Light* light );

template
void RayCastingRenderer::rasterize<kvs::UInt16>(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Camera* camera,
    const kvs::Light* light );

template
void RayCastingRenderer::rasterize<kvs::Int16>(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Camera* camera,
    const kvs::Light* light );

template
void RayCastingRenderer::rasterize<kvs::Real32>(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Camera* camera,
    const kvs::Light* light );

template
void RayCastingRenderer::rasterize<kvs::Real64>(
    const kvs::StructuredVolumeObject* volume,
    const kvs::Camera* camera,
    const kvs::Light* light );

} // end of namespace kvs
