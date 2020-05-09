/*****************************************************************************/
/**
 *  @file   Scene.cpp
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
#include "Scene.h"
#include <kvs/OpenGL>
#include <kvs/ScreenBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Mouse>
#include <kvs/Background>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/ObjectBase>
#include <kvs/RendererBase>
#include <kvs/VisualizationPipeline>
#include <kvs/Coordinate>


namespace
{

inline kvs::Vec3 World2Camera( const kvs::Vec3& p, const kvs::Camera* camera )
{
    return kvs::WorldCoordinate( p ).
        toCameraCoordinate( camera ).position();
}

inline kvs::Vec2 Object2Window( const kvs::Vec3& p, const kvs::Camera* camera )
{
    const float w = camera->windowWidth();
    const float h = camera->windowHeight();
    const kvs::Xform pvm( camera->projectionMatrix() * kvs::OpenGL::ModelViewMatrix() );
    const kvs::Vec3 p_cam = pvm.project( p );
    return ( kvs::Vec2( 1, 1 ) + p_cam.xy() ) * kvs::Vec2( w, h ) * 0.5f;
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Scene class.
 */
/*===========================================================================*/
Scene::Scene( kvs::ScreenBase* screen ):
    m_screen( screen ),
    m_target( Scene::TargetObject ),
    m_enable_object_operation( true ),
    m_enable_collision_detection( false )
{
    m_camera = new kvs::Camera();
    m_light = new kvs::Light();
    m_mouse = new kvs::Mouse();
    m_background = new kvs::Background( kvs::RGBColor( 212, 221, 229 ) );
    m_object_manager = new kvs::ObjectManager();
    m_renderer_manager = new kvs::RendererManager();
    m_id_manager = new kvs::IDManager();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Scene class.
 */
/*===========================================================================*/
Scene::~Scene()
{
    delete m_camera;
    delete m_light;
    delete m_mouse;
    delete m_background;
    delete m_object_manager;
    delete m_renderer_manager;
    delete m_id_manager;
}

/*===========================================================================*/
/**
 *  @brief  Registers an object with a renderer.
 *  @param  object [in] pointer to the object
 *  @param  renderer [in] pointer to the renderer
 *  @return Pair of IDs (object ID and renderer ID)
 */
/*===========================================================================*/
const std::pair<int,int> Scene::registerObject( kvs::ObjectBase* object, kvs::RendererBase* renderer )
{
    // If the given pointer to the renderer is null, a renderer for the given
    // object is automatically created by using visualization pipeline class.
    if ( !renderer )
    {
        kvs::VisualizationPipeline pipeline( object );
        if ( !pipeline.exec() )
        {
            kvsMessageError("Cannot create a renderer for the given object.");
            return std::pair<int,int>( -1, -1 );
        }

        renderer = const_cast<kvs::RendererBase*>( pipeline.renderer() );
    }

    if ( !object->hasMinMaxObjectCoords() )
    {
        object->updateMinMaxCoords();
    }

    // If the object has not been registered in the object managet,
    // the object is registered and then its ID is returned.
    int object_id = m_object_manager->objectID( object );
    if ( object_id == -1 ) object_id = m_object_manager->insert( object );

    // If the renderer has not been registered in the renderer managet,
    // the renderer is registered and then its ID is returned.
    int renderer_id = m_renderer_manager->rendererID( renderer );
    if ( renderer_id == -1 ) renderer_id = m_renderer_manager->insert( renderer );

    // Insert the IDs into the ID manager.
    m_id_manager->insert( object_id, renderer_id );
    renderer->setScreen( this->screen() );

    return std::pair<int,int>( object_id, renderer_id );
}

/*===========================================================================*/
/**
 *  @brief  Removes the object specified by the given object ID.
 *  @param  object_id [in] object ID
 *  @param  delete_object [in] if true, the object will be deleted
 *  @param  delete_renderer [in] if true, the renderers for the object will be deleted
 */
/*===========================================================================*/
void Scene::removeObject( int object_id, bool delete_object, bool delete_renderer )
{
    // Remove the object specified by the given object ID for the object manager.
    m_object_manager->erase( object_id, delete_object );

    // Remove the all of renderers assigned for the specified object from
    // the renderer manager.
    const std::vector<int> renderer_ids = m_id_manager->rendererID( object_id );
    for ( size_t i = 0; i < renderer_ids.size(); i++ )
    {
        m_renderer_manager->erase( renderer_ids[i], delete_renderer );
    }

    // Remove IDs specified by the given object ID from the ID manager.
    m_id_manager->eraseByObjectID( object_id );
}

/*===========================================================================*/
/**
 *  @brief  Removes the object specified by the given object name.
 *  @param  object_name [in] object name
 *  @param  delete_object [in] if true, the registered object will be deleted
 *  @param  delete_renderer [in] if true, the registered renderers for the object will be deleted
 */
/*===========================================================================*/
void Scene::removeObject( std::string object_name, bool delete_object, bool delete_renderer )
{
    const kvs::ObjectBase* object = m_object_manager->object( object_name );
    const int object_id = m_object_manager->objectID( object );
    this->removeObject( object_id, delete_object, delete_renderer );
}

/*===========================================================================*/
/**
 *  @brief  Replaces the object specified by the given object ID with the input object.
 *  @param  object_id [in] object ID
 *  @param  object [in] pointer to the object will be registered
 *  @param  delete_object [in] if true, the registered object will be deleted
 */
/*===========================================================================*/
void Scene::replaceObject( int object_id, kvs::ObjectBase* object, bool delete_object )
{
    m_object_manager->change( object_id, object, delete_object );
}

/*===========================================================================*/
/**
 *  @brief  Replaces the object specified by the given object name with the input object.
 *  @param  object_id [in] object name
 *  @param  object [in] pointer to the object will be registered
 *  @param  delete_object [in] if true, the registered object will be deleted
 */
/*===========================================================================*/
void Scene::replaceObject( std::string object_name, kvs::ObjectBase* object, bool delete_object )
{
    m_object_manager->change( object_name, object, delete_object );
}

/*===========================================================================*/
/**
 *  @brief  Replaces the renderer specified by the given renderer ID with the input renderer.
 *  @param  renderer_id [in] renderer ID
 *  @param  renderer [in] pointer to the renderer will be registered
 *  @param  delete_renderer [in] if true, the registered renderer will be deleted
 */
/*===========================================================================*/
void Scene::replaceRenderer( int renderer_id, kvs::RendererBase* renderer, bool delete_renderer )
{
    m_renderer_manager->change( renderer_id, renderer, delete_renderer );
}

/*===========================================================================*/
/**
 *  @brief  Replaces the renderer specified by the given renderer name with the input renderer.
 *  @param  renderer_name [in] renderer name
 *  @param  renderer [in] pointer to the renderer will be registered
 *  @param  delete_renderer [in] if true, the registered renderer will be deleted
 */
/*===========================================================================*/
void Scene::replaceRenderer( std::string renderer_name, kvs::RendererBase* renderer, bool delete_renderer )
{
    m_renderer_manager->change( renderer_name, renderer, delete_renderer );
}

/*===========================================================================*/
/**
 *  @brief  Returns number of objects contained in the scene.
 *  @return number of objects
 */
/*===========================================================================*/
int Scene::numberOfObjects() const
{
    return m_object_manager->numberOfObjects();
}

/*===========================================================================*/
/**
 *  @brief  Return true if the scene has one or more objects.
 *  @return true if the scene has one or more objects
 */
/*===========================================================================*/
bool Scene::hasObjects() const
{
    return m_object_manager->hasObject();
}

/*===========================================================================*/
/**
 *  @brief  Return true if the scene has the object specified by given ID.
 *  @param  id [in] object ID
 *  @return true if the scene has the object
 */
/*===========================================================================*/
bool Scene::hasObject( int id ) const
{
    return m_object_manager->object( id ) != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Return true if the scene has the object specified by given name.
 *  @param  name [in] object name
 *  @return true if the scene has the object
 */
/*===========================================================================*/
bool Scene::hasObject( std::string name ) const
{
    return m_object_manager->object( name ) != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Return pointer to the object specified by given ID.
 *  @param  id [in] object ID
 *  @return pointer to the object
 */
/*===========================================================================*/
kvs::ObjectBase* Scene::object( int id )
{
    if ( id < 0 ) { return m_object_manager->object(); }
    return m_object_manager->object( id );
}

/*===========================================================================*/
/**
 *  @brief  Return pointer to the object specified by given name.
 *  @param  name [in] object name
 *  @return pointer to the object
 */
/*===========================================================================*/
kvs::ObjectBase* Scene::object( std::string name )
{
    return m_object_manager->object( name );
}

/*===========================================================================*/
/**
 *  @brief  Return pointer to the renderer specified by given ID.
 *  @param  id [in] renderer ID
 *  @return pointer to the renderer
 */
/*===========================================================================*/
kvs::RendererBase* Scene::renderer( int id )
{
    if ( id < 0 ) { return m_renderer_manager->renderer(); }
    return m_renderer_manager->renderer( id );
}

/*===========================================================================*/
/**
 *  @brief  Return pointer to the renderer specified by given name.
 *  @param  name [in] renderer name
 *  @return pointer to the renderer
 */
/*===========================================================================*/
kvs::RendererBase* Scene::renderer( std::string name )
{
    return m_renderer_manager->renderer( name );
}

/*===========================================================================*/
/**
 *  @brief  Resets the x-form matrix of the basic screen components.
 */
/*===========================================================================*/
void Scene::reset()
{
    // Reset viewpoint to the initial position.
    m_mouse->reset();

    // Reset the xform of the object.
    if ( m_object_manager->hasActiveObject() )
    {
        m_object_manager->resetActiveObjectXform();
    }
    else
    {
        m_object_manager->resetXform();
    }

    // Reset the xform of the camera.
    m_camera->resetXform();

    // Reset the xform of the light.
    m_light->resetXform();
}

/*==========================================================================*/
/**
 *  @brief  Test whether the screen is the active move mode.
 *  @param  x [in] x coordinate value of the mouse cursor position
 *  @param  y [in] y coordinate value of the mouse cursor position
 *  @return true, if the screen is the active move mode
 */
/*==========================================================================*/
bool Scene::isActiveMove( int x, int y )
{
    if ( !m_object_manager->hasObject() ) return true;

    if ( m_target == TargetObject )
    {
        if ( !m_enable_object_operation && m_enable_collision_detection )
        {
            const float px = static_cast<float>(x);
            const float py = static_cast<float>(y);
            return this->detect_collision( kvs::Vec2( px, py ) );
        }
    }

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Updates the controlling object.
 */
/*==========================================================================*/
void Scene::updateControllingObject()
{
    if ( m_target == TargetObject )
    {
        if ( m_enable_object_operation )
        {
            m_object_manager->releaseActiveObject();
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Updates the center of rotation.
 */
/*==========================================================================*/
void Scene::updateCenterOfRotation()
{
    // Center of rotation in the device coordinate system.
    kvs::Vec2 center( 0.0, 0.0 );

    switch ( m_target )
    {
    case TargetCamera:
    case TargetLight:
        // Get an at-point of the camera, which is the center of rotation,
        // in the device coord.
        center = m_camera->lookAtInDevice();
        break;
    case TargetObject:
        if ( m_enable_object_operation || !m_object_manager->hasObject() )
        {
            // The center of rotation will be set to the gravity of the object
            // manager in device coordinates.
            center = this->position_in_device();
        }
        else
        {
            // If the object manager has no active object, the center
            // of rotation is not updated.
            if ( !m_object_manager->hasActiveObject() ) return;

            // The center of rotation will be set to the gravity of the object
            // in device coordinates.
            kvs::ObjectBase* object = m_object_manager->activeObject();
            center = this->position_in_device( object );
        }
        break;
    default:
        break;
    }

    m_mouse->trackball().setRotationCenter( center );
}

/*==========================================================================*/
/**
 *  @brief  Updates the xform of the control target.
 */
/*==========================================================================*/
void Scene::updateXform()
{
    switch ( m_target )
    {
    case TargetCamera:
        this->updateXform( m_camera );
        break;
    case TargetLight:
        this->updateXform( m_light );
        break;
    case TargetObject:
        this->updateXform( m_object_manager );
        break;
    default:
        break;
    }
}

/*==========================================================================*/
/**
 *  @brief  Updates the xform of the object manager.
 *  @param  manager [in] pointer to the object manager
 */
/*==========================================================================*/
void Scene::updateXform( kvs::ObjectManager* manager )
{
    switch ( m_mouse->operationMode() )
    {
    case kvs::Mouse::Rotation:
        manager->rotate( m_mouse->trackball().rotation().toMatrix() );
        break;
    case kvs::Mouse::Translation:
        manager->translate( m_mouse->trackball().translation() );
        break;
    case kvs::Mouse::Scaling:
        manager->scale( m_mouse->trackball().scaling() );
        break;
    default:
        break;
    }
}

/*==========================================================================*/
/**
 *  @brief  Updates the xform of the camera.
 *  @param  camera [in] pointer to the camera
 */
/*==========================================================================*/
void Scene::updateXform( kvs::Camera* camera )
{
    switch ( m_mouse->operationMode() )
    {
    case kvs::Mouse::Rotation:
        camera->rotate( m_mouse->trackball().rotation().toMatrix().transposed() );
        break;
    case kvs::Mouse::Translation:
        camera->translate( -m_mouse->trackball().translation() );
        break;
    case kvs::Mouse::Scaling:
    {
        const kvs::Vec3 s = m_mouse->trackball().scaling();
        camera->scale( kvs::Vec3( 1 / s.x(), 1 / s.y(), 1 / s.z() ) );
        break;
    }
    default:
        break;
    }
}

/*==========================================================================*/
/**
 *  @brief  Updates the xform of the light.
 *  @param  light [in] pointer to the light
 */
/*==========================================================================*/
void Scene::updateXform( kvs::Light* light )
{
    switch ( m_mouse->operationMode() )
    {
    case kvs::Mouse::Rotation:
        light->rotate( m_mouse->trackball().rotation().toMatrix() );
        break;
    case kvs::Mouse::Translation:
        light->translate( m_mouse->trackball().translation() );
        break;
    case kvs::Mouse::Scaling:
        light->scale( m_mouse->trackball().scaling() );
        break;
    default:
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Updates the modeling matrinx for the object.
 *  @param  object [in] pointer to the object
 */
/*===========================================================================*/
void Scene::updateGLModelingMatrix( const kvs::ObjectBase* object ) const
{
    float X[16]; object->xform().toArray( X );
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::MultMatrix( X );
}

/*===========================================================================*/
/**
 *  @brief  Updates the modeling matrinx for the object manager.
 *  @param  object [in] pointer to the object
 */
/*===========================================================================*/
void Scene::updateGLModelingMatrix() const
{
    float X[16]; m_object_manager->xform().toArray( X );
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::MultMatrix( X );
}

/*===========================================================================*/
/**
 *  @brief  Updates the viewing matrinx.
 */
/*===========================================================================*/
void Scene::updateGLViewingMatrix() const
{
    float m[16];
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::LoadIdentity();
    kvs::Xform v( m_camera->viewingMatrix() ); v.toArray( m );
    kvs::OpenGL::MultMatrix( m );
}

/*===========================================================================*/
/**
 *  @brief  Updates the projection matrinx.
 */
/*===========================================================================*/
void Scene::updateGLProjectionMatrix() const
{
    float m[16];
    kvs::OpenGL::SetMatrixMode( GL_PROJECTION );
    kvs::OpenGL::LoadIdentity();
    kvs::Xform p( m_camera->projectionMatrix() ); p.toArray( m );
    kvs::OpenGL::MultMatrix( m );
}

/*===========================================================================*/
/**
 *  @brief  Updates the light source parameters.
 */
/*===========================================================================*/
void Scene::updateGLLightParameters() const
{
    const kvs::Vec3 p = ::World2Camera( m_light->position(), m_camera );
    const kvs::Vec4 position( p, 1.0f );
    const kvs::Vec4 diffuse( m_light->diffuse(), 1.0f );
    const kvs::Vec4 ambient( m_light->ambient(), 1.0f );
    const kvs::Vec4 specular( m_light->specular(), 1.0f );

    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::LoadIdentity();
    kvs::OpenGL::SetLight( m_light->id(), GL_POSITION, (GLfloat*)&(position[0]) );
    kvs::OpenGL::SetLight( m_light->id(), GL_DIFFUSE,  (GLfloat*)&(diffuse[0]) );
    kvs::OpenGL::SetLight( m_light->id(), GL_AMBIENT,  (GLfloat*)&(ambient[0]) );
    kvs::OpenGL::SetLight( m_light->id(), GL_SPECULAR, (GLfloat*)&(specular[0]) );
    kvs::OpenGL::PopMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Returns cube map image generated from 6 direction images.
 *  @return cube map image
 */
/*===========================================================================*/
kvs::CubicImage Scene::cubemap()
{
    kvs::CubicImage cube_map;

    const float c_fov = this->camera()->fieldOfView();
    const float c_front = this->camera()->front();
    const kvs::Vec3 c_at = this->camera()->lookAt();
    const kvs::Vec3 c_up = this->camera()->upVector();
    const kvs::Vec3 c_p = this->camera()->position();
    const kvs::Vec3 l_p = this->light()->position();
    {
        this->camera()->setFieldOfView( 90.0 );
        this->camera()->setFront( 0.1 );
        for ( size_t i = 0; i < kvs::CubicImage::NumberOfDirections; i++ )
        {
            const kvs::CubicImage::Direction dir = kvs::CubicImage::Direction( i );
            const kvs::Vec3 dir_vec = kvs::CubicImage::DirectionVector( dir );
            const kvs::Vec3 up_vec = kvs::CubicImage::UpVector( dir );
            this->light()->setPosition( c_p );
            this->camera()->setPosition( c_p, c_p + dir_vec, up_vec );
            this->screen()->draw();
            cube_map.setImage( dir, this->screen()->capture() );
        }
    }
    this->camera()->setFieldOfView( c_fov );
    this->camera()->setFront( c_front );
    this->camera()->setPosition( c_p, c_at, c_up );
    this->light()->setPosition( l_p );

    return cube_map;
}

/*===========================================================================*/
/**
 *  @brief  Enables or disables the collision detection.
 *  @param  enable [in] flag for the collision detection
 */
/*===========================================================================*/
void Scene::setEnabledCollisionDetection( bool enable )
{
    m_enable_collision_detection = enable;
    if ( enable ) m_enable_object_operation = false;
}

/*==========================================================================*/
/**
 *  @brief  Initalizes the screen.
 */
/*==========================================================================*/
void Scene::initializeFunction()
{
    // Set the lighting parameters.
    m_light->on();

    // Attach the Camera to the Mouse
    m_mouse->trackball().attachCamera( m_camera );
}

/*==========================================================================*/
/**
 *  @brief  Core paint event function.
 */
/*==========================================================================*/
void Scene::paintFunction()
{
    this->updateGLProjectionMatrix();
    this->updateGLViewingMatrix();
    this->updateGLLightParameters();

    // Set the background color or image.
    m_background->apply();

    // Rendering the resistered object by using the corresponding renderer.
    if ( m_object_manager->hasObject() )
    {
        const int size = m_id_manager->size();
        for ( int index = 0; index < size; index++ )
        {
            kvs::IDManager::IDPair id = m_id_manager->id( index );
            kvs::ObjectBase* object = m_object_manager->object( id.first );
            kvs::RendererBase* renderer = m_renderer_manager->renderer( id.second );
            if ( object->isShown() )
            {
                kvs::OpenGL::PushMatrix();
                this->updateGLModelingMatrix( object );
                renderer->exec( object, m_camera, m_light );
                kvs::OpenGL::PopMatrix();
            }
        }
    }
    else
    {
        this->updateGLModelingMatrix();
    }
}

/*==========================================================================*/
/**
 *  @brief  Core resize event function.
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*==========================================================================*/
void Scene::resizeFunction( int width, int height )
{
    // Update the window size for camera.
    m_camera->setWindowSize( width, height );

    // Update the viewport for OpenGL.
    const float dpr = m_camera->devicePixelRatio();
    const int framebuffer_width = width * dpr;
    const int framebuffer_height = height * dpr;
    kvs::OpenGL::SetViewport( 0, 0, framebuffer_width, framebuffer_height );
}

/*==========================================================================*/
/**
 *  @brief  Function which is called when the mouse button is released.
 *  @param  x [in] x coordinate value of the mouse cursor position
 *  @param  y [in] y coordinate value of the mouse cursor position
 */
/*==========================================================================*/
void Scene::mouseReleaseFunction( int x, int y )
{
    m_enable_object_operation = true;
    m_enable_collision_detection = false;
    m_mouse->release( x, y );

    if ( !( m_mouse->isEnabledAutoUpdating() && m_mouse->isUpdating() ) )
    {
        m_object_manager->releaseActiveObject();
    }
}

/*==========================================================================*/
/**
 *  @brief  Function which is called when the mouse button is released.
 *  @param  x [in] x coordinate value of the mouse cursor position
 *  @param  y [in] y coordinate value of the mouse cursor position
 *  @param  mode [in] mouse operation mode
 */
/*==========================================================================*/
void Scene::mousePressFunction( int x, int y, kvs::Mouse::OperationMode mode )
{
    if ( m_enable_object_operation || m_object_manager->hasActiveObject() )
    {
        this->updateControllingObject();
        m_mouse->setOperationMode( mode );
        m_mouse->press( x, y );
    }
}

/*==========================================================================*/
/**
 *  @brief  Function which is called when the mouse cursor is moved.
 *  @param  x [in] x coordinate value of the mouse cursor position
 *  @param  y [in] y coordinate value of the mouse cursor position
 */
/*==========================================================================*/
void Scene::mouseMoveFunction( int x, int y )
{
    if ( m_enable_object_operation || m_object_manager->hasActiveObject() )
    {
        m_mouse->move( x, y );
        this->updateXform();
    }
}

/*===========================================================================*/
/**
 *  @brief  Function which is called when the mouse wheel is scrolled.
 *  @param  value [in] incremental value
 */
/*===========================================================================*/
void Scene::wheelFunction( int value )
{
    if ( m_enable_object_operation || m_object_manager->hasActiveObject() )
    {
        this->updateControllingObject();
        m_mouse->setOperationMode( kvs::Mouse::Scaling );
        m_mouse->press( 0, 0 );
        m_mouse->move( 0, value );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the gravity of the center of the object in device coordinates.
 *  @param  object [in] pointer to the object
 *  @return gravity of the center of the object in device coordinates
 */
/*===========================================================================*/
kvs::Vec2 Scene::position_in_device( const kvs::ObjectBase* object ) const
{
    kvs::Vec2 p_dev;
    kvs::Vec3 p_obj = object->objectCenter();
    kvs::OpenGL::PushMatrix();
    {
        this->updateGLProjectionMatrix();
        this->updateGLViewingMatrix();
        this->updateGLModelingMatrix( object );

        const kvs::Vec2 p_win = ::Object2Window( p_obj, m_camera );
        p_dev.x() = p_win.x();
        p_dev.y() = m_camera->windowHeight() - p_win.y();
    }
    kvs::OpenGL::PopMatrix();

    return p_dev;
}

/*===========================================================================*/
/**
 *  @brief  Returns the gravity of the center of the object manager in device coordinates.
 *  @return gravity of the center of the object manager in device coordinates
 */
/*===========================================================================*/
kvs::Vec2 Scene::position_in_device() const
{
    kvs::Vec2 p_dev;
    kvs::Vec3 p_obj = m_object_manager->xform().translation();
    kvs::OpenGL::PushMatrix();
    {
        this->updateGLProjectionMatrix();
        this->updateGLViewingMatrix();

        const kvs::Vec2 p_win = ::Object2Window( p_obj, m_camera );
        p_dev.x() = p_win.x();
        p_dev.y() = m_camera->windowHeight() - p_win.y();
    }
    kvs::OpenGL::PopMatrix();

    return p_dev;
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the object collision is detected by clicking the mouse.
 *  @param  p_win [in] clicking position in the window coordinates
 *  @return true if the collision is detected
 */
/*===========================================================================*/
bool Scene::detect_collision( const kvs::Vec2& p_win )
{
    double min_distance = 100000.0;

    int active_object_id = -1;
    kvs::ObjectBase* active_object = 0;
    const size_t size = m_id_manager->size();
    for ( size_t i = 0; i < size; i++ )
    {
        kvs::IDManager::IDPair id = m_id_manager->id( i );
        kvs::ObjectBase* object = m_object_manager->object( id.first );
        const kvs::Vec2 p = this->position_in_device( object );
        const kvs::Vec2 diff = p - p_win;
        const double distance = diff.length();
        if ( distance < min_distance )
        {
            min_distance = distance;
            active_object_id = id.first;
            active_object = object;
        }
    }

    bool has_active_object = false;
    if ( this->detect_collision( active_object, p_win ) )
    {
        m_object_manager->setActiveObject( active_object_id );
        has_active_object = true;
    }

    return has_active_object;
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the object is selected by clicking the mouse.
 *  @param  object [in] pointer to the object
 *  @param  p_win [in] clicking position in the window coordinates
 *  @return true if the object is selected
 */
/*===========================================================================*/
bool Scene::detect_collision( const kvs::ObjectBase* object, const kvs::Vec2& p_win )
{
    float max_distance = -1.0f;

    // Center of this object in the window coordinate system.
    kvs::Vec2 center;

    kvs::OpenGL::PushMatrix();
    {
        this->updateGLProjectionMatrix();
        this->updateGLViewingMatrix();
        this->updateGLModelingMatrix( object );

        center = ::Object2Window( object->objectCenter(), m_camera );

        // Object's corner points in the object coordinate system.
        const kvs::Vec3 min_object_coord = object->minObjectCoord();
        const kvs::Vec3 max_object_coord = object->maxObjectCoord();
        const kvs::Vec3 corners[8] = {
            kvs::Vec3( min_object_coord.x(), min_object_coord.y(), min_object_coord.z() ),
            kvs::Vec3( max_object_coord.x(), min_object_coord.y(), min_object_coord.z() ),
            kvs::Vec3( min_object_coord.x(), min_object_coord.y(), max_object_coord.z() ),
            kvs::Vec3( max_object_coord.x(), min_object_coord.y(), max_object_coord.z() ),
            kvs::Vec3( min_object_coord.x(), max_object_coord.y(), min_object_coord.z() ),
            kvs::Vec3( max_object_coord.x(), max_object_coord.y(), min_object_coord.z() ),
            kvs::Vec3( min_object_coord.x(), max_object_coord.y(), max_object_coord.z() ),
            kvs::Vec3( max_object_coord.x(), max_object_coord.y(), max_object_coord.z() ) };

        // Calculate max distance between the center and the corner in
        // the window coordinate system.
        for( int i = 0; i < 8; i++ )
        {
            const kvs::Vec2 corner = ::Object2Window( corners[i], m_camera );
            const float distance = static_cast<float>( ( corner - center ).length() );
            max_distance = kvs::Math::Max( max_distance, distance );
        }
    }
    kvs::OpenGL::PopMatrix();

    kvs::Vec2 pos_window( p_win.x(), m_camera->windowHeight() - p_win.y() );

    return ( pos_window - center ).length() < max_distance;
}

} // end of namespace kvs
