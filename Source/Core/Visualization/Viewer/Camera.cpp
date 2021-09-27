/****************************************************************************/
/**
 *  @file   Camera.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Camera.h"
#include <utility>
#include <kvs/OpenGL>
#include <kvs/ColorImage>
#include <kvs/Matrix44>
#include <kvs/ViewingMatrix44>
#include <kvs/FrustumMatrix44>
#include <kvs/PerspectiveMatrix44>
#include <kvs/OrthogonalMatrix44>


namespace
{

const kvs::Xform MakeCameraXform(
    const kvs::Vec3& eye,
    const kvs::Vec3& center,
    const kvs::Vec3& up )
{
    const kvs::Vec3 z = ( eye - center ).normalized();
    const kvs::Vec3 x = up.cross( z ).normalized();
    const kvs::Vec3 y = z.cross( x ).normalized();
    const kvs::Vec3 t = eye;

    const kvs::Mat4 m(
        x[0], y[0], z[0], t[0],
        x[1], y[1], z[1], t[1],
        x[2], y[2], z[2], t[2],
        0,    0,    0,    1 );

    return kvs::Xform( m );
}

}

namespace kvs
{

/*==========================================================================*/
/**
 *  Default constructor.
 *  @param collision [in] collision dectection flag
 */
/*==========================================================================*/
Camera::Camera()
{
    const auto p = kvs::Vec3( 0, 0, 12 ); // initial camera position
    const auto a = kvs::Vec3( 0, 0, 0 ); // initial camera look-at
    const auto u = kvs::Vec3( 0, 1, 0 ); // initial camera up-vector
    this->setPosition( p, a, u );
    this->saveXform();
}

/*==========================================================================*/
/**
 *  Set a window size.
 *  @param width [in] window width
 *  @param height [in] window height
 */
/*==========================================================================*/
void Camera::setWindowSize( const size_t width, const size_t height )
{
    m_window_width  = width;
    m_window_height = height;
}

/*==========================================================================*/
/**
 *  Set a camera position in the world coordinate system.
 *  @param position [in] camera position
 */
/*==========================================================================*/
void Camera::setPosition( const kvs::Vec3& position )
{
    this->setPosition( position, this->lookAt(), this->upVector() );
}

/*===========================================================================*/
/**
 *  @brief  Sets a camera position with a target position in the world coordinates.
 *  @param  position [in] camera position
 *  @param  look_at [in] target position
 */
/*===========================================================================*/
void Camera::setPosition( const kvs::Vec3& position, const kvs::Vec3& look_at )
{
    this->setPosition( position, look_at, this->upVector() );
}

/*===========================================================================*/
/**
 *  @brief  Sets a camera position with a target position in the world coordinates.
 *  @param  position [in] camera position
 *  @param  look_at [in] target position
 *  @param  up [in] camera's up vector
 */
/*===========================================================================*/
void Camera::setPosition( const kvs::Vec3& position, const kvs::Vec3& look_at, const kvs::Vec3& up )
{
    m_transform_center = look_at;
    this->setXform( ::MakeCameraXform( position, look_at, up ) );
}

/*==========================================================================*/
/**
 *  Set a up vector in the world coordinate system.
 *  @param up [in] up vector
 */
/*==========================================================================*/
void Camera::setUpVector( const kvs::Vec3& up_vector )
{
    this->setPosition( this->position(), this->lookAt(), up_vector );
}

/*==========================================================================*/
/**
 *  Set a look-at point in the world coordinate system.
 *  @param at [in] look-at point
 */
/*==========================================================================*/
void Camera::setLookAt( const kvs::Vec3& look_at )
{
    this->setPosition( this->position(), look_at, this->upVector() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the device pixel ratio.
 *  @return device pixel ratio
 */
/*===========================================================================*/
float Camera::devicePixelRatio() const
{
    const kvs::Vec4 vp = kvs::OpenGL::Viewport();
    return vp[2] / m_window_width;
}

/*==========================================================================*/
/**
 *  Get the camera position.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::position() const
{
    return this->xform().translation();
}

/*==========================================================================*/
/**
 *  Get the up vector.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::upVector() const
{
    const kvs::Mat4 m = this->xform().toMatrix();
    return kvs::Vec3( m[0][1], m[1][1], m[2][1] ).normalized();
}

/*==========================================================================*/
/**
 *  Get the look-at point.
 */
/*==========================================================================*/
const kvs::Vec3 Camera::lookAt() const
{
    return m_transform_center;
}

/*===========================================================================*/
/**
 *  @brief  Returns projection matrix.
 *  @return projection matrix
 */
/*===========================================================================*/
const kvs::Mat4 Camera::projectionMatrix() const
{
    const float aspect = static_cast<float>( m_window_width ) / m_window_height;

    if ( m_projection_type == Camera::Perspective )
    {
        return kvs::PerspectiveMatrix44<float>( m_field_of_view, aspect, m_front, m_back );
    }
    else if ( m_projection_type == Camera::Orthogonal )
    {
        // Orthogonal camera mode
        if ( aspect >= 1.0f )
        {
            return kvs::OrthogonalMatrix44<float>(
                m_left * aspect,
                m_right * aspect,
                m_bottom, m_top,
                m_front, m_back );
        }
        else
        {
            return kvs::OrthogonalMatrix44<float>(
                m_left, m_right, 
                m_bottom / aspect, m_top / aspect,
                m_front, m_back );
        }
    }
    else
    {
        // Frustum camera mode
        if ( aspect >= 1.0f )
        {
            return kvs::FrustumMatrix44<float>(
                m_left * aspect, m_right * aspect,
                m_bottom, m_top,
                m_front, m_back );
        }
        else
        {
            return kvs::FrustumMatrix44<float>(
                m_left, m_right,
                m_bottom / aspect, m_top / aspect,
                m_front, m_back );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns viewing matrix.
 *  @return viewing matrix
 */
/*===========================================================================*/
const kvs::Mat4 Camera::viewingMatrix() const
{
    return this->xform().inverse().toMatrix();
}

/*==========================================================================*/
/**
 *  Get a look-at point in device coordinate system.
 *  @return look-at point in the device coordinate system
 */
/*==========================================================================*/
const kvs::Vec2 Camera::lookAtInDevice() const
{
    return kvs::Vec2( m_window_width / 2.0f, m_window_height / 2.0f );
}

/*==========================================================================*/
/**
 *  Update the camera.
 */
/*==========================================================================*/
void Camera::update()
{
    float ary[16];

    kvs::OpenGL::SetMatrixMode( GL_PROJECTION );
    kvs::OpenGL::LoadIdentity();
    kvs::Xform p( this->projectionMatrix() ); p.toArray( ary );
    kvs::OpenGL::MultMatrix( ary );

    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::LoadIdentity();
    kvs::Xform v( this->viewingMatrix() ); v.toArray( ary );
    kvs::OpenGL::MultMatrix( ary );
}

/*==========================================================================*/
/**
 *  Get a snapshot image.
 *  @return snapshot image
 */
/*==========================================================================*/
kvs::ColorImage Camera::snapshot()
{
    const int width = static_cast<int>( m_window_width );
    const int height = static_cast<int>( m_window_height );
    const float dpr = this->devicePixelRatio();

    const int buffer_width = static_cast<int>( width * dpr );
    const int buffer_height = static_cast<int>( height * dpr );
    const int buffer_size = buffer_width * buffer_height * 3;
    kvs::ValueArray<kvs::UInt8> buffer( buffer_size );

    kvs::OpenGL::SetPixelStorageMode( GL_PACK_ALIGNMENT, GLint(1) );
    kvs::OpenGL::SetReadBuffer( GL_FRONT );
    kvs::OpenGL::ReadPixels(
        0, 0, buffer_width, buffer_height,
        GL_RGB, GL_UNSIGNED_BYTE, buffer.data() );

    kvs::ColorImage ret( buffer_width, buffer_height, buffer );
    ret.flip();
    return ret;
}

/*===========================================================================*/
/**
 *  @brief  Resets the xform of the camera.
 */
/*===========================================================================*/
void Camera::resetXform()
{
    kvs::XformControl::resetXform();
    m_transform_center.set( 0, 0, 0 );
}

/*==========================================================================*/
/**
 *  Rotate the camera.
 *  @param rot [in] rotation matrix.
 */
/*==========================================================================*/
void Camera::rotate( const kvs::Mat3& rotation )
{
    const kvs::Vec3 t = m_transform_center;
    const kvs::Xform x =
        kvs::Xform::Translation( t ) *
        kvs::Xform::Rotation( rotation ) *
        kvs::Xform::Translation( -t );
    this->multiplyXform( x );
}

/*==========================================================================*/
/**
 *  Translate the camera.
 *  @param translation [in] translation vector
 */
/*==========================================================================*/
void Camera::translate( const kvs::Vec3& translation )
{
    this->multiplyXform( kvs::Xform::Translation( translation ) );
    m_transform_center += translation;
}

/*==========================================================================*/
/**
 *  Scale the camera.
 *  @param scaling [in] scaling vector
 */
/*==========================================================================*/
void Camera::scale( const kvs::Vec3& scaling )
{
    this->multiplyXform( kvs::Xform::Scaling( scaling ) );
}

} // end of namespace kvs
