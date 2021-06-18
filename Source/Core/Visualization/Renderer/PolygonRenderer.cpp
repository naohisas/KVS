/****************************************************************************/
/**
 *  @file   PolygonRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "PolygonRenderer.h"
#include "PolygonRenderingFunction.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/PolygonObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  Polygon rendering method.
 *  @param object [in] pointer to the object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void PolygonRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    auto* polygon = kvs::PolygonObject::DownCast( object );

    this->initialize();

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    ::PolygonRenderingFunction( polygon );

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Sets depth offset.
 *  @param  factor [in] scale factor
 *  @param  units [in] constant depth offset
 */
/*===========================================================================*/
void PolygonRenderer::setDepthOffset( const float factor, const float units )
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
void PolygonRenderer::setAntiAliasingEnabled( const bool enable, const bool multisample ) const
{
    m_enable_anti_aliasing = enable;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  @brief  Enables two-side lighting option.
 *  @param  enable [in] if true, two-side lighting is enabled
 */
/*===========================================================================*/
void PolygonRenderer::setTwoSideLightingEnabled( const bool enable ) const
{
    m_enable_two_side_lighting = enable;
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing option.
 *  @param  multisample [in] if true, multi-sample anti-aliasing is enabled
 */
/*===========================================================================*/
void PolygonRenderer::enableAntiAliasing( const bool multisample ) const
{
    this->setAntiAliasingEnabled( true, multisample );
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing option.
 */
/*===========================================================================*/
void PolygonRenderer::disableAntiAliasing() const
{
    this->setAntiAliasingEnabled( false, false );
}

/*===========================================================================*/
/**
 *  @brief  Enables two-side lighting option.
 */
/*===========================================================================*/
void PolygonRenderer::enableTwoSideLighting() const
{
    this->setTwoSideLightingEnabled( true );
}

/*===========================================================================*/
/**
 *  @brief  Disables two-side lighting option.
 */
/*===========================================================================*/
void PolygonRenderer::disableTwoSideLighting() const
{
    this->setTwoSideLightingEnabled( false );
}

/*==========================================================================*/
/**
 *  Initialize OpenGL status for the rendering.
 */
/*==========================================================================*/
void PolygonRenderer::initialize()
{
    // Lighting
    kvs::Light::SetModelTwoSide( this->isTwoSideLightingEnabled() );
    if( !this->isShadingEnabled() )
    {
        kvs::OpenGL::Disable( GL_NORMALIZE );
        kvs::OpenGL::Disable( GL_LIGHTING );
    }
    else
    {
        kvs::OpenGL::Enable( GL_NORMALIZE );
        kvs::OpenGL::Enable( GL_LIGHTING );
    }

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
            kvs::OpenGL::Enable( GL_POLYGON_SMOOTH );
            kvs::OpenGL::Enable( GL_BLEND );
            kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            kvs::OpenGL::Hint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
        }
    }

    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    kvs::OpenGL::Disable( GL_LINE_SMOOTH );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );

#if 0
    if ( this->isShadingEnabled() )
    {
        if ( polygon->normals().size() == 0 )
        {
            glEnable( GL_AUTO_NORMAL );
        }
    }
#endif
}

} // end of namespace kvs
