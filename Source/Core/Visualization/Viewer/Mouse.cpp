/****************************************************************************/
/**
 *  @file   Mouse.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Mouse.cpp 1713 2014-02-04 05:46:24Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Mouse.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new Mouse class.
 */
/*==========================================================================*/
Mouse::Mouse():
    m_operation_mode( Mouse::Rotation ),
    m_old( kvs::Vec2i::Zero() ),
    m_new( kvs::Vec2i::Zero() ),
    m_start( kvs::Vec2i::Zero() ),
    m_stop( kvs::Vec2i::Zero() ),
    m_timer(),
    m_enable_auto_updating( true ),
    m_is_updating( false ),
    m_is_damping( false )
{
}

/*==========================================================================*/
/**
 *  @brief  Destroys the Mouse class.
 */
/*==========================================================================*/
Mouse::~Mouse()
{
}

/*==========================================================================*/
/**
 *  @brief  Press event function.
 *  @param  x [in] x coordinate value of the mouse pointer position
 *  @param  y [in] y coordinate value of the mouse pointer position
 */
/*==========================================================================*/
void Mouse::press( const int x, const int y )
{
    m_old = kvs::Vec2i( x, y );
    m_new = kvs::Vec2i( x, y );
    m_start = kvs::Vec2i( x, y );

    m_timer.start();
    m_is_updating = false;
    m_is_damping = false;
}

/*==========================================================================*/
/**
 *  @brief  Move event function.
 *  @param  x [in] x coordinate value of the mouse pointer position
 *  @param  y [in] y coordinate value of the mouse pointer position
 */
/*==========================================================================*/
void Mouse::move( const int x, const int y )
{
    m_new = kvs::Vec2i( x, y );

    switch ( m_operation_mode )
    {
    case Mouse::Scaling:
        m_trackball.scale( m_old, m_new );
        break;
    case Mouse::Rotation:
        m_trackball.rotate( m_old, m_new );
        break;
    case Mouse::Translation:
        m_trackball.translate( m_old, m_new );
        break;
    default:
        break;
    }

    // Update the mouse cursor position.
    m_old = m_new;
}

/*==========================================================================*/
/**
 *  @brief  Wheel event function.
 *  @param  value [in] scaling ratio
 */
/*==========================================================================*/
void Mouse::wheel( const float value )
{
    m_trackball.setScaling( kvs::Vec3::Constant( value ) );

    // Update the mouse cursor position.
    m_old = m_new;
}

/*==========================================================================*/
/**
 *  @brief  Release event function.
 *  @param  x [in] x coordinate value of the mouse pointer position
 *  @param  y [in] y coordinate value of the mouse pointer position
 */
/*==========================================================================*/
void Mouse::release( const int x, const int y )
{
    m_timer.stop();
    m_stop = kvs::Vec2i( x, y );

    const double threshould_time = 500.0;
    const double tolerance_length_updating = 2.0;
    const double tolerance_length_damping = 80.0;
    const double length = ( m_start - m_stop ).length();
    if ( m_timer.msec() < threshould_time )
    {
        if ( length > tolerance_length_updating )
        {
            m_is_updating = true;
            m_is_damping = ( length < tolerance_length_damping );
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Idle event function.
 */
/*==========================================================================*/
bool Mouse::idle()
{
    if ( !( m_enable_auto_updating && m_is_updating ) ) { return false; }

    if ( m_is_damping )
    {
        switch ( m_operation_mode )
        {
        case Mouse::Scaling:
        {
            kvs::Vec3 scaling = m_trackball.scaling();
            if ( scaling.x() < 1.0f )
            {
                const float factor = 1.0003f;
                scaling *= factor;
                if ( scaling.x() > 1.0f )
                {
                    scaling = kvs::Vec3::Constant( 1.0f );
                    m_is_damping = false;
                }
            }
            else if ( scaling.x() > 1.0f )
            {
                const float factor = 0.9997f;
                scaling *= factor;
                if ( scaling.x() < 1.0f )
                {
                    scaling = kvs::Vec3::Constant( 1.0f );
                    m_is_damping = false;
                }
            }
            m_trackball.setScaling( scaling );
            break;
        }

        case Mouse::Rotation:
        {
            kvs::Quaternion rotation = m_trackball.rotation();
            const float factor = 0.9f;
            rotation.x() *= factor;
            rotation.y() *= factor;
            rotation.z() *= factor;
            m_trackball.setRotation( rotation );
            break;
        }

        case Mouse::Translation:
        {
            kvs::Vec3 translation = m_trackball.translation();
            const float factor = 0.9f;
            translation *= factor;
            if ( translation.length() < 0.001 )
            {
                translation = kvs::Vec3::Constant( 0.0f );
                m_is_damping = false;
            };
            m_trackball.setTranslation( translation );
            break;
        }
        default:
            break;
        }
    }

    return true;
}

/*==========================================================================*/
/**
 *  @brief Reset parameters.
 */
/*==========================================================================*/
void Mouse::reset()
{
    m_trackball.reset();
    m_operation_mode = kvs::Mouse::Rotation;
    m_old = kvs::Vec2i::Zero();
    m_new = kvs::Vec2i::Zero();
    m_start = kvs::Vec2i::Zero();
    m_stop = kvs::Vec2i::Zero();
    m_enable_auto_updating = true;
    m_is_updating = false;
    m_is_damping = false;
}

} // end of namespace kvs
