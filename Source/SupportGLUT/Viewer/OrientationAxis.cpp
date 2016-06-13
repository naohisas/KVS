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


// Default parameters.
namespace { namespace Default
{
const size_t AxisWindowSize = 80;
} }


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

//    GLint vp[4]; glGetIntegerv( GL_VIEWPORT, vp );
    GLint vp[4];
    kvs::OpenGL::GetViewport( &(vp[0]) );

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
//    glMatrixMode( GL_PROJECTION ); glLoadIdentity();
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
//            gluPerspective( field_of_view, aspect, front, back );
            kvs::OpenGL::SetPerspective( field_of_view, aspect, front, back );
        }
        else
        {
            // Orthogonal projection.
            const float left = -5.0f;
            const float bottom = -5.0f;
            const float right = 5.0f;
            const float top = 5.0f;
//            glOrtho( left, right, bottom, top, front, back );
            kvs::OpenGL::SetOrtho( left, right, bottom, top, front, back );
        }
    }

    // The origin is set to the top-left on the screen.
    const int x = m_x + m_margin;
    const int y = screen()->height() - m_y - m_height + m_margin;
    const int width = m_width - m_margin;
    const int height = m_height - m_margin;
//    glViewport( x, y, width, height);
    kvs::OpenGL::SetViewport( x, y, width, height);
//    glMatrixMode( GL_MODELVIEW ); glLoadIdentity();
    kvs::OpenGL::WithPushedMatrix p2( GL_MODELVIEW );
    p2.loadIdentity();
    {
        // Viewing transformation.
        const kvs::Vec3 eye( 0.0f, 0.0f, 12.0f );
        const kvs::Vec3 center( 0.0f, 0.0f, 0.0f );
        const kvs::Vec3 up( 0.0f, 1.0f, 0.0f );
        kvs::OpenGL::SetLookAt( eye.x(), eye.y(), eye.z(),
                                center.x(), center.y(), center.z(),
                                up.x(), up.y(), up.z() );
//        gluLookAt( eye.x(), eye.y(), eye.z(),
//                   center.x(), center.y(), center.z(),
//                   up.x(), up.y(), up.z() );

        // Rotate the axis and the box using the object's rotation matrix.
        kvs::Xform xform = m_object->xform();
        const kvs::Vector3f trans = xform.translation();
        const kvs::Vector3f scale = xform.scaling();
        const float max_scale = kvs::Math::Max( scale.x(), scale.y(), scale.z() );
        xform = kvs::Xform::Translation( -trans ) * xform; // Remove translation.
        xform = kvs::Xform::Scaling( 1.0f / max_scale ) * xform; // Normalize by maximum scale.

        float mat[16];
        xform.toArray( mat );
//        glMultMatrixf( mat );
        kvs::OpenGL::MultMatrix( &(mat[0]) );

        // Fixed length of the axis
        const float length = 4.0f;

        // Draw the box.
        switch ( m_box_type )
        {
        case OrientationAxis::WiredBox:
            this->draw_wired_box( length );
            break;
        case OrientationAxis::SolidBox:
            this->draw_solid_box( length );
            break;
        default:
            break;
        }

        // Draw the axis.
        switch ( m_axis_type )
        {
        case OrientationAxis::CorneredAxis:
            this->draw_cornered_axis( length );
            break;
        case OrientationAxis::CenteredAxis:
            this->draw_centered_axis( length );
            break;
        default:
            break;
        }
    }

//    glViewport( vp[0], vp[1], vp[2], vp[3]);
    kvs::OpenGL::SetViewport( vp[0], vp[1], vp[2], vp[3]);
//    glPopAttrib();
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
int OrientationAxis::get_fitted_width()
{
    return ::Default::AxisWindowSize;
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen height.
 *  @return screen height
 */
/*===========================================================================*/
int OrientationAxis::get_fitted_height()
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
    const kvs::Vector3f v0( x, y, z );
    const kvs::Vector3f v1( x + length, y, z );
    //const kvs::Vector3f v2( x + length, y, z + length );
    const kvs::Vector3f v3( x, y, z + length );
    const kvs::Vector3f v4( x, y + length, z );
    //const kvs::Vector3f v5( x + length, y + length, z );
    //const kvs::Vector3f v6( x + length, y + length, z + length );
    //const kvs::Vector3f v7( x, y + length, z + length );

    KVS_GL_CALL( glLineWidth( m_axis_line_width ) );

    const float padding = static_cast<float>( length ) / 4.0f;

    // X-axis.
    KVS_GL_CALL_VER( glColor3ub( m_x_axis_color.r(), m_x_axis_color.g(), m_x_axis_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
    KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
    KVS_GL_CALL_END( glEnd() );

//    glRasterPos3d( v1.x(), v1.y() - padding, v1.z() - padding );
    kvs::OpenGL::SetRasterPos( v1.x(), v1.y() - padding, v1.z() - padding );
    char* x_char = const_cast<char*>( m_x_tag.c_str() );
    for( char* p = x_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }

    // Y-axis.
    KVS_GL_CALL_VER( glColor3ub( m_y_axis_color.r(), m_y_axis_color.g(), m_y_axis_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
    KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
    KVS_GL_CALL_END( glEnd() );

    kvs::OpenGL::SetRasterPos( v4.x() - padding, v4.y(), v4.z() - padding );
    char* y_char = const_cast<char*>( m_y_tag.c_str() );
    for( char* p = y_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }

    // Z-axis.
    KVS_GL_CALL_VER( glColor3ub( m_z_axis_color.r(), m_z_axis_color.g(), m_z_axis_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
    KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
    KVS_GL_CALL_END( glEnd() );

    kvs::OpenGL::SetRasterPos( v3.x() - padding, v3.y() - padding, v3.z() );
    char* z_char = const_cast<char*>( m_z_tag.c_str() );
    for( char* p = z_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }
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
    const kvs::Vector3f v0( x, y, z );
    const kvs::Vector3f v1( x + length, y, z );
    //const kvs::Vector3f v2( x + length, y, z + length );
    const kvs::Vector3f v3( x, y, z + length );
    const kvs::Vector3f v4( x, y + length, z );
    //const kvs::Vector3f v5( x + length, y + length, z );
    //const kvs::Vector3f v6( x + length, y + length, z + length );
    //const kvs::Vector3f v7( x, y + length, z + length );

    KVS_GL_CALL( glLineWidth( m_axis_line_width ) );

    const float padding = static_cast<float>( length ) / 6.0f;

    // X-axis.
    KVS_GL_CALL( glColor3ub( m_x_axis_color.r(), m_x_axis_color.g(), m_x_axis_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
    KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
    KVS_GL_CALL_END( glEnd() );

    kvs::OpenGL::SetRasterPos( v1.x(), v1.y() - padding, v1.z() - padding );
    char* x_char = const_cast<char*>( m_x_tag.c_str() );
    for( char* p = x_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }

    // Y-axis.
    KVS_GL_CALL_VER( glColor3ub( m_y_axis_color.r(), m_y_axis_color.g(), m_y_axis_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
    KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
    KVS_GL_CALL_END( glEnd() );

    kvs::OpenGL::SetRasterPos( v4.x() - padding, v4.y(), v4.z() - padding );
    char* y_char = const_cast<char*>( m_y_tag.c_str() );
    for( char* p = y_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }

    // Z-axis.
    KVS_GL_CALL_VER( glColor3ub( m_z_axis_color.r(), m_z_axis_color.g(), m_z_axis_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
    KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
    KVS_GL_CALL_END( glEnd() );

    kvs::OpenGL::SetRasterPos( v3.x() - padding, v3.y() - padding, v3.z() );
    char* z_char = const_cast<char*>( m_z_tag.c_str() );
    for( char* p = z_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }
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
    const kvs::Vector3f v0( x, y, z );
    const kvs::Vector3f v1( x + length, y, z );
    const kvs::Vector3f v2( x + length, y, z + length );
    const kvs::Vector3f v3( x, y, z + length );
    const kvs::Vector3f v4( x, y + length, z );
    const kvs::Vector3f v5( x + length, y + length, z );
    const kvs::Vector3f v6( x + length, y + length, z + length );
    const kvs::Vector3f v7( x, y + length, z + length );

    KVS_GL_CALL( glLineWidth( m_box_line_width ) );
    KVS_GL_CALL_VER( glColor3ub( m_box_line_color.r(), m_box_line_color.g(), m_box_line_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    {
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // X axis.
            KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
            KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        }
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Z axis.
            KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
            KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
        }

        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );

        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Y axis.
            KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
            KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
        }
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );

        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );

        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
    }
    KVS_GL_CALL_END( glEnd() );
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
    const kvs::Vector3f v0( x, y, z );
    const kvs::Vector3f v1( x + length, y, z );
    const kvs::Vector3f v2( x + length, y, z + length );
    const kvs::Vector3f v3( x, y, z + length );
    const kvs::Vector3f v4( x, y + length, z );
    const kvs::Vector3f v5( x + length, y + length, z );
    const kvs::Vector3f v6( x + length, y + length, z + length );
    const kvs::Vector3f v7( x, y + length, z + length );

    kvs::OpenGL::SetPolygonOffset( 1, 1 );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    KVS_GL_CALL_VER( glColor3ub( m_box_color.r(), m_box_color.g(), m_box_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_QUADS ) );
    {
        // bottom
        KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        // top
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
        // back
        KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        // right
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        // front
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        // left
        KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
    }
    KVS_GL_CALL_END( glEnd() );

    KVS_GL_CALL( glLineWidth( m_box_line_width ) );
    KVS_GL_CALL_VER( glColor3ub( m_box_line_color.r(), m_box_line_color.g(), m_box_line_color.b() ) );
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    {
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // X axis.
            KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
            KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        }
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Z axis.
            KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
            KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
        }

        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );

        if ( m_axis_type != OrientationAxis::CorneredAxis )
        {
            // Y axis.
            KVS_GL_CALL_VER( glVertex3f( v0.x(), v0.y(), v0.z() ) );
            KVS_GL_CALL_VER( glVertex3f( v4.x(), v4.y(), v4.z() ) );
        }
        KVS_GL_CALL_VER( glVertex3f( v1.x(), v1.y(), v1.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v5.x(), v5.y(), v5.z() ) );

        KVS_GL_CALL_VER( glVertex3f( v2.x(), v2.y(), v2.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v6.x(), v6.y(), v6.z() ) );

        KVS_GL_CALL_VER( glVertex3f( v3.x(), v3.y(), v3.z() ) );
        KVS_GL_CALL_VER( glVertex3f( v7.x(), v7.y(), v7.z() ) );
    }
    KVS_GL_CALL_END( glEnd() );

    kvs::OpenGL::Disable( GL_POLYGON_OFFSET_FILL );
}

} // end of namesapce glut

} // end of namespace kvs
