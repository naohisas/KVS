/*****************************************************************************/
/**
 *  @file   ScatterPlotRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScatterPlotRenderer.cpp 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ScatterPlotRenderer.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/RGBAColor>
#include <kvs/TableObject>


namespace
{

void BeginDraw()
{
    GLint vp[4]; glGetIntegerv( GL_VIEWPORT, vp );
    const GLint left = vp[0];
    const GLint bottom = vp[1];
    const GLint right = vp[2];
    const GLint top = vp[3];

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glMatrixMode( GL_MODELVIEW );  glPushMatrix(); glLoadIdentity();
    glMatrixMode( GL_PROJECTION ); glPushMatrix(); glLoadIdentity();
    glOrtho( left, right, top, bottom, -1, 1 ); // The origin is upper-left.
}

void EndDraw()
{
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glPopAttrib();
}

} // end of namespace

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ScatterPlotRenderer class.
 */
/*===========================================================================*/
ScatterPlotRenderer::ScatterPlotRenderer():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_has_point_color( false ),
    m_point_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_point_opacity( 255 ),
    m_point_size( 1.0f ),
    m_color_map( 256 ),
    m_background_color( 0, 0, 0, 0.0f )
{
    m_color_map.create();
}

/*===========================================================================*/
/**
 *  @brief  Render scatter plot.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void ScatterPlotRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::TableObject* table = kvs::TableObject::DownCast( object );
    if ( !m_has_point_color ) if ( table->numberOfColumns() < 3 ) m_has_point_color = true;

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    ::BeginDraw();

    glDisable( GL_LIGHTING );
    glEnable( GL_POINT_SMOOTH );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // X and Y values.
    const kvs::AnyValueArray& x_values = table->column(0);
    const kvs::AnyValueArray& y_values = table->column(1);
    const kvs::Real64 x_min_value = table->minValue(0);
    const kvs::Real64 x_max_value = table->maxValue(0);
    const kvs::Real64 y_min_value = table->minValue(1);
    const kvs::Real64 y_max_value = table->maxValue(1);

    const int x0 = m_left_margin;
    const int x1 = camera->windowWidth() - m_right_margin;
    const int y0 = m_top_margin;
    const int y1 = camera->windowHeight() - m_bottom_margin;

    // Draw background.
    if ( m_background_color.a() > 0.0f )
    {
        const GLubyte r = static_cast<GLubyte>( m_background_color.r() );
        const GLubyte g = static_cast<GLubyte>( m_background_color.g() );
        const GLubyte b = static_cast<GLubyte>( m_background_color.b() );
        const GLubyte a = static_cast<GLubyte>( m_background_color.a() * 255.0f );
        glBegin( GL_QUADS );
        glColor4ub( r, g, b, a );
        glVertex2i( x0, y0 );
        glVertex2i( x1, y0 );
        glVertex2i( x1, y1 );
        glVertex2i( x0, y1 );
        glEnd();
    }

    const kvs::UInt8 opacity = m_point_opacity;
    const kvs::Real32 size = m_point_size;
    if ( m_has_point_color )
    {
        const kvs::RGBColor color = m_point_color;

        glPointSize( size );
        glBegin( GL_POINTS );
        glColor4ub( color.r(), color.g(), color.b(), opacity );
        const kvs::Real64 x_ratio = kvs::Real64( x1 - x0 ) / ( x_max_value - x_min_value );
        const kvs::Real64 y_ratio = kvs::Real64( y1 - y0 ) / ( y_max_value - y_min_value );
        const size_t nrows = table->numberOfRows();
        for ( size_t i = 0; i < nrows; i++ )
        {
            if ( !table->insideRange( i ) ) continue;

            const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
            const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
            const double x = x0 + ( x_value - x_min_value ) * x_ratio;
            const double y = y1 - ( y_value - y_min_value ) * y_ratio;
            glVertex2d( x, y );
        }
        glEnd();
    }
    else
    {
        const float color_axis_min_value = static_cast<float>( table->minValue(2) );
        const float color_axis_max_value = static_cast<float>( table->maxValue(2) );
        const kvs::AnyValueArray& color_axis_values = table->column(2);
        m_color_map.setRange( color_axis_min_value, color_axis_max_value );

        glPointSize( size );
        glBegin( GL_POINTS );
        const kvs::Real64 x_ratio = kvs::Real64( x1 - x0 ) / ( x_max_value - x_min_value );
        const kvs::Real64 y_ratio = kvs::Real64( y1 - y0 ) / ( y_max_value - y_min_value );
        const size_t nrows = table->numberOfRows();
        for ( size_t i = 0; i < nrows; i++ )
        {
            if ( !table->insideRange( i ) ) continue;

            const kvs::Real64 color_value = color_axis_values[i].to<kvs::Real64>();
            const kvs::RGBColor color = m_color_map.at( static_cast<float>( color_value) );
            glColor4ub( color.r(), color.g(), color.b(), opacity );

            const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
            const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
            const double x = x0 + ( x_value - x_min_value ) * x_ratio;
            const double y = y1 - ( y_value - y_min_value ) * y_ratio;
            glVertex2d( x, y );
        }
        glEnd();
    }

    ::EndDraw();

    glPopAttrib();

    BaseClass::stopTimer();
}

} // end of namespace kvs
