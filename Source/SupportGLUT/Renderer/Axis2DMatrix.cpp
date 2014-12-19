/*****************************************************************************/
/**
 *  @file   Axis2DMatrix.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Axis2DMatrix.cpp 1457 2013-03-24 06:32:17Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Axis2DMatrix.h"
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
 *  @brief  Constructs a new Axis2DMatrix class.
 */
/*===========================================================================*/
Axis2DMatrix::Axis2DMatrix():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_margin( 1 ),
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
void Axis2DMatrix::setTopMargin( const int top_margin )
{
    m_top_margin = top_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets bottom margin.
 *  @param  bottom_margin [in] margin
 */
/*===========================================================================*/
void Axis2DMatrix::setBottomMargin( const int bottom_margin )
{
    m_bottom_margin = bottom_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets left margin.
 *  @param  left_margin [in] left margin
 */
/*===========================================================================*/
void Axis2DMatrix::setLeftMargin( const int left_margin )
{
    m_left_margin = left_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets right margin.
 *  @param  right_margin [in] right margin
 */
/*===========================================================================*/
void Axis2DMatrix::setRightMargin( const int right_margin )
{
    m_right_margin = right_margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets margin
 *  @param  margin [in] margin
 */
/*===========================================================================*/
void Axis2DMatrix::setMargin( const int margin )
{
    m_margin = margin;
}

/*===========================================================================*/
/**
 *  @brief  Sets axis width.
 *  @param  axis_width [in] axis width
 */
/*===========================================================================*/
void Axis2DMatrix::setAxisWidth( const kvs::Real32 axis_width )
{
    m_axis_width = axis_width;
}

/*===========================================================================*/
/**
 *  @brief  Sets axis color.
 *  @param  axis_color [in] axis color
 */
/*===========================================================================*/
void Axis2DMatrix::setAxisColor( const kvs::RGBColor axis_color )
{
    m_axis_color = axis_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets value color.
 *  @param  value_color [in] value color
 */
/*===========================================================================*/
void Axis2DMatrix::setValueColor( const kvs::RGBColor value_color )
{
    m_value_color = value_color;
}

/*===========================================================================*/
/**
 *  @brief  Sets label color.
 *  @param  label_color [in] label color
 */
/*===========================================================================*/
void Axis2DMatrix::setLabelColor( const kvs::RGBColor label_color )
{
    m_label_color = label_color;
}

/*===========================================================================*/
/**
 *  @brief  Returns top margin.
 *  @return top margin
 */
/*===========================================================================*/
int Axis2DMatrix::topMargin() const
{
    return( m_top_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns bottom margin.
 *  @return bottom margin
 */
/*===========================================================================*/
int Axis2DMatrix::bottomMargin() const
{
    return( m_bottom_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns left margin.
 *  @return left margin
 */
/*===========================================================================*/
int Axis2DMatrix::leftMargin() const
{
    return( m_left_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns right margin.
 *  @return right margin
 */
/*===========================================================================*/
int Axis2DMatrix::rightMargin() const
{
    return( m_right_margin );
}

/*===========================================================================*/
/**
 *  @brief  Returns axis width.
 *  @return axis width
 */
/*===========================================================================*/
kvs::Real32 Axis2DMatrix::axisWidth() const
{
    return( m_axis_width );
}

/*===========================================================================*/
/**
 *  @brief  Returns axis color.
 *  @return axis color.
 */
/*===========================================================================*/
const kvs::RGBColor& Axis2DMatrix::axisColor() const
{
    return( m_axis_color );
}

/*===========================================================================*/
/**
 *  @brief  Returns value color.
 *  @return value color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis2DMatrix::valueColor() const
{
    return( m_value_color );
}

/*===========================================================================*/
/**
 *  @brief  Returns label color.
 *  @return label color
 */
/*===========================================================================*/
const kvs::RGBColor& Axis2DMatrix::labelColor() const
{
    return( m_label_color );
}

/*===========================================================================*/
/**
 *  @brief  Render 2D axis matrix.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void Axis2DMatrix::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    ::BeginDraw();

    const int X0 = m_left_margin;
    const int X1 = camera->windowWidth() - m_right_margin;
    const int Y0 = m_top_margin;
    const int Y1 = camera->windowHeight() - m_bottom_margin;

    const int ncolumns = table->numberOfColumns();
    const float X_stride = float( X1 - X0 - m_margin * ( ncolumns - 1 ) ) / ncolumns;
    const float Y_stride = float( Y1 - Y0 - m_margin * ( ncolumns - 1 ) ) / ncolumns;

    for ( int i = 0; i < ncolumns; i++ )
    {
        for ( int j = 0; j < ncolumns; j++ )
        {
            const float x0 = X0 + ( X_stride + m_margin ) * j;
            const float y0 = Y0 + ( Y_stride + m_margin ) * i;
            const float x1 = x0 + X_stride;
            const float y1 = y0 + Y_stride;

            // Draw axes.
            glLineWidth( m_axis_width );
            glBegin( GL_LINES );
            glColor4ub( m_axis_color.r(), m_axis_color.g(), m_axis_color.b(), 255 );
            glVertex2f( x0 - int( m_axis_width * 0.5f ), y1 ); glVertex2f( x1, y1 ); // X axis
            glVertex2f( x0, y1 + int( m_axis_width * 0.5f - 0.5f ) ); glVertex2f( x0, y0 ); // Y axis
            glEnd();

            // Draw min/max values.
            const size_t x_index = j;
            const size_t y_index = ncolumns - i - 1;

            if ( y_index == 0 )
            {
                const std::string x_min_value = kvs::String::ToString( table->minValue( x_index ) );
                const std::string x_max_value = kvs::String::ToString( table->maxValue( x_index ) );

                const float x_min_position_x = x0;
                const float x_min_position_y = y1 + ::CharacterHeight + 5;
                ::DrawString( x_min_value, x_min_position_x, x_min_position_y, m_value_color );

                const float x_max_position_x = x1 - x_max_value.size() * ::CharacterWidth;
                const float x_max_position_y = y1 + ::CharacterHeight + 5;
                ::DrawString( x_max_value, x_max_position_x, x_max_position_y, m_value_color );
            }

            if ( x_index == 0 )
            {
                const std::string y_min_value = kvs::String::ToString( table->minValue( y_index ) );
                const std::string y_max_value = kvs::String::ToString( table->maxValue( y_index ) );

                const float y_min_position_x = x0 - y_min_value.size() * ::CharacterWidth - 5;
                const float y_min_position_y = y1;
                ::DrawString( y_min_value, y_min_position_x, y_min_position_y, m_value_color );

                const float y_max_position_x = x0 - y_max_value.size() * ::CharacterWidth - 5;
                const float y_max_position_y = y0 + ::CharacterHeight;
                ::DrawString( y_max_value, y_max_position_x, y_max_position_y, m_value_color );
            }

            // Draw labels.
            if ( x_index != y_index )
            {
                const std::string x_label( table->label(x_index) );
                const std::string y_label( table->label(y_index) );
                const size_t x_label_width = x_label.size() * ::CharacterWidth;

                const float x_label_position_x = x1 - x_label_width - 5;
                const float x_label_position_y = y1 - 5;
                ::DrawString( x_label, x_label_position_x, x_label_position_y, m_label_color );

                const float y_label_position_x = x0 + 5;
                const float y_label_position_y = y0 + ::CharacterWidth + 5;
                ::DrawString( y_label, y_label_position_x, y_label_position_y, m_label_color );
            }
            else
            {
                const std::string label( table->label(x_index) );
                const size_t label_width = label.size() * ::CharacterWidth;

                const float label_position_x = ( x1 + x0 ) * 0.5f - label_width * 0.5f;
                const float label_position_y = ( y1 + y0 ) * 0.5f + ::CharacterHeight * 0.5f;
                ::DrawString( label, label_position_x, label_position_y, m_label_color );
            }
        }
    }

    ::EndDraw();

    glPopAttrib();

    BaseClass::stopTimer();
}

} // end of namespace glut

} // end of namespace kvs
