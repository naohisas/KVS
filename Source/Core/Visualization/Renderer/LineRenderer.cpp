/****************************************************************************/
/**
 *  @file   LineRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineRenderer.cpp 1634 2013-09-06 08:55:47Z naohisa.sakamoto@gmail.com $
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
 *  @brief  Contructs a new LineRenderer class.
 */
/*==========================================================================*/
LineRenderer::LineRenderer():
    m_enable_anti_aliasing( false ),
    m_enable_multisample_anti_aliasing( false )
{
    // Disable shading since the line object don't have the normal vectors.
    this->disableShading();
}

/*==========================================================================*/
/**
 *  @brief  Destructs the LineRenderer class.
 */
/*==========================================================================*/
LineRenderer::~LineRenderer()
{
}

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
    kvs::IgnoreUnusedVariable( camera );

    kvs::LineObject* line = reinterpret_cast<kvs::LineObject*>( object );

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    this->initialize();

    glEnable( GL_DEPTH_TEST );
    ::LineRenderingFunction( line );
    glDisable( GL_DEPTH_TEST );

    glPopAttrib();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing.
 */
/*===========================================================================*/
void LineRenderer::enableAntiAliasing( const bool multisample ) const
{
    m_enable_anti_aliasing = true;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing.
 */
/*===========================================================================*/
void LineRenderer::disableAntiAliasing() const
{
    m_enable_anti_aliasing = false;
    m_enable_multisample_anti_aliasing = false;
}

/*===========================================================================*/
/**
 *  @brief  Initializes the OpenGL properties.
 */
/*===========================================================================*/
void LineRenderer::initialize()
{
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    // Polygon offset.
    glPolygonOffset( 1.0, 0.0 );
    glEnable( GL_POLYGON_OFFSET_FILL );

    // Lighting.
    if ( !BaseClass::isEnabledShading() ) glDisable( GL_LIGHTING );
    else glEnable( GL_LIGHTING );

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
#if defined ( GL_MULTISAMPLE )
        if ( m_enable_multisample_anti_aliasing )
        {
            GLint buffers = 0;
            GLint samples = 0;
            glGetIntegerv( GL_SAMPLE_BUFFERS, &buffers );
            glGetIntegerv( GL_SAMPLES, &samples );
            if ( buffers > 0 && samples > 1 ) glEnable( GL_MULTISAMPLE );
        }
        else
#endif
        {
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        }
    }
}

} // end of namespace kvs
