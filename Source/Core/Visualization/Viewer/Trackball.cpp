/****************************************************************************/
/**
 *  @file   Trackball.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Trackball.cpp 1717 2014-02-06 09:38:58Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Trackball.h"
#include <kvs/Camera>
#include <kvs/Matrix33>
#include <kvs/Quaternion>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new Trackball class.
 */
/*==========================================================================*/
Trackball::Trackball()
{
    this->reset();
}

/*==========================================================================*/
/**
 *  @brief  Destroys the Trackball class.
 */
/*==========================================================================*/
Trackball::~Trackball()
{
}

/*==========================================================================*/
/**
 *  @brief  Scaling operation.
 *  @param  start [in] start dragging point
 *  @param  end [in] end dragging point
 */
/*==========================================================================*/
void Trackball::scale( const kvs::Vec2i& start, const kvs::Vec2i& end )
{
    const kvs::Vec2 n_old = this->get_norm_position( start );
    const kvs::Vec2 n_new = this->get_norm_position( end );

    const float h = static_cast<float>( m_ref_camera->windowHeight() );
    const float s = 1.0f + m_scaling_factor * ( n_old.y() - n_new.y() ) / h;

    m_scaling = kvs::Vec3::Constant( s );
}

/*==========================================================================*/
/**
 *  @brief  Translation operation.
 *  @param  start [in] start dragging point in the window coordinate system
 *  @param  end [in] end dragging point in the widnow coordinate system
 */
/*==========================================================================*/
void Trackball::translate( const kvs::Vec2i& start, const kvs::Vec2i& end )
{
    const kvs::Vec2i diff = end - start;
    const float w = m_ref_camera->windowWidth();
    const float h = m_ref_camera->windowHeight();
    const kvs::Vec3 trans( diff.x() * 10.0f / w, -diff.y() * 10.0f / h, 0.0f );

    // Transform camera to world.
    m_translation = m_ref_camera->xform().transformNormal( trans );
}

/*==========================================================================*/
/**
 *  @brief  Rotation operation.
 *  @param  start [in] start dragging point in the window coordinate system
 *  @param  end [in] end dragging point in the window coordinate system
 */
/*==========================================================================*/
void Trackball::rotate( const kvs::Vec2i& start, const kvs::Vec2i& end )
{
    if ( start == end )
    {
        m_rotation.set( 0.0, 0.0, 0.0, 1.0 );
        return;
    }

    const kvs::Vec2 n_old( this->get_norm_position( start ) );
    const kvs::Vec2 n_new( this->get_norm_position( end   ) );

    const kvs::Vec3 p1( n_old, this->depth_on_sphere( n_old ) );
    const kvs::Vec3 p2( n_new, this->depth_on_sphere( n_new ) );

    // Transform to world coordinate.
    const kvs::Vec3 p1w = m_ref_camera->xform().transformNormal( p1 );
    const kvs::Vec3 p2w = m_ref_camera->xform().transformNormal( p2 );

    m_rotation = kvs::Quaternion::RotationQuaternion( p1w, p2w );
}

/*==========================================================================*/
/**
 *  @brief  Resets parameters.
 */
/*==========================================================================*/
void Trackball::reset()
{
    m_size = 0.6f;
    m_depth = 1.0f;
    m_scaling_factor = 100.0f;
    m_rotation_center = kvs::Vec2( 0.0f, 0.0f );
    m_scaling = kvs::Vec3( 1.0f, 1.0f, 1.0f );
    m_translation = kvs::Vec3( 0.0f, 0.0f, 0.0f );
    m_rotation = kvs::Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a distance between 'pos' and the virtual sphere.
 *  @param  pos [in] position in the normalized device coordinates
 *  @return distance
 */
/*==========================================================================*/
float Trackball::depth_on_sphere( const kvs::Vec2& pos ) const
{
    const float Sqrt2 = 1.4142135623730950488f;
    const float HalfOfSqrt2 = 0.7071067811865475244f;

    const double r = m_size;
    const double d = pos.length();
    if ( d < r * HalfOfSqrt2 )
    {
        // inside sphere
        return static_cast<float>( std::sqrt( r * r - d * d ) );
    }
    else
    {
        // on hyperbola
        const double t = r / Sqrt2;
        return static_cast<float>( t * t / d );
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns mosue position in the normalized device coordinates.
 *  @param  pos [in] mouse position
 *  @return mouse position
 *
 */
/*==========================================================================*/
kvs::Vec2 Trackball::get_norm_position( const kvs::Vec2i& pos ) const
{
    // Normalized Device coordinates: A center of the normalized device
    // coordinates is a center of the image and the rage is [-1,-1].

    const float w = m_ref_camera->windowWidth();
    const float h = m_ref_camera->windowHeight();
    const float x =  2.0f * ( pos.x() - m_rotation_center.x() ) / w;
    const float y = -2.0f * ( pos.y() - m_rotation_center.y() ) / h;

    return kvs::Vec2( x, y );
}

} // end of namespace kvs
