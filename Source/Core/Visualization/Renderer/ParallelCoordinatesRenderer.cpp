/*****************************************************************************/
/**
 *  @file   ParallelCoordinatesRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "ParallelCoordinatesRenderer.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/TableObject>


namespace
{

void BeginDraw()
{
    GLint vp[4]; kvs::OpenGL::GetIntegerv( GL_VIEWPORT, vp );
    const GLint left = vp[0];
    const GLint bottom = vp[1];
    const GLint right = vp[2];
    const GLint top = vp[3];

    kvs::OpenGL::PushAttrib( GL_ALL_ATTRIB_BITS );
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW ); kvs::OpenGL::PushMatrix(); kvs::OpenGL::LoadIdentity();
    kvs::OpenGL::SetMatrixMode( GL_PROJECTION ); kvs::OpenGL::PushMatrix(); kvs::OpenGL::LoadIdentity();
    kvs::OpenGL::SetOrtho( left, right, top, bottom, -1, 1 ); // The origin is upper-left.
    kvs::OpenGL::Disable( GL_DEPTH_TEST );
}

void EndDraw()
{
    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::PopAttrib();
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParallelCoordinatesRenderer class.
 */
/*===========================================================================*/
ParallelCoordinatesRenderer::ParallelCoordinatesRenderer():
    m_top_margin( 20 ),
    m_bottom_margin( 20 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_enable_anti_aliasing( false ),
    m_enable_multisample_anti_aliasing( false ),
    m_active_axis( 0 ),
    m_line_opacity( 255 ),
    m_line_width( 1.0f ),
    m_color_map( 256 )
{
    m_color_map.create();
}

/*===========================================================================*/
/**
 *  @brief  Enables anti-aliasing mode.
 *  @param  multisample [in] if true, multisampling is available
 */
/*===========================================================================*/
void ParallelCoordinatesRenderer::enableAntiAliasing( const bool multisample ) const
{
    m_enable_anti_aliasing = true;
    m_enable_multisample_anti_aliasing = multisample;
}

/*===========================================================================*/
/**
 *  @brief  Disables anti-aliasing mode.
 */
/*===========================================================================*/
void ParallelCoordinatesRenderer::disableAntiAliasing() const
{
    m_enable_anti_aliasing = false;
    m_enable_multisample_anti_aliasing = false;
}

/*===========================================================================*/
/**
 *  @brief  Render parallel coordinates.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer to light
 */
/*===========================================================================*/
void ParallelCoordinatesRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );

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
            KVS_GL_CALL( glHint( GL_LINE_SMOOTH_HINT, GL_NICEST ) );
        }
    }

    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    ::BeginDraw();

    const float color_axis_min_value = static_cast<float>( table->minValue( m_active_axis ) );
    const float color_axis_max_value = static_cast<float>( table->maxValue( m_active_axis ) );
    const kvs::AnyValueArray& color_axis_values = table->column( m_active_axis );
    m_color_map.setRange( color_axis_min_value, color_axis_max_value );

    const int x0 = m_left_margin;
    const int x1 = camera->windowWidth() - m_right_margin;
    const int y0 = m_top_margin;
    const int y1 = camera->windowHeight() - m_bottom_margin;

    const size_t nrows = table->column(0).size();
    const size_t naxes = table->numberOfColumns();
    const float stride = float( x1 - x0 ) / ( naxes - 1 );
    for ( size_t i = 0; i < nrows; i++ )
    {
        if ( !table->insideRange( i ) ) continue;

        KVS_GL_CALL( glLineWidth( m_line_width ) );
        KVS_GL_CALL_BEG( glBegin( GL_LINE_STRIP ) );

        const kvs::Real64 color_value = color_axis_values[i].to<kvs::Real64>();
        const kvs::RGBColor color = m_color_map.at( static_cast<float>( color_value ) );
        KVS_GL_CALL_VER( glColor4ub( color.r(), color.g(), color.b(), m_line_opacity ) );

        for ( size_t j = 0; j < naxes; j++ )
        {
            const kvs::Real64 min_value = table->minValue(j);
            const kvs::Real64 max_value = table->maxValue(j);
            const kvs::Real64 value = table->column(j)[i].to<kvs::Real64>();

            const kvs::Real64 x = m_left_margin + stride * j;
            const kvs::Real64 y = y1 - ( y1 - y0 ) * ( value - min_value ) / ( max_value - min_value );
            KVS_GL_CALL_VER( glVertex2d( x, y ) );
        }

        KVS_GL_CALL_END( glEnd() );
    }

    ::EndDraw();

    BaseClass::stopTimer();
}

} // end of namespace kvs
