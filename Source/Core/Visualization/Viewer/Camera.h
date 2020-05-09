/****************************************************************************/
/**
 *  @file   Camera.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Camera.h 1791 2014-07-24 06:51:25Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <kvs/XformControl>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Matrix33>
#include <kvs/ColorImage>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Camera class
 */
/*==========================================================================*/
class Camera : public kvs::XformControl
{
public:
    enum ProjectionType
    {
        Perspective = 0,
        Orthogonal = 1,
        Frustum
    };

private:
    kvs::Vec3 m_transform_center; // transform center
    ProjectionType m_projection_type; ///< projection type
    float m_field_of_view; ///< field of view [deg]
    float m_front; ///< front plane position
    float m_back; ///< back plane position
    float m_left; ///< left plane position
    float m_right; ///< right plane position
    float m_bottom; ///< bottom plane position
    float m_top; ///< top plane position
    size_t m_window_width; ///< window width
    size_t m_window_height; ///< window height

public:
    Camera();
    virtual ~Camera();

    void setProjectionType( const ProjectionType type ) { m_projection_type = type; }
    void setProjectionTypeToPerspective() { this->setProjectionType( Perspective ); }
    void setProjectionTypeToOrthogonal() { this->setProjectionType( Orthogonal ); }
    void setProjectionTypeToFrustum() { this->setProjectionType( Frustum ); }
    void setFieldOfView( const float fov ) { m_field_of_view = fov; }
    void setBack( const float back ) { m_back = back; }
    void setFront( const float front ) { m_front = front; }
    void setLeft( const float left ) { m_left = left; }
    void setRight( const float right ) { m_right = right; }
    void setBottom( const float bottom ) { m_bottom = bottom; }
    void setTop( const float top ) { m_top = top; }
    void setWindowSize( const size_t width, const size_t height );
    void setPosition( const kvs::Vec3& position );
    void setPosition( const kvs::Vec3& position, const kvs::Vec3& look_at );
    void setPosition( const kvs::Vec3& position, const kvs::Vec3& look_at, const kvs::Vec3& up );
    void setUpVector( const kvs::Vec3& up_vector );
    void setLookAt( const kvs::Vec3& look_at );

    bool isPerspective() const { return m_projection_type == Perspective; }
    ProjectionType projectionType() const { return m_projection_type; }
    float fieldOfView() const { return m_field_of_view; }
    float back() const { return m_back; }
    float front() const { return m_front; }
    float left() const { return m_left; }
    float right() const { return m_right; }
    float bottom() const { return m_bottom; }
    float top() const { return m_top; }
    size_t windowWidth() const { return m_window_width; }
    size_t windowHeight() const { return m_window_height; }
    float devicePixelRatio() const;
    const kvs::Vec3 position() const;
    const kvs::Vec3 upVector() const;
    const kvs::Vec3 lookAt() const;
    const kvs::Mat4 projectionMatrix() const;
    const kvs::Mat4 viewingMatrix() const;
    const kvs::Vec2 lookAtInDevice() const;

    virtual void initialize();
    virtual void update();
    virtual kvs::ColorImage snapshot();

    void resetXform();
    void rotate( const kvs::Mat3& rotation );
    void translate( const kvs::Vec3& translation );
    void scale( const kvs::Vec3& scaling );

public:

//deprecated unused
    KVS_DEPRECATED( const kvs::Mat4 modelViewMatrix() const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Mat4 projectionModelViewMatrix() const );
//deprecated unused
    KVS_DEPRECATED( void getProjectionMatrix( float (*projection)[16] ) const );
//deprecated unused
    KVS_DEPRECATED( void getModelViewMatrix( float (*modelview)[16] ) const );
//deprecated unused
    KVS_DEPRECATED( void getProjectionModelViewMatrix( float (*projection_modelview)[16] ) const );
//deprecated unused
    KVS_DEPRECATED( void getProjectionModelViewMatrix(
        const float projection[16],
        const float modelview[16],
        float (*projection_modelview)[16] ) const );
    KVS_DEPRECATED( void getCombinedMatrix( float (*combined)[16] ) const );
//deprecated unused
    KVS_DEPRECATED( void getCombinedMatrix(
        const float projection[16],
        const float modelview[16],
        float (*combined)[16] ) const );

//deprecated unused
    KVS_DEPRECATED( const kvs::Vec2 projectObjectToWindow(
        float  p_obj_x,
        float  p_obj_y,
        float  p_obj_z,
        float* depth = NULL ) const );
//deprecated used at ObjectBase.cpp and ObjectManager.cpp
    KVS_DEPRECATED( const kvs::Vec2 projectObjectToWindow( const kvs::Vec3& p_obj, float* depth = NULL ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectWindowToObject( const kvs::Vec2& p_win, float depth = 0.0 ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectWindowToCamera( const kvs::Vec2& p_win, float depth = 0.0 ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectWindowToWorld( const kvs::Vec2& p_win, float depth = 0.0 ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectObjectToCamera( const kvs::Vec3& p_obj ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectCameraToObject( const kvs::Vec3& p_cam ) const );
//deprecated used at Light.cpp
    KVS_DEPRECATED( const kvs::Vec3 projectWorldToCamera( const kvs::Vec3& p_wld ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectCameraToWorld( const kvs::Vec3& p_cam ) const );
//deprecated used at Shader.cpp, HAVSVolumeRenderer and RayCastingRenderer.cpp
    KVS_DEPRECATED( const kvs::Vec3 projectWorldToObject( const kvs::Vec3& p_wld ) const );
//deprecated unused
    KVS_DEPRECATED( const kvs::Vec3 projectObjectToWorld( const kvs::Vec3& p_obj ) const );
};

} // end of namespace kvs
