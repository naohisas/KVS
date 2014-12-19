/*****************************************************************************/
/**
 *  @file   Axis2D.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Axis2D.cpp 1457 2013-03-24 06:32:17Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Axis2D.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/String>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/glut/GLUT>
#include <kvs/TableObject>


namespace
{
void* CharacterFont = GLUT_BITMAP_8_BY_13;
const int CharacterWidth = 8;
const int CharacterHeight = 12;
}

namespace
{

void BeginDraw( void )
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

void EndDraw( void )
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
 *  @brief  Constructs a new Axis2D class.
 */
/*===========================================================================*/
Axis2D::Axis2D():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
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
 *  @brief  Sets top margin.
 *  @param  top_margin [in] margin
 */
/*===========================================================================*/
void Axis2D::setTopMargin( const int top_margin )
{
    m_top_margin = top_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets bottom margin.
 *  @param  bottom_margin [in] margin
 */
/*===========================================================================*/
void Axis2D::setBottomMargin( const int bottom_margin )
{
    m_bottom_margin = bottom_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets left margin.
 *  @param  left_margin [in] left margin
 */
/*===========================================================================*/
void Axis2D::setLeftMargin( const int left_margin )
{
    m_left_margin = left_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets right margin.
 *  @param  right_margin [in] right margin
 */
/*===========================================================================*/
void Axis2D::setRightMargin( const int right_margin )
{
    m_right_margin = right_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets axis width.
 *  @param  axis_width [in] axis width
 */
/*===========================================================================*/
void Axis2D::setAxisWidth( const kvs::Real32 axis_width )
{
    m_axis_width = axis_width;
}

/*===========================================================================*/
/**
 *  @brief  Sets axis color.
 *  @param  axis_color [in] axis color
 */
/*===========================================================================*/
void Axis2D::setAxisColor( const kvs::RGBColor axis_color )
{
    m_axis_color = axis_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets value color.
 *  @param  value_color [in] value color
 */
/*===========================================================================*/
void Axis2D::setValueColor( const kvs::RGBColor value_color )
{
    m_value_color = value_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets label color.
 *  @param  label_color [in] label color
 */
/*===========================================================================*/
void Axis2D::setLabelColor( const kvs::RGBColor label_color )
{
    m_label_color = label_color;
}

/*===========================================================================*/
/**
 *  @brief  Returns top margin.
 *  @return top margin
 */
/*===========================================================================*/
int Axis2D::topMargin() const
{
    return( m_top_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns bottom margin.
 *  @return bottom margin
 */
/*===========================================================================*/
int Axis2D::bottomMargin() const
{
    return( m_bottom_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns left margin.
 *  @return left margin
 */
/*===========================================================================*/
int Axis2D::leftMargin() const
{
    return( m_left_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns right margin.
 *  @return right margin
 */
/*===========================================================================*/
int Axis2D::rightMargin() const
{
    return( m_right_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns axis width.
 *  @return axis width
 */
/*===========================================================================*/
kvs::Real32 Axis2D::axisWidth() const
{
    return( m_axis_width );
}

/*===========================================================================*/
/**
 *  @brief  Returns axis color.
 *  @return axis color.
 */
/*===========================================================================*/
const kvs::RGBColor& Axis2D::axisColor() const
{
    return( m_axis_color );
}

/*===========================================================================*/
/**
 *  @brief  Returns value color.
 *  @return value color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis2D::valueColor() const
{
    return( m_value_color );
}

/*===========================================================================*/
/**
 *  @brief  Returns label color.
 *  @return label color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis2D::labelColor() const
{
    return( m_label_color );
}

/*===========================================================================*/
/**
 *  @brief  Render 2D axis.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void Axis2D::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    ::BeginDraw();

    const int x0 = m_left_margin;
    const int x1 = camera->windowWidth() - m_right_margin;
    const int y0 = m_top_margin;
    const int y1 = camera->windowHeight() - m_bottom_margin;

    // Draw axes.
    glLineWidth( m_axis_width );
    glBegin( GL_LINES );
    glColor4ub( m_axis_color.r(), m_axis_color.g(), m_axis_color.b(), 255 );
    glVertex2f( x0 - int( m_axis_width * 0.5 ), y1 ); glVertex2f( x1, y1 ); // X axis
    glVertex2f( x0, y1 + int( m_axis_width * 0.5 ) ); glVertex2f( x0, y0 ); // Y axis
    glEnd();

    // Draw min/max values.
    const std::string x_min_value = kvs::String::ToString( table->minValue(0) );
    const std::string x_max_value = kvs::String::ToString( table->maxValue(0) );

    const float x_min_position_x = x0;
    const float x_min_position_y = y1 + ::CharacterHeight + 5;
    ::DrawString( x_min_value, x_min_position_x, x_min_position_y, m_value_color );

    const float x_max_position_x = x1 - x_max_value.size() * ::CharacterWidth;
    const float x_max_position_y = y1 + ::CharacterHeight + 5;
    ::DrawString( x_max_value, x_max_position_x, x_max_position_y, m_value_color );

    const std::string y_min_value = kvs::String::ToString( table->minValue(1) );
    const std::string y_max_value = kvs::String::ToString( table->maxValue(1) );

    const float y_min_position_x = x0 - y_min_value.size() * ::CharacterWidth - 5;
    const float y_min_position_y = y1;
    ::DrawString( y_min_value, y_min_position_x, y_min_position_y, m_value_color );

    const float y_max_position_x = x0 - y_max_value.size() * ::CharacterWidth - 5;
    const float y_max_position_y = y0 + ::CharacterHeight;
    ::DrawString( y_max_value, y_max_position_x, y_max_position_y, m_value_color );

    // Draw labels.
    const std::string x_label( table->label(0) );
    const std::string y_label( table->label(1) );
    const size_t x_label_width = x_label.size() * ::CharacterWidth;

    const float x_label_position_x = x1 - x_label_width - 5;
    const float x_label_position_y = y1 - m_axis_width * 0.5f - 5;
    ::DrawString( x_label, x_label_position_x, x_label_position_y, m_label_color );

    const float y_label_position_x = x0 + m_axis_width * 0.5f + 5;
    const float y_label_position_y = y0 + ::CharacterWidth + 5;
    ::DrawString( y_label, y_label_position_x, y_label_position_y, m_label_color );

    ::EndDraw();

    glPopAttrib();

    BaseClass::stopTimer();
}

} // end of namesapce glut

} // end of namespace kvs
