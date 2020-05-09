/****************************************************************************/
/**
 *  @file   PolygonRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonRenderer.cpp 1634 2013-09-06 08:55:47Z naohisa.sakamoto@gmail.com $
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
 *  Constructor.
 */
/*==========================================================================*/
PolygonRenderer::PolygonRenderer():
    m_enable_anti_aliasing( false ),
    m_enable_multisample_anti_aliasing( false ),
    m_enable_two_side_lighting( true ),
    m_polygon_offset( 0.0f )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
PolygonRenderer::~PolygonRenderer()
{
}

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

    kvs::PolygonObject* polygon = kvs::PolygonObject::DownCast( object );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

/*
    if ( this->isEnabledShading() )
    {
        if ( polygon->normals().size() == 0 )
        {
            glEnable( GL_AUTO_NORMAL );
        }
    }
*/

    this->initialize();
#if KVS_ENABLE_DEPRECATED
    polygon->applyMaterial();
#endif

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

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    ::PolygonRenderingFunction( polygon );
    kvs::OpenGL::Disable( GL_DEPTH_TEST );

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  Enables anti-aliasing.
 */
/*===========================================================================*/
void PolygonRenderer::enableAntiAliasing( const bool multisample ) const
{
    m_enable_anti_aliasing = true;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  Disables anti-aliasing.
 */
/*===========================================================================*/
void PolygonRenderer::disableAntiAliasing() const
{
    m_enable_anti_aliasing = false;
    m_enable_multisample_anti_aliasing = false;
}

void PolygonRenderer::enableTwoSideLighting() const
{
    m_enable_two_side_lighting = true;
}

void PolygonRenderer::disableTwoSideLighting() const
{
    m_enable_two_side_lighting = false;
}

bool PolygonRenderer::isTwoSideLighting() const
{
    return( m_enable_two_side_lighting );
}

/*==========================================================================*/
/**
 *  Initialize OpenGL status for the rendering.
 */
/*==========================================================================*/
void PolygonRenderer::initialize()
{
    kvs::OpenGL::Disable( GL_LINE_SMOOTH );

    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    kvs::OpenGL::SetShadeModel( GL_SMOOTH );

    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );

    // Polygon offset.
    kvs::OpenGL::SetPolygonOffset( m_polygon_offset, 0.0 );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    if( !this->isEnabledShading() )
    {
        kvs::OpenGL::Disable( GL_NORMALIZE );
        kvs::OpenGL::Disable( GL_LIGHTING );
    }
    else
    {
        kvs::OpenGL::Enable( GL_NORMALIZE );
        kvs::OpenGL::Enable( GL_LIGHTING );
    }

    kvs::Light::SetModelTwoSide( this->isTwoSideLighting() );
}

} // end of namespace kvs
