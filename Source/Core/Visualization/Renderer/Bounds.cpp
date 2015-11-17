/****************************************************************************/
/**
 *  @file   Bounds.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Bounds.cpp 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Bounds.h"
#include <kvs/OpenGL>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/LineObject>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ValueArray>
#include <vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Bounds class.
 */
/*===========================================================================*/
Bounds::Bounds():
    m_type( Bounds::Box ),
    m_line_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_line_width( 1.0f ),
    m_corner_scale( 0.2f ),
    m_division( 50.0f ),
    m_enable_anti_aliasing( false ),
    m_show( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void Bounds::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    if ( !m_show ) return;

    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib p( GL_CURRENT_BIT | GL_ENABLE_BIT );

    // Anti-aliasing.
    if ( m_enable_anti_aliasing )
    {
        kvs::OpenGL::Enable( GL_LINE_SMOOTH );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    {
        KVS_GL_CALL( glColor3ub( m_line_color.r(), m_line_color.g(), m_line_color.b() ) );
        KVS_GL_CALL( glLineWidth( m_line_width ) );

        switch( m_type )
        {
        case Bounds::Box: this->draw_box_bounds( object ); break;
        case Bounds::Corner: this->draw_corner_bounds( object ); break;
        case Bounds::Circle: this->draw_circle_bounds( object ); break;
        default: break;
        }
    }

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Returns a pointer to the line object represents the bounds.
 *  @param  object [in] pointer to the object
 *  @return pointer to the line object
 *  @note   The returned pointer is allocated in this method, but it has not
 *          been deallocated in this class.
 */
/*===========================================================================*/
kvs::LineObject* Bounds::outputLineObject( const kvs::ObjectBase* object ) const
{
    switch( m_type )
    {
    case Bounds::Box: return this->output_box_bounds( object );
    case Bounds::Corner: return this->output_corner_bounds( object );
    case Bounds::Circle: return this->output_circle_bounds( object );
    default: return NULL;
    }
}

/*===========================================================================*/
/**
 *  @brief  Draws a bounding box.
 *  @param  object [in] pointer to the object
 */
/*===========================================================================*/
void Bounds::draw_box_bounds( const kvs::ObjectBase* object )
{
    const kvs::Real32 x_min = object->minObjectCoord().x();
    const kvs::Real32 x_max = object->maxObjectCoord().x();
    const kvs::Real32 y_min = object->minObjectCoord().y();
    const kvs::Real32 y_max = object->maxObjectCoord().y();
    const kvs::Real32 z_min = object->minObjectCoord().z();
    const kvs::Real32 z_max = object->maxObjectCoord().z();

    // Bottom edges.
    KVS_GL_CALL_BEG( glBegin( GL_LINE_LOOP ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_min ) ); // (0)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_min ) ); // (1)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_max ) ); // (2)
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_max ) ); // (3)
    KVS_GL_CALL_END( glEnd() );

    // Top edges.
    KVS_GL_CALL_BEG( glBegin( GL_LINE_LOOP ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_min ) ); // (4)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_min ) ); // (5)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_max ) ); // (6)
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_max ) ); // (7)
    KVS_GL_CALL_END( glEnd() );

    // Vertical edges.
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_min ) ); // (0)
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_min ) ); // (4)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_min ) ); // (1)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_min ) ); // (5)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_max ) ); // (2)
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_max ) ); // (6)
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_max ) ); // (3)
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_max ) ); // (7)
    KVS_GL_CALL_END( glEnd() );
}

/*===========================================================================*/
/**
 *  @brief  Draws a triad corner bounds.
 *  @param  object [in] pointer to the object
 */
/*===========================================================================*/
void Bounds::draw_corner_bounds( const kvs::ObjectBase* object )
{
    const kvs::Real32 x_min = object->minObjectCoord().x();
    const kvs::Real32 x_max = object->maxObjectCoord().x();
    const kvs::Real32 y_min = object->minObjectCoord().y();
    const kvs::Real32 y_max = object->maxObjectCoord().y();
    const kvs::Real32 z_min = object->minObjectCoord().z();
    const kvs::Real32 z_max = object->maxObjectCoord().z();

    const kvs::Real32 x_diff = x_max - x_min;
    const kvs::Real32 y_diff = y_max - y_min;
    const kvs::Real32 z_diff = z_max - z_min;
    const kvs::Real32 corner = kvs::Math::Min( x_diff, y_diff, z_diff ) * m_corner_scale;

    const kvs::Real32 x_max_ext = x_max - corner;
    const kvs::Real32 x_min_ext = x_min + corner;
    const kvs::Real32 y_max_ext = y_max - corner;
    const kvs::Real32 y_min_ext = y_min + corner;
    const kvs::Real32 z_max_ext = z_max - corner;
    const kvs::Real32 z_min_ext = z_min + corner;

    // Corner (0): (x_min, y_min, z_min) - (x_min_ext, y_min_ext, z_min_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_min_ext, y_min, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_min_ext, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_min_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (1): (x_max, y_min, z_min) - (x_max_ext, y_min_ext, z_min_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_max_ext, y_min, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_min_ext, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_min_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (2): (x_max, y_min, z_max) - (x_max_ext, y_min_ext, z_max_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_max_ext, y_min, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_min_ext, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_min, z_max_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (3): (x_min, y_min, z_max) - (x_min_ext, y_min_ext, z_max_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_min_ext, y_min, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_min_ext, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_min, z_max_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (4): (x_min, y_max, z_min) - (x_min_ext, y_max_ext, z_min_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_min_ext, y_max, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_max_ext, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_min_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (5): (x_max, y_max, z_min) - (x_max_ext, y_max_ext, z_min_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_max_ext, y_max, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_max_ext, z_min ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_min ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_min_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (6): (x_max, y_max, z_max) - (x_max_ext, y_max_ext, z_max_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_max_ext, y_max, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_max_ext, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_max, y_max, z_max_ext ) );
    KVS_GL_CALL_END( glEnd() );

    // Corner (7): (x_min, y_max, z_max) - (x_min_ext, y_max_ext, z_max_ext)
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_min_ext, y_max, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_max_ext, z_max ) );
    KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_max ) ); KVS_GL_CALL_VER( glVertex3f( x_min, y_max, z_max_ext ) );
    KVS_GL_CALL_END( glEnd() );
}

/*===========================================================================*/
/**
 *  @brief  Draws a bounding circle.
 *  @param  object [in] pointer to the object
 */
/*===========================================================================*/
void Bounds::draw_circle_bounds( const kvs::ObjectBase* object )
{
    const kvs::Real32 x_min = object->minObjectCoord().x();
    const kvs::Real32 x_max = object->maxObjectCoord().x();
    const kvs::Real32 y_min = object->minObjectCoord().y();
    const kvs::Real32 y_max = object->maxObjectCoord().y();
    const kvs::Real32 z_min = object->minObjectCoord().z();
    const kvs::Real32 z_max = object->maxObjectCoord().z();

    const kvs::Real32 x_diff = x_max - x_min;
    const kvs::Real32 y_diff = y_max - y_min;
    const kvs::Real32 z_diff = z_max - z_min;

    const kvs::Real32 x_center = ( x_max + x_min ) * 0.5f;
    const kvs::Real32 y_center = ( y_max + y_min ) * 0.5f;
    const kvs::Real32 z_center = ( z_max + z_min ) * 0.5f;

    const kvs::Real32 pi = static_cast<kvs::Real32>( kvs::Math::PI() );
    const kvs::Real32 diff_angle = ( 2.0f * pi ) / m_division;

    const kvs::Real32 x1 = x_diff * 0.5f;
    const kvs::Real32 y1 = y_diff * 0.5f;
    const kvs::Real32 z1 = z_diff * 0.5f;

    const kvs::Real32 sqrt2 = std::sqrt( 2.0f );
    const kvs::Real32 a = sqrt2 * x1;  const kvs::Real32 a2 = a * a;
    const kvs::Real32 b = sqrt2 * y1;  const kvs::Real32 b2 = b * b;
    const kvs::Real32 c = sqrt2 * z1;  const kvs::Real32 c2 = c * c;

    const size_t division = static_cast<size_t>( m_division );

    // Circle (0) on XY plane
    KVS_GL_CALL_BEG( glBegin( GL_LINE_LOOP ) );
    const kvs::Real32 a2b2 = a2 * b2;
    for ( size_t i = 0; i <= division; i++ )
    {
        const kvs::Real32 angle = diff_angle * i;
        const kvs::Real32 sin = std::sin( angle );
        const kvs::Real32 cos = std::cos( angle );
        const kvs::Real32 sin2 = sin * sin;
        const kvs::Real32 cos2 = cos * cos;

        const kvs::Real32 r = std::sqrt( a2b2 / ( a2 * sin2 + b2 * cos2 ) );
        const kvs::Real32 x = r * cos + x_center;
        const kvs::Real32 y = r * sin + y_center;
        const kvs::Real32 z = z_center;
        KVS_GL_CALL_VER( glVertex3f( x, y, z ) );
    }
    KVS_GL_CALL_END( glEnd() );

    // Circle (1) on YZ plane
    KVS_GL_CALL_BEG( glBegin( GL_LINE_LOOP ) );
    const float b2c2 = b2 * c2;
    for ( size_t i = 0; i <= division; i++ )
    {
        const kvs::Real32 angle = diff_angle * i;
        const kvs::Real32 sin = std::sin( angle );
        const kvs::Real32 cos = std::cos( angle );
        const kvs::Real32 sin2 = sin * sin;
        const kvs::Real32 cos2 = cos * cos;

        const kvs::Real32 r = std::sqrt( b2c2 / ( b2 * sin2 + c2 * cos2 ) );
        const kvs::Real32 x = x_center;
        const kvs::Real32 y = r * cos + y_center;
        const kvs::Real32 z = r * sin + z_center;
        KVS_GL_CALL_VER( glVertex3f( x, y, z ) );
    }
    KVS_GL_CALL_END( glEnd() );

    // Circle (2) on ZX plane
    KVS_GL_CALL_BEG( glBegin( GL_LINE_LOOP ) );
    const kvs::Real32 c2a2 = c2 * a2;
    for ( size_t i = 0; i <= division; i++ )
    {
        const kvs::Real32 angle = diff_angle * i;
        const kvs::Real32 sin = std::sin( angle );
        const kvs::Real32 cos = std::cos( angle );
        const kvs::Real32 sin2 = sin * sin;
        const kvs::Real32 cos2 = cos * cos;

        const kvs::Real32 r = std::sqrt( c2a2 / ( c2 * sin2 + a2 * cos2 ) );
        const kvs::Real32 x = r * sin + x_center;
        const kvs::Real32 y = y_center;
        const kvs::Real32 z = r * cos + z_center;
        KVS_GL_CALL_VER( glVertex3f( x, y, z ) );
    }
    KVS_GL_CALL_END( glEnd() );
}

/*===========================================================================*/
/**
 *  @brief  Outputs a box bounds as a line object.
 *  @param  object [in] pointer to the object
 *  @return pointer to the line object
 *  @note   The returned pointer is allocated in this method, but it has not
 *          been deallocated in this class.
 */
/*===========================================================================*/
kvs::LineObject* Bounds::output_box_bounds( const kvs::ObjectBase* object ) const
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;

    const kvs::Real32 x_min = object->minObjectCoord().x();
    const kvs::Real32 x_max = object->maxObjectCoord().x();
    const kvs::Real32 y_min = object->minObjectCoord().y();
    const kvs::Real32 y_max = object->maxObjectCoord().y();
    const kvs::Real32 z_min = object->minObjectCoord().z();
    const kvs::Real32 z_max = object->maxObjectCoord().z();

    // Coordinate values.
    coords.push_back( x_min ); coords.push_back( y_min ); coords.push_back( z_min ); // (0)
    coords.push_back( x_max ); coords.push_back( y_min ); coords.push_back( z_min ); // (1)
    coords.push_back( x_max ); coords.push_back( y_min ); coords.push_back( z_max ); // (2)
    coords.push_back( x_min ); coords.push_back( y_min ); coords.push_back( z_max ); // (3)
    coords.push_back( x_min ); coords.push_back( y_max ); coords.push_back( z_min ); // (4)
    coords.push_back( x_max ); coords.push_back( y_max ); coords.push_back( z_min ); // (5)
    coords.push_back( x_max ); coords.push_back( y_max ); coords.push_back( z_max ); // (6)
    coords.push_back( x_min ); coords.push_back( y_max ); coords.push_back( z_max ); // (7)

    // Bottom edges.
    connects.push_back( 0 ); connects.push_back( 1 );
    connects.push_back( 1 ); connects.push_back( 2 );
    connects.push_back( 2 ); connects.push_back( 3 );
    connects.push_back( 3 ); connects.push_back( 0 );

    // Top edges.
    connects.push_back( 4 ); connects.push_back( 5 );
    connects.push_back( 5 ); connects.push_back( 6 );
    connects.push_back( 6 ); connects.push_back( 7 );
    connects.push_back( 7 ); connects.push_back( 4 );

    // Vertical edges.
    connects.push_back( 0 ); connects.push_back( 4 );
    connects.push_back( 1 ); connects.push_back( 5 );
    connects.push_back( 2 ); connects.push_back( 6 );
    connects.push_back( 3 ); connects.push_back( 7 );

    kvs::LineObject* line_object = new kvs::LineObject();
    line_object->setMinMaxObjectCoords( object->minObjectCoord(), object->maxObjectCoord() );
    line_object->setMinMaxExternalCoords( object->minExternalCoord(), object->maxExternalCoord() );
    line_object->setLineType( kvs::LineObject::Segment );
    line_object->setColorType( kvs::LineObject::LineColor );
    line_object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    line_object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    line_object->setColor( m_line_color );
    line_object->setSize( m_line_width );

    return line_object;
}

/*===========================================================================*/
/**
 *  @brief  Outputs a corner bounds as a line object.
 *  @param  object [in] pointer to the object
 *  @return pointer to the line object
 *  @note   The returned pointer is allocated in this method, but it has not
 *          been deallocated in this class.
 */
/*===========================================================================*/
kvs::LineObject* Bounds::output_corner_bounds( const kvs::ObjectBase* object ) const
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;

    const kvs::Real32 x_min = object->minObjectCoord().x();
    const kvs::Real32 x_max = object->maxObjectCoord().x();
    const kvs::Real32 y_min = object->minObjectCoord().y();
    const kvs::Real32 y_max = object->maxObjectCoord().y();
    const kvs::Real32 z_min = object->minObjectCoord().z();
    const kvs::Real32 z_max = object->maxObjectCoord().z();

    const kvs::Real32 x_diff = x_max - x_min;
    const kvs::Real32 y_diff = y_max - y_min;
    const kvs::Real32 z_diff = z_max - z_min;
    const kvs::Real32 corner = kvs::Math::Min( x_diff, y_diff, z_diff ) * m_corner_scale;

    const kvs::Real32 x_max_ext = x_max - corner;
    const kvs::Real32 x_min_ext = x_min + corner;
    const kvs::Real32 y_max_ext = y_max - corner;
    const kvs::Real32 y_min_ext = y_min + corner;
    const kvs::Real32 z_max_ext = z_max - corner;
    const kvs::Real32 z_min_ext = z_min + corner;

    // Corner (0): (x_min, y_min, z_min) - (x_min_ext, y_min_ext, z_min_ext)
    coords.push_back( x_min ); coords.push_back( y_min ); coords.push_back( z_min ); // (0)
    coords.push_back( x_min_ext ); coords.push_back( y_min ); coords.push_back( z_min ); // (1)
    coords.push_back( x_min ); coords.push_back( y_min_ext ); coords.push_back( z_min ); // (2)
    coords.push_back( x_min ); coords.push_back( y_min ); coords.push_back( z_min_ext ); // (3)
    connects.push_back( 0 ); connects.push_back( 1 );
    connects.push_back( 0 ); connects.push_back( 2 );
    connects.push_back( 0 ); connects.push_back( 3 );

    // Corner (1): (x_max, y_min, z_min) - (x_max_ext, y_min_ext, z_min_ext)
    coords.push_back( x_max ); coords.push_back( y_min ); coords.push_back( z_min ); // (4)
    coords.push_back( x_max_ext ); coords.push_back( y_min ); coords.push_back( z_min ); // (5)
    coords.push_back( x_max ); coords.push_back( y_min_ext ); coords.push_back( z_min ); // (6)
    coords.push_back( x_max ); coords.push_back( y_min ); coords.push_back( z_min_ext ); // (7)
    connects.push_back( 4 ); connects.push_back( 5 );
    connects.push_back( 4 ); connects.push_back( 6 );
    connects.push_back( 4 ); connects.push_back( 7 );

    // Corner (2): (x_max, y_min, z_max) - (x_max_ext, y_min_ext, z_max_ext)
    coords.push_back( x_max ); coords.push_back( y_min ); coords.push_back( z_max ); // (8)
    coords.push_back( x_max_ext ); coords.push_back( y_min ); coords.push_back( z_max ); // (9)
    coords.push_back( x_max ); coords.push_back( y_min_ext ); coords.push_back( z_max ); // (10)
    coords.push_back( x_max ); coords.push_back( y_min ); coords.push_back( z_max_ext ); // (11)
    connects.push_back( 8 ); connects.push_back( 9 );
    connects.push_back( 8 ); connects.push_back( 10 );
    connects.push_back( 8 ); connects.push_back( 11 );

    // Corner (3): (x_min, y_min, z_max) - (x_min_ext, y_min_ext, z_max_ext)
    coords.push_back( x_min ); coords.push_back( y_min ); coords.push_back( z_max ); // (12)
    coords.push_back( x_min_ext ); coords.push_back( y_min ); coords.push_back( z_max ); // (13)
    coords.push_back( x_min ); coords.push_back( y_min_ext ); coords.push_back( z_max ); // (14)
    coords.push_back( x_min ); coords.push_back( y_min ); coords.push_back( z_max_ext ); // (15)
    connects.push_back( 12 ); connects.push_back( 13 );
    connects.push_back( 12 ); connects.push_back( 14 );
    connects.push_back( 12 ); connects.push_back( 15 );

    // Corner (4): (x_min, y_max, z_min) - (x_min_ext, y_max_ext, z_min_ext)
    coords.push_back( x_min ); coords.push_back( y_max ); coords.push_back( z_min ); // (16)
    coords.push_back( x_min_ext ); coords.push_back( y_max ); coords.push_back( z_min ); // (17)
    coords.push_back( x_min ); coords.push_back( y_max_ext ); coords.push_back( z_min ); // (18)
    coords.push_back( x_min ); coords.push_back( y_max ); coords.push_back( z_min_ext ); // (19)
    connects.push_back( 16 ); connects.push_back( 17 );
    connects.push_back( 16 ); connects.push_back( 18 );
    connects.push_back( 16 ); connects.push_back( 19 );

    // Corner (5): (x_max, y_max, z_min) - (x_max_ext, y_max_ext, z_min_ext)
    coords.push_back( x_max ); coords.push_back( y_max ); coords.push_back( z_min ); // (20)
    coords.push_back( x_max_ext ); coords.push_back( y_max ); coords.push_back( z_min ); // (21)
    coords.push_back( x_max ); coords.push_back( y_max_ext ); coords.push_back( z_min ); // (22)
    coords.push_back( x_max ); coords.push_back( y_max ); coords.push_back( z_min_ext ); // (23)
    connects.push_back( 20 ); connects.push_back( 21 );
    connects.push_back( 20 ); connects.push_back( 22 );
    connects.push_back( 20 ); connects.push_back( 23 );

    // Corner (6): (x_max, y_max, z_max) - (x_max_ext, y_max_ext, z_max_ext)
    coords.push_back( x_max ); coords.push_back( y_max ); coords.push_back( z_max ); // (24)
    coords.push_back( x_max_ext ); coords.push_back( y_max ); coords.push_back( z_max ); // (25)
    coords.push_back( x_max ); coords.push_back( y_max_ext ); coords.push_back( z_max ); // (26)
    coords.push_back( x_max ); coords.push_back( y_max ); coords.push_back( z_max_ext ); // (27)
    connects.push_back( 24 ); connects.push_back( 25 );
    connects.push_back( 24 ); connects.push_back( 26 );
    connects.push_back( 24 ); connects.push_back( 27 );

    // Corner (7): (x_min, y_max, z_max) - (x_min_ext, y_max_ext, z_max_ext)
    coords.push_back( x_min ); coords.push_back( y_max ); coords.push_back( z_max ); // (28)
    coords.push_back( x_min_ext ); coords.push_back( y_max ); coords.push_back( z_max ); // (29)
    coords.push_back( x_min ); coords.push_back( y_max_ext ); coords.push_back( z_max ); // (30)
    coords.push_back( x_min ); coords.push_back( y_max ); coords.push_back( z_max_ext ); // (31)
    connects.push_back( 28 ); connects.push_back( 29 );
    connects.push_back( 28 ); connects.push_back( 30 );
    connects.push_back( 28 ); connects.push_back( 31 );

    kvs::LineObject* line_object = new kvs::LineObject();
    line_object->setMinMaxObjectCoords( object->minObjectCoord(), object->maxObjectCoord() );
    line_object->setMinMaxExternalCoords( object->minExternalCoord(), object->maxExternalCoord() );
    line_object->setLineType( kvs::LineObject::Segment );
    line_object->setColorType( kvs::LineObject::LineColor );
    line_object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    line_object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    line_object->setColor( m_line_color );
    line_object->setSize( m_line_width );

    return line_object;
}

/*===========================================================================*/
/**
 *  @brief  Outputs a circle bounds as a line object.
 *  @param  object [in] pointer to the object
 *  @return pointer to the line object
 *  @note   The returned pointer is allocated in this method, but it has not
 *          been deallocated in this class.
 */
/*===========================================================================*/
kvs::LineObject* Bounds::output_circle_bounds( const kvs::ObjectBase* object ) const
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;

    const kvs::Real32 x_min = object->minObjectCoord().x();
    const kvs::Real32 x_max = object->maxObjectCoord().x();
    const kvs::Real32 y_min = object->minObjectCoord().y();
    const kvs::Real32 y_max = object->maxObjectCoord().y();
    const kvs::Real32 z_min = object->minObjectCoord().z();
    const kvs::Real32 z_max = object->maxObjectCoord().z();

    const kvs::Real32 x_diff = x_max - x_min;
    const kvs::Real32 y_diff = y_max - y_min;
    const kvs::Real32 z_diff = z_max - z_min;

    const kvs::Real32 x_center = ( x_max + x_min ) * 0.5f;
    const kvs::Real32 y_center = ( y_max + y_min ) * 0.5f;
    const kvs::Real32 z_center = ( z_max + z_min ) * 0.5f;

    const kvs::Real32 diff_angle = ( 2.0f * kvs::Math::PI() ) / m_division;

    const kvs::Real32 x1 = x_diff * 0.5f;
    const kvs::Real32 y1 = y_diff * 0.5f;
    const kvs::Real32 z1 = z_diff * 0.5f;

    const kvs::Real32 sqrt2 = std::sqrt( 2.0f );
    const kvs::Real32 a = sqrt2 * x1;  const kvs::Real32 a2 = a * a;
    const kvs::Real32 b = sqrt2 * y1;  const kvs::Real32 b2 = b * b;
    const kvs::Real32 c = sqrt2 * z1;  const kvs::Real32 c2 = c * c;

    const size_t division = static_cast<size_t>( m_division );

    kvs::UInt32 vertex_id = 0;

    // Circle (0) on XY plane
    const kvs::Real32 a2b2 = a2 * b2;
    for ( size_t i = 0; i <= division; i++ )
    {
        const kvs::Real32 angle = diff_angle * i;
        const kvs::Real32 sin = std::sin( angle );
        const kvs::Real32 cos = std::cos( angle );
        const kvs::Real32 sin2 = sin * sin;
        const kvs::Real32 cos2 = cos * cos;

        const kvs::Real32 r = std::sqrt( a2b2 / ( a2 * sin2 + b2 * cos2 ) );
        const kvs::Real32 x = r * cos + x_center;
        const kvs::Real32 y = r * sin + y_center;
        const kvs::Real32 z = z_center;
        coords.push_back( x ); coords.push_back( y ); coords.push_back( z );
    }

    connects.push_back( vertex_id );
    vertex_id = coords.size() / 3 - 1;
    connects.push_back( vertex_id );
    vertex_id++;

    // Circle (1) on YZ plane
    const float b2c2 = b2 * c2;
    for ( size_t i = 0; i <= division; i++ )
    {
        const kvs::Real32 angle = diff_angle * i;
        const kvs::Real32 sin = std::sin( angle );
        const kvs::Real32 cos = std::cos( angle );
        const kvs::Real32 sin2 = sin * sin;
        const kvs::Real32 cos2 = cos * cos;

        const kvs::Real32 r = std::sqrt( b2c2 / ( b2 * sin2 + c2 * cos2 ) );
        const kvs::Real32 x = x_center;
        const kvs::Real32 y = r * cos + y_center;
        const kvs::Real32 z = r * sin + z_center;
        coords.push_back( x ); coords.push_back( y ); coords.push_back( z );
    }

    connects.push_back( vertex_id );
    vertex_id = coords.size() / 3 - 1;
    connects.push_back( vertex_id );
    vertex_id++;

    // Circle (2) on ZX plane
    const kvs::Real32 c2a2 = c2 * a2;
    for ( size_t i = 0; i <= division; i++ )
    {
        const kvs::Real32 angle = diff_angle * i;
        const kvs::Real32 sin = std::sin( angle );
        const kvs::Real32 cos = std::cos( angle );
        const kvs::Real32 sin2 = sin * sin;
        const kvs::Real32 cos2 = cos * cos;

        const kvs::Real32 r = std::sqrt( c2a2 / ( c2 * sin2 + a2 * cos2 ) );
        const kvs::Real32 x = r * sin + x_center;
        const kvs::Real32 y = y_center;
        const kvs::Real32 z = r * cos + z_center;
        coords.push_back( x ); coords.push_back( y ); coords.push_back( z );
    }

    connects.push_back( vertex_id );
    vertex_id = coords.size() / 3 - 1;
    connects.push_back( vertex_id );

    kvs::LineObject* line_object = new kvs::LineObject();
    line_object->setMinMaxObjectCoords( object->minObjectCoord(), object->maxObjectCoord() );
    line_object->setMinMaxExternalCoords( object->minExternalCoord(), object->maxExternalCoord() );
    line_object->setLineType( kvs::LineObject::Polyline );
    line_object->setColorType( kvs::LineObject::LineColor );
    line_object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    line_object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    line_object->setColor( m_line_color );
    line_object->setSize( m_line_width );

    return line_object;
}

} // end of namespace kvs
