/*****************************************************************************/
/**
 *  @file   Scene.h
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
#pragma once

#include <kvs/Mouse>
#include <kvs/ScreenBase>
#include <kvs/CubicImage>


namespace kvs
{

class Camera;
class Light;
class Mouse;
class Background;
class ObjectManager;
class RendererManager;
class IDManager;
class ObjectBase;
class RendererBase;

/*===========================================================================*/
/**
 *  @brief  Default scene class.
 */
/*===========================================================================*/
class Scene
{
public:
    enum ControlTarget
    {
        TargetObject = 0, ///< controlling object
        TargetCamera, ///< controlling camera
        TargetLight, ///< controlling light
        NumberOfTargets ///< number of control targets
    };

private:
    kvs::ScreenBase* m_screen; ///< screen
    kvs::Camera* m_camera; ///< camera
    kvs::Light* m_light; ///< light
    kvs::Mouse* m_mouse; ///< mouse
    kvs::Background* m_background; ///< background
    kvs::ObjectManager* m_object_manager; ///< object manager
    kvs::RendererManager* m_renderer_manager; ///< renderer manager
    kvs::IDManager* m_id_manager; ///< ID manager ( object_id, renderer_id )
    ControlTarget m_target; ///< control target
    bool m_enable_object_operation;  ///< flag for object operation
    bool m_enable_collision_detection; ///< flag for collision detection

public:
    Scene( kvs::ScreenBase* screen );
    virtual ~Scene();

    const std::pair<int,int> registerObject( kvs::ObjectBase* object, kvs::RendererBase* renderer = 0 );
    void removeObject( int object_id, bool delete_object = true, bool delete_renderer = true );
    void removeObject( std::string object_name, bool delete_object = true, bool delete_renderer = true );
    void replaceObject( int object_id, kvs::ObjectBase* object, bool delete_object = true );
    void replaceObject( std::string object_name, kvs::ObjectBase* object, bool delete_object = true );
    void replaceRenderer( int renderer_id, kvs::RendererBase* renderer, bool delete_renderer = true );
    void replaceRenderer( std::string renderer_name, kvs::RendererBase* renderer, bool delete_renderer = true );

    int numberOfObjects() const;
    bool hasObjects() const;
    bool hasObject( int id ) const;
    bool hasObject( std::string name ) const;
    kvs::ObjectBase* object( int id = -1 );
    kvs::ObjectBase* object( std::string name );
    kvs::RendererBase* renderer( int id = -1 );
    kvs::RendererBase* renderer( std::string name );

    void reset();
    bool isActiveMove( int x, int y );
    void updateControllingObject();
    void updateCenterOfRotation();
    void updateXform();
    void updateXform( kvs::ObjectManager* manager );
    void updateXform( kvs::Camera* camera );
    void updateXform( kvs::Light* light );

    void updateGLModelingMatrix( const kvs::ObjectBase* object ) const;
    void updateGLModelingMatrix() const;
    void updateGLViewingMatrix() const;
    void updateGLProjectionMatrix() const;
    void updateGLLightParameters() const;

    kvs::CubicImage cubemap();

    void setEnabledCollisionDetection( bool enable );
    void enableCollisionDetection() { this->setEnabledCollisionDetection( true ); }
    void disableCollisionDetection() { this->setEnabledCollisionDetection( false ); }
    bool isEnabledCollisionDetection() const { return m_enable_collision_detection; }

    void setEnabledObjectOperation( bool enable ) { m_enable_object_operation = enable; }
    void enableObjectOperation() { this->setEnabledObjectOperation( true ); }
    void disableObjectOperation() { this->setEnabledObjectOperation( false ); }
    bool isEnabledObjectOperation() const { return m_enable_object_operation; }

    kvs::ScreenBase* screen() { return m_screen; }
    kvs::Camera* camera() { return m_camera; }
    kvs::Light* light() { return m_light; }
    kvs::Mouse* mouse() { return m_mouse; }
    kvs::Background* background() { return m_background; }
    kvs::ObjectManager* objectManager() { return m_object_manager; }
    kvs::RendererManager* rendererManager() { return m_renderer_manager; }
    kvs::IDManager* IDManager() { return m_id_manager; }
    ControlTarget& controlTarget() { return m_target; }

    const kvs::Camera* camera() const { return m_camera; }
    const kvs::Light* light() const { return m_light; }
    const kvs::Mouse* mouse() const { return m_mouse; }
    const kvs::Background* background() const { return m_background; }
    const kvs::ObjectManager* objectManager() const { return m_object_manager; }
    const kvs::RendererManager* rendererManager() const { return m_renderer_manager; }
    const kvs::IDManager* IDManager() const { return m_id_manager; }
    const ControlTarget& controlTarget() const { return m_target; }

    void initializeFunction();
    void paintFunction();
    void resizeFunction( int width, int height );
    void mouseReleaseFunction( int x, int y );
    void mousePressFunction( int x, int y, kvs::Mouse::OperationMode mode );
    void mouseMoveFunction( int x, int y );
    void wheelFunction( int value );

private:
    kvs::Vec2 position_in_device( const kvs::ObjectBase* object ) const;
    kvs::Vec2 position_in_device() const;
    bool detect_collision( const kvs::Vec2& p_win );
    bool detect_collision( const kvs::ObjectBase* object, const kvs::Vec2& p_win );
};

} // end of namespace kvs
