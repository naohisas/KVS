/*****************************************************************************/
/**
 *  @file   ParallelAxis.cpp
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
#include "ParallelAxis.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/String>
#include <kvs/RGBColor>
#include <kvs/glut/GLUT>
#include <kvs/TableObject>
#include <kvs/Math>


namespace
{
void* CharacterFont = GLUT_BITMAP_8_BY_13;
const int CharacterWidth = 8;
const int CharacterHeight = 12;
}

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
    glDisable( GL_DEPTH_TEST );
}

void EndDraw()
{
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glPopAttrib();
}

void DrawString( const std::string& text, const float x, const float y, const kvs::RGBColor& color )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glColor3ub( color.r(), color.g(), color.b() );

    glDisable( GL_TEXTURE_1D );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    glRasterPos2f( x, y );
    char* line_head = const_cast<char*>( text.c_str() );
    for( char* p = line_head; *p; p++ )
    {
        glutBitmapCharacter( ::CharacterFont, *p );
    }
    glPopAttrib();
}

} // end of namespace


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParallelAxis class.
 */
/*===========================================================================*/
ParallelAxis::ParallelAxis():
    m_top_margin( 20 ),
    m_bottom_margin( 20 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_axis_width( 2.0f ),
    m_axis_color( 0, 0, 0 ),
    m_value_color( 0, 0, 0 ),
    m_label_color( 0, 0, 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns top margin.
 *  @return top margin
 */
/*===========================================================================*/
size_t ParallelAxis::topMargin() const
{
    return( m_top_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns bottom margin.
 *  @return bottom margin
 */
/*===========================================================================*/
size_t ParallelAxis::bottomMargin() const
{
    return( m_bottom_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns left margin.
 *  @return left margin
 */
/*===========================================================================*/
size_t ParallelAxis::leftMargin() const
{
    return( m_left_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns right margin.
 *  @return right margin
 */
/*===========================================================================*/
size_t ParallelAxis::rightMargin() const
{
    return( m_right_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns axis width.
 *  @return axis width
 */
/*===========================================================================*/
kvs::Real32 ParallelAxis::axisWidth() const
{
    return( m_axis_width );
}

/*===========================================================================*/
/**
 *  @brief  Returns axis color.
 *  @return axis color.
 */
/*===========================================================================*/
const kvs::RGBColor& ParallelAxis::axisColor() const
{
    return( m_axis_color );
}

/*===========================================================================*/
/**
 *  @brief  Returns value color.
 *  @return value color
 */
/*===========================================================================*/
const kvs::RGBColor& ParallelAxis::valueColor() const
{
    return( m_value_color );
}

/*===========================================================================*/
/**
 *  @brief  Returns label color.
 *  @return label color
 */
/*===========================================================================*/
const kvs::RGBColor& ParallelAxis::labelColor() const
{
    return( m_label_color );
}

/*===========================================================================*/
/**
 *  @brief  Sets top margin.
 *  @param  top_margin [in] margin
 */
/*===========================================================================*/
void ParallelAxis::setTopMargin( const size_t top_margin )
{
    m_top_margin = top_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets bottom margin.
 *  @param  bottom_margin [in] margin
 */
/*===========================================================================*/
void ParallelAxis::setBottomMargin( const size_t bottom_margin )
{
    m_bottom_margin = bottom_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets left margin.
 *  @param  left_margin [in] margin
 */
/*===========================================================================*/
void ParallelAxis::setLeftMargin( const size_t left_margin )
{
    m_left_margin = left_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets right margin.
 *  @param  right_margin [in] margin
 */
/*===========================================================================*/
void ParallelAxis::setRightMargin( const size_t right_margin )
{
    m_right_margin = right_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets axis width.
 *  @param  axis_width [in] axis width
 */
/*===========================================================================*/
void ParallelAxis::setAxisWidth( const kvs::Real32 axis_width )
{
    m_axis_width = axis_width;
}

/*===========================================================================*/
/**
 *  @brief  Sets axis color.
 *  @param  axis_color [in] axis color
 */
/*===========================================================================*/
void ParallelAxis::setAxisColor( const kvs::RGBColor axis_color )
{
    m_axis_color = axis_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets value color.
 *  @param  value_color [in] value color
 */
/*===========================================================================*/
void ParallelAxis::setValueColor( const kvs::RGBColor value_color )
{
    m_value_color = value_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets label color.
 *  @param  label_color [in] label color
 */
/*===========================================================================*/
void ParallelAxis::setLabelColor( const kvs::RGBColor label_color )
{
    m_label_color = label_color;
}

/*===========================================================================*/
/**
 *  @brief  Render parallel axis.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void ParallelAxis::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    ::BeginDraw();

    bool has_label = false;
    for ( size_t i = 0; i < table->labels().size(); i++ )
    {
        if ( table->labels().at(i).size() > 0 ) { has_label = true; break; }
    }

    const int x0 = m_left_margin;
    const int x1 = camera->windowWidth() - m_right_margin;
    const int y0 = m_top_margin;
    const int y1 = camera->windowHeight() - m_bottom_margin;

    // Draw axes.
    glLineWidth( m_axis_width );
    glBegin( GL_LINES );
    const size_t naxes = table->numberOfColumns();
    const float stride = float( x1 - x0 ) / ( naxes - 1 );
    for ( size_t i = 0; i < naxes; i++ )
    {
        const float x = m_left_margin + stride * i;
        glColor4ub( m_axis_color.r(), m_axis_color.g(), m_axis_color.b(), 255 );
        glVertex2f( x, y0 );
        glVertex2f( x, y1 );
    }
    glEnd();

    // Draw min/max values and label.
    for ( size_t i = 0; i < naxes; i++ )
    {
        std::string max_value;
        std::string min_value;
        if ( std::ceil( table->maxValue(i) ) == std::floor( table->maxValue(i) ) )
        {
            max_value = kvs::String::ToString( int(table->maxValue(i)) );
            min_value = kvs::String::ToString( int(table->minValue(i)) );
        }
        else
        {
            max_value = kvs::String::ToString( table->maxValue(i) );
            min_value = kvs::String::ToString( table->minValue(i) );
        }

        const size_t max_width = max_value.size() * ::CharacterWidth;
        const size_t min_width = min_value.size() * ::CharacterWidth;
        const size_t height = ::CharacterHeight;

        const float max_x = ( m_left_margin + stride * i ) - max_width * 0.5f;
        const float max_y = y0 - 5.0f;
        const float min_x = ( m_left_margin + stride * i ) - min_width * 0.5f;
        const float min_y = y1 + height;

        ::DrawString( max_value, kvs::Math::Max( 0.0f, max_x ), max_y, m_value_color );
        ::DrawString( min_value, kvs::Math::Max( 0.0f, min_x ), min_y, m_value_color );

        if ( has_label )
        {
            const std::string label( table->label(i) );
            const size_t label_width = label.size() * ::CharacterWidth;
            const float label_x = ( m_left_margin + stride * i ) - label_width * 0.5f;
            const float label_y = min_y + height;
            ::DrawString( label, kvs::Math::Max( 0.0f, label_x ), label_y, m_label_color );
        }
    }

    ::EndDraw();

    BaseClass::stopTimer();
}

} // end of namespace glut

} // end of namespace kvs
