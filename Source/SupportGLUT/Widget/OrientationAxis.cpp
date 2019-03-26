/****************************************************************************/
/**
 *  @file   OrientationAxis.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OrientationAxis.cpp 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "OrientationAxis.h"
#include <kvs/ObjectManager>
#include <kvs/EventBase>
#include <kvs/ScreenBase>
#include <kvs/OpenGL>
#include <kvs/glut/Screen>
#include <kvs/glut/GLUT>
#include <kvs/IgnoreUnusedVariable>


// Default parameters.
namespace { namespace Default
{
const size_t AxisWindowSize = 80;
} }

namespace
{

inline void DrawString( const std::string& str, const kvs::Vec3& pos )
{
    kvs::OpenGL::SetRasterPos( pos );
    char* y_char = const_cast<char*>( str.c_str() );
    for ( char* p = y_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }
}

}


namespace kvs
{

namespace glut
{

OrientationAxis::OrientationAxis( kvs::glut::Screen* screen ):
    kvs::glut::WidgetBase( screen ),
    m_object( screen->scene()->objectManager() ),
    m_x_tag( "x" ),
    m_y_tag( "y" ),
    m_z_tag( "z" ),
    m_x_axis_color( kvs::RGBColor( 200, 10, 10 ) ),
    m_y_axis_color( kvs::RGBColor( 10, 200, 10 ) ),
    m_z_axis_color( kvs::RGBColor( 10, 10, 200 ) ),
    m_axis_line_width( 1.0f ),
    m_box_color( kvs::RGBColor( 200, 200, 200 ) ),
    m_box_line_color( kvs::RGBColor( 10, 10, 10 ) ),
    m_box_line_width( 1.0f ),
    m_enable_anti_aliasing( false ),
    m_axis_type( OrientationAxis::CorneredAxis ),
    m_box_type( OrientationAxis::NoneBox ),
    m_projection_type( kvs::Camera::Perspective )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );

    BaseClass::setMargin( 0 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new orientation axis class.
 *  @param  screen [in] pointer to the screen
 *  @param  scene [in] pointer to the scene
 */
/*===========================================================================*/
OrientationAxis::OrientationAxis( kvs::ScreenBase* screen, const kvs::Scene* scene ):
    kvs::glut::WidgetBase( screen ),
    m_object( scene->objectManager() ),
    m_x_tag( "x" ),
    m_y_tag( "y" ),
    m_z_tag( "z" ),
    m_x_axis_color( kvs::RGBColor( 200, 10, 10 ) ),
    m_y_axis_color( kvs::RGBColor( 10, 200, 10 ) ),
    m_z_axis_color( kvs::RGBColor( 10, 10, 200 ) ),
    m_axis_line_width( 1.0f ),
    m_box_color( kvs::RGBColor( 200, 200, 200 ) ),
    m_box_line_color( kvs::RGBColor( 10, 10, 10 ) ),
    m_box_line_width( 1.0f ),
    m_enable_anti_aliasing( false ),
    m_axis_type( OrientationAxis::CorneredAxis ),
    m_box_type( OrientationAxis::NoneBox ),
    m_projection_type( kvs::Camera::Perspective )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );

    BaseClass::setMargin( 0 );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new orientation axis class.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
OrientationAxis::OrientationAxis( kvs::ScreenBase* screen, const kvs::ObjectBase* object ):
    kvs::glut::WidgetBase( screen ),
    m_object( object ),
    m_x_tag( "x" ),
    m_y_tag( "y" ),
    m_z_tag( "z" ),
    m_x_axis_color( kvs::RGBColor( 200, 10, 10 ) ),
    m_y_axis_color( kvs::RGBColor( 10, 200, 10 ) ),
    m_z_axis_color( kvs::RGBColor( 10, 10, 200 ) ),
    m_axis_line_width( 1.0f ),
    m_box_color( kvs::RGBColor( 200, 200, 200 ) ),
    m_box_line_color( kvs::RGBColor( 10, 10, 10 ) ),
    m_box_line_width( 1.0f ),
    m_enable_anti_aliasing( false ),
    m_axis_type( OrientationAxis::CorneredAxis ),
    m_box_type( OrientationAxis::NoneBox ),
    m_projection_type( kvs::Camera::Perspective )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );

    BaseClass::setMargin( 0 );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the orientation axis class.
 */
/*===========================================================================*/
OrientationAxis::~OrientationAxis()
{
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void OrientationAxis::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    const kvs::Vec4 vp = kvs::OpenGL::Viewport();

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );

    kvs::OpenGL::Disable( GL_LIGHTING );
    kvs::OpenGL::Disable( GL_TEXTURE_2D );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );

    kvs::OpenGL::SetClearDepth( 1.0 );
    kvs::OpenGL::Clear( GL_DEPTH_BUFFER_BIT );

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
        kvs::OpenGL::Enable( GL_LINE_SMOOTH );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    // Set the projection parameters.
    kvs::OpenGL::WithPushedMatrix p1( GL_PROJECTION );
    p1.loadIdentity();
    {
        const float front = 1.0f;
        const float back = 2000.0f;
        if ( m_projection_type == kvs::Camera::Perspective )
        {
            // Perspective projection.
            const float field_of_view = 45.0f;
            const float aspect = 1.0f;
            kvs::OpenGL::SetPerspective( field_of_view, aspect, front, back );
        }
        else
        {
            // Orthogonal projection.
            const float left = -5.0f;
            const float bottom = -5.0f;
            const float right = 5.0f;
            const float top = 5.0f;
            kvs::OpenGL::SetOrtho( left, right, bottom, top, front, back );
        }
    }

    // The origin is set to the top-left on the screen.
    const int x = BaseClass::x0() + m_margin;
    const int y = screen()->height() - BaseClass::y0() - BaseClass::height() + m_margin;
    const int width = BaseClass::width() - m_margin;
    const int height = BaseClass::height() - m_margin;
    kvs::OpenGL::SetViewport( x, y, width, height);

    kvs::OpenGL::WithPushedMatrix p2( GL_MODELVIEW );
    p2.loadIdentity();
    {
        // Viewing transformation.
        const kvs::Vec3 eye( 0.0f, 0.0f, 12.0f );
        const kvs::Vec3 center( 0.0f, 0.0f, 0.0f );
        const kvs::Vec3 up( 0.0f, 1.0f, 0.0f );
        kvs::OpenGL::SetLookAt( eye, center, up );

        // Rotate the axis and the box using the object's rotation matrix.
        kvs::OpenGL::MultMatrix( kvs::Xform::Rotation( m_object->xform().rotation() ) );

        // Fixed length of the axis
        const float length = 4.0f;

        // Draw the box.
        switch ( m_box_type )
        {
        case OrientationAxis::WiredBox: { this->draw_wired_box( length ); break; }
        case OrientationAxis::SolidBox: { this->draw_solid_box( length ); break; }
        default: break;
        }

        // Draw the axis.
        switch ( m_axis_type )
        {
        case OrientationAxis::CorneredAxis: { this->draw_cornered_axis( length ); break; }
        case OrientationAxis::CenteredAxis: { this->draw_centered_axis( length ); break; }
        default: break;
        }
    }

    kvs::OpenGL::SetViewport( vp );
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*===========================================================================*/
void OrientationAxis::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );
    this->screenResized();
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen width.
 *  @return screen width
 */
/*===========================================================================*/
int OrientationAxis::adjustedWidth()
{
    return ::Default::AxisWindowSize;
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen height.
 *  @return screen height
 */
/*===========================================================================*/
int OrientationAxis::adjustedHeight()
{
    return ::Default::AxisWindowSize;
}

/*===========================================================================*/
/**
 *  @brief  Draw the centered axis.
 */
/*===========================================================================*/
void OrientationAxis::draw_centered_axis( const float length )
{
    const float x = 0.0f;
    const float y = 0.0f;
    const float z = 0.0f;
    const kvs::Vec3 v0( x, y, z );
    const kvs::Vec3 v1( x + length, y, z );
    const kvs::Vec3 v3( x, y, z + length );
    const kvs::Vec3 v4( x, y + length, z );

    const float offset = length / 4.0f;
    const kvs::Vec3 offsetx( 0.0f, offset, offset );
    const kvs::Vec3 offsety( offset, 0.0f, offset );
    const kvs::Vec3 offsetz( offset, offset, 0.0f );

    kvs::OpenGL::SetLineWidth( m_axis_line_width );

    // X-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_x_axis_color );
    kvs::OpenGL::Vertices( v0, v1 );
    kvs::OpenGL::End();
    ::DrawString( m_x_tag, v1 - offsetx );

    // Y-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_y_axis_color );
    kvs::OpenGL::Vertices( v0, v4 );
    kvs::OpenGL::End();
    ::DrawString( m_y_tag, v4 - offsety );

    // Z-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_z_axis_color );
    kvs::OpenGL::Vertices( v0, v3 );
    kvs::OpenGL::End();
    ::DrawString( m_z_tag, v3 - offsetz );
}

/*===========================================================================*/
/**
 *  @brief  Draw the boundary axis.
 */
/*===========================================================================*/
void OrientationAxis::draw_cornered_axis( const float length )
{
    const float x = -length/2.0f;
    const float y = -length/2.0f;
    const float z = -length/2.0f;
    const kvs::Vec3 v0( x, y, z );
    const kvs::Vec3 v1( x + length, y, z );
    const kvs::Vec3 v3( x, y, z + length );
    const kvs::Vec3 v4( x, y + length, z );

    const float offset = length / 6.0f;
    const kvs::Vec3 offsetx( 0.0f, offset, offset );
    const kvs::Vec3 offsety( offset, 0.0f, offset );
    const kvs::Vec3 offsetz( offset, offset, 0.0f );

    kvs::OpenGL::SetLineWidth( m_axis_line_width );

    // X-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_x_axis_color );
    kvs::OpenGL::Vertices( v0, v1 );
    kvs::OpenGL::End();
    ::DrawString( m_x_tag, v1 - offsetx );

    // Y-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_y_axis_color );
    kvs::OpenGL::Vertices( v0, v4 );
    kvs::OpenGL::End();
    ::DrawString( m_y_tag, v4 - offsety );

    // Z-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_z_axis_color );
    kvs::OpenGL::Vertices( v0, v3 );
    kvs::OpenGL::End();
    ::DrawString( m_z_tag, v3 - offsetz );
}

/*===========================================================================*/
/**
 *  @brief  Draws the wired box.
 *  @param  length [in] length of the box's edge
 */
/*===========================================================================*/
void OrientationAxis::draw_wired_box( const float length )
{
    const float x = -length/2.0f;
    const float y = -length/2.0f;
    const float z = -length/2.0f;
    const kvs::Vec3 v0( x, y, z );
    const kvs::Vec3 v1( x + length, y, z );
    const kvs::Vec3 v2( x + length, y, z + length );
    const kvs::Vec3 v3( x, y, z + length );
    const kvs::Vec3 v4( x, y + length, z );
    const kvs::Vec3 v5( x + length, y + length, z );
    const kvs::Vec3 v6( x + length, y + length, z + length );
    const kvs::Vec3 v7( x, y + length, z + length );

    kvs::OpenGL::SetLineWidth( m_box_line_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_box_line_color );
    {
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // X axis.
            kvs::OpenGL::Vertices( v0, v1 );
        }
        kvs::OpenGL::Vertices( v1, v2, v2, v3 );
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Z axis.
            kvs::OpenGL::Vertices( v3, v0 );
        }

        kvs::OpenGL::Vertices( v4, v5 );
        kvs::OpenGL::Vertices( v5, v6 );
        kvs::OpenGL::Vertices( v6, v7 );
        kvs::OpenGL::Vertices( v7, v4 );

        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Y axis.
            kvs::OpenGL::Vertices( v0, v4 );
        }
        kvs::OpenGL::Vertices( v1, v5 );
        kvs::OpenGL::Vertices( v2, v6 );
        kvs::OpenGL::Vertices( v3, v7 );
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draws the solid box.
 *  @param  length [in] length of the box's edge
 */
/*===========================================================================*/
void OrientationAxis::draw_solid_box( const float length )
{
    const float x = -length/2.0f;
    const float y = -length/2.0f;
    const float z = -length/2.0f;
    const kvs::Vec3 v0( x, y, z );
    const kvs::Vec3 v1( x + length, y, z );
    const kvs::Vec3 v2( x + length, y, z + length );
    const kvs::Vec3 v3( x, y, z + length );
    const kvs::Vec3 v4( x, y + length, z );
    const kvs::Vec3 v5( x + length, y + length, z );
    const kvs::Vec3 v6( x + length, y + length, z + length );
    const kvs::Vec3 v7( x, y + length, z + length );

    kvs::OpenGL::SetPolygonOffset( 1, 1 );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    kvs::OpenGL::Begin( GL_QUADS );
    kvs::OpenGL::Color( m_box_color );
    kvs::OpenGL::Vertices( v0, v1, v2, v3 ); // bottom
    kvs::OpenGL::Vertices( v7, v6, v5, v4 ); // top
    kvs::OpenGL::Vertices( v0, v4, v5, v1 ); // back
    kvs::OpenGL::Vertices( v1, v5, v6, v2 ); // right
    kvs::OpenGL::Vertices( v2, v6, v7, v3 ); // front
    kvs::OpenGL::Vertices( v0, v3, v7, v4 ); // left
    kvs::OpenGL::End();

    kvs::OpenGL::SetLineWidth( m_box_line_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_box_line_color );
    {
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // X axis.
            kvs::OpenGL::Vertices( v0, v1 );
        }
        kvs::OpenGL::Vertices( v1, v2 );
        kvs::OpenGL::Vertices( v2, v3 );

        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Z axis.
            kvs::OpenGL::Vertices( v3, v0 );
        }
        kvs::OpenGL::Vertices( v4, v5 );
        kvs::OpenGL::Vertices( v5, v6 );
        kvs::OpenGL::Vertices( v6, v7 );
        kvs::OpenGL::Vertices( v7, v4 );

        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Y axis.
            kvs::OpenGL::Vertices( v0, v4 );
        }
        kvs::OpenGL::Vertices( v1, v5 );
        kvs::OpenGL::Vertices( v2, v6 );
        kvs::OpenGL::Vertices( v3, v7 );
    }
    kvs::OpenGL::End();
}

} // end of namesapce glut

} // end of namespace kvs
