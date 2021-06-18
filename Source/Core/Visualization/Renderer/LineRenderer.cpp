/****************************************************************************/
/**
 *  @file   LineRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "LineRenderer.h"
#include "LineRenderingFunction.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/LineObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*==========================================================================*/
void LineRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    auto* line = kvs::LineObject::DownCast( object );

    this->initialize();
    ::LineRenderingFunction( line, camera->devicePixelRatio() );

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Sets depth offset.
 *  @param  factor [in] scale factor
 *  @param  units [in] constant depth offset
 */
/*===========================================================================*/
void LineRenderer::setDepthOffset( const float factor, const float units )
{
    m_depth_offset = kvs::Vec2( factor, units );
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing option.
 *  @param  enable [in] if true, anti-aliasing is enabled
 *  @param  multisample [in] if true, multi-sample anti-aliasing is enabled
 */
/*===========================================================================*/
void LineRenderer::setAntiAliasingEnabled( const bool enable, const bool multisample ) const
{
    m_enable_anti_aliasing = enable;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing option.
 *  @param  multisample [in] if true, multi-sample anti-aliasing is enabled
 */
/*===========================================================================*/
void LineRenderer::enableAntiAliasing( const bool multisample ) const
{
    this->setAntiAliasingEnabled( true, multisample );
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing option.
 */
/*===========================================================================*/
void LineRenderer::disableAntiAliasing() const
{
    this->setAntiAliasingEnabled( false, false );
}

/*===========================================================================*/
/**
 *  @brief  Initializes the OpenGL properties.
 */
/*===========================================================================*/
void LineRenderer::initialize()
{
    // Lighting.
    if ( !BaseClass::isShadingEnabled() ) { kvs::OpenGL::Disable( GL_LIGHTING ); }
    else { kvs::OpenGL::Enable( GL_LIGHTING ); }

    // Depth offset
    if ( !kvs::Math::IsZero( m_depth_offset[0] ) )
    {
        kvs::OpenGL::SetPolygonOffset( m_depth_offset[0], m_depth_offset[1] );
        kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );
    }

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
#if defined ( GL_MULTISAMPLE )
        if ( m_enable_multisample_anti_aliasing )
        {
            GLint buffers = 0;
            GLint samples = 0;
            kvs::OpenGL::GetIntegerv( GL_SAMPLE_BUFFERS, &buffers );
            kvs::OpenGL::GetIntegerv( GL_SAMPLES, &samples );
            if ( buffers > 0 && samples > 1 ) kvs::OpenGL::Enable( GL_MULTISAMPLE );
        }
        else
#endif
        {
            kvs::OpenGL::Enable( GL_LINE_SMOOTH );
            kvs::OpenGL::Enable( GL_BLEND );
            kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            kvs::OpenGL::Hint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        }
    }

    kvs::OpenGL::SetColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
}

} // end of namespace kvs
