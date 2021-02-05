/****************************************************************************/
/**
 *  @file   PointRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "PointRenderer.h"
#include "PointRenderingFunction.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/PointObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  Point rendering method.
 *  @param object [in] pointer to the object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void PointRenderer::exec( ObjectBase* object, Camera* camera, Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    auto* point = kvs::PointObject::DownCast( object );
    if ( point->normals().size() == 0 ) { BaseClass::disableShading(); }

    this->initialize();
    ::PointRenderingFunction( point, camera->devicePixelRatio() );

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Sets depth offset.
 *  @param  factor [in] scale factor
 *  @param  units [in] constant depth offset
 */
/*===========================================================================*/
void PointRenderer::setDepthOffset( const float factor, const float units )
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
void PointRenderer::setAntiAliasingEnabled( const bool enable, const bool multisample ) const
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
void PointRenderer::setTwoSideLightingEnabled( const bool enable ) const
{
    m_enable_two_side_lighting = enable;
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing option.
 *  @param  multisample [in] if true, multi-sample anti-aliasing is enabled
 */
/*===========================================================================*/
void PointRenderer::enableAntiAliasing( const bool multisample ) const
{
    this->setAntiAliasingEnabled( true, multisample );
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing option.
 */
/*===========================================================================*/
void PointRenderer::disableAntiAliasing() const
{
    this->setAntiAliasingEnabled( false, false );
}

/*===========================================================================*/
/**
 *  @brief  Enables two-side lighting option.
 */
/*===========================================================================*/
void PointRenderer::enableTwoSideLighting() const
{
    this->setTwoSideLightingEnabled( true );
}

/*===========================================================================*/
/**
 *  @brief  Disables two-side lighting option.
 */
/*===========================================================================*/
void PointRenderer::disableTwoSideLighting() const
{
    this->setTwoSideLightingEnabled( false );
}

/*===========================================================================*/
/**
 *  @brief  Initializes the OpenGL properties.
 */
/*===========================================================================*/
void PointRenderer::initialize()
{
    // Lighting
    kvs::Light::SetModelTwoSide( this->isTwoSideLightingEnabled() );
    if ( !this->isShadingEnabled() )
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

    // Anti-aliasing
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
            kvs::OpenGL::Enable( GL_POINT_SMOOTH );
            kvs::OpenGL::Enable( GL_BLEND );
            kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            kvs::OpenGL::Hint( GL_POINT_SMOOTH_HINT, GL_NICEST );
        }
    }

    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    kvs::OpenGL::Enable( GL_POINT_SMOOTH ); // Rounded shape.
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
}

} // end of namespace kvs
