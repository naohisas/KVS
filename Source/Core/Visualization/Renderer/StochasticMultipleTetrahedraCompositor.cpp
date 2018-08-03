/*****************************************************************************/
/**
 *  @file   StochasticMultipleTetrahedraCompositor.cpp
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
#include "StochasticMultipleTetrahedraCompositor.h"
#include <kvs/Assert>
#include <kvs/OpenGL>
#include <kvs/PaintEvent>
#include <kvs/EventHandler>
#include <kvs/ScreenBase>
#include <kvs/Scene>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Background>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/UnstructuredVolumeObject>
#include "StochasticRendererBase.h"
#include "StochasticMultipleTetrahedraRenderer.h"

#if defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/GLUT>
#endif


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticMultipleTetrahedraCompositor class.
 *  @param  scene [in] pointer to the scene
 */
/*===========================================================================*/
StochasticMultipleTetrahedraCompositor::StochasticMultipleTetrahedraCompositor( kvs::Scene* scene ):
    m_scene( scene ),
    m_width( 0 ),
    m_height( 0 ),
    m_repetition_level( 1 ),
    m_coarse_level( 1 ),
    m_enable_lod( false ),
    m_enable_refinement( false ),
    m_enable_shading( true ),
    m_renderer( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Updates the scene.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::update()
{
    KVS_ASSERT( m_scene );

    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();
    {
        m_scene->updateGLProjectionMatrix();
        m_scene->updateGLViewingMatrix();
        m_scene->updateGLLightParameters();
        m_scene->background()->apply();

        if ( m_scene->objectManager()->hasObject() )
        {
            this->draw();
        }
    }

    kvs::OpenGL::Flush();

#if defined( KVS_SUPPORT_GLUT )
    glutSwapBuffers();
#endif
}

/*===========================================================================*/
/**
 *  @brief  Draws the objects with stochastic renderers.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::draw()
{
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    this->check_window_created();
    this->check_window_resized();
    this->check_object_changed();

    // LOD control.
    size_t repetitions = m_repetition_level;
    kvs::Vec3 camera_position = m_scene->camera()->position();
    kvs::Vec3 light_position = m_scene->light()->position();
    kvs::Mat4 object_xform = this->object_xform();
    if ( m_camera_position != camera_position ||
         m_light_position != light_position ||
         m_object_xform != object_xform )
    {
        if ( m_enable_lod )
        {
            repetitions = m_coarse_level;
        }
        m_camera_position = camera_position;
        m_light_position = light_position;
        m_object_xform = object_xform;
        m_ensemble_buffer.clear();
    }

    // Setup engine.
    this->engine_setup();

    // Ensemble rendering.
    const bool reset_count = !m_enable_refinement;
    if ( reset_count ) m_ensemble_buffer.clear();
    for ( size_t i = 0; i < repetitions; i++ )
    {
        m_ensemble_buffer.bind();
        this->engine_draw();
        m_ensemble_buffer.unbind();
        m_ensemble_buffer.add();
    }
    m_ensemble_buffer.draw();

    kvs::OpenGL::Finish();
}

/*===========================================================================*/
/**
 *  @brief  Check whether the window is created and initialize the parameters.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::check_window_created()
{
    const bool window_created = m_width == 0 && m_height == 0;
    if ( window_created )
    {
        const size_t width = m_scene->camera()->windowWidth();
        const size_t height = m_scene->camera()->windowHeight();
        m_width = width;
        m_height = height;
        m_ensemble_buffer.create( width, height );
        m_ensemble_buffer.clear();
        m_object_xform = this->object_xform();
        m_camera_position = m_scene->camera()->position();
        m_light_position = m_scene->light()->position();
        this->create_extra_texture( width, height );
        this->engine_create();
    }
}

/*===========================================================================*/
/**
 *  @brief  Check whether the window is resized and update the parameters.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::check_window_resized()
{
    const size_t width = m_scene->camera()->windowWidth();
    const size_t height = m_scene->camera()->windowHeight();
    const bool window_resized = m_width != width || m_height != height;
    if ( window_resized )
    {
        m_width = width;
        m_height = height;
        m_ensemble_buffer.release();
        m_ensemble_buffer.create( width, height );
        m_ensemble_buffer.clear();
        this->create_extra_texture( width, height );
        this->engine_update();
    }
}

/*===========================================================================*/
/**
 *  @brief  Check whether the object is changed and recreated the engine.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::check_object_changed()
{
    typedef kvs::StochasticMultipleTetrahedraRenderer Renderer;

    kvs::ObjectBase* objects[2] = { NULL, NULL };
    size_t counter = 0;
    const size_t size = m_scene->IDManager()->size();
    for ( size_t i = 0; i < size; i++ )
    {
        kvs::IDManager::IDPair id = m_scene->IDManager()->id( i );
        kvs::ObjectBase* object = m_scene->objectManager()->object( id.first );
        kvs::RendererBase* renderer = m_scene->rendererManager()->renderer( id.second );
        if ( Renderer::DownCast( renderer ) )
        {
            // Drawing object.
            objects[ counter++ ] = object;
        }

        if ( counter == 2 ) break;
    }

    if ( counter == 2 )
    {
        const kvs::UnstructuredVolumeObject* volume0 = kvs::UnstructuredVolumeObject::DownCast( objects[0] );
        const kvs::UnstructuredVolumeObject* volume1 = kvs::UnstructuredVolumeObject::DownCast( objects[1] );
        const bool object_changed =
            ( m_renderer->volume(0) != volume0 ) ||
            ( m_renderer->volume(1) != volume1 );
        if ( object_changed )
        {
            m_ensemble_buffer.clear();
            m_renderer->engine().release();
            m_renderer->attachVolume( volume0, 0 );
            m_renderer->attachVolume( volume1, 1 );
            m_renderer->setExtraTexture( m_extra_texture );
            m_renderer->engine().setDepthTexture( m_ensemble_buffer.currentDepthTexture() );
            m_renderer->engine().setShader( &m_renderer->shader() );
            m_renderer->engine().setEnabledShading( m_enable_shading );
            m_renderer->engine().create( NULL, m_scene->camera(), m_scene->light() );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform matrix of the active object.
 *  @return xform matrix
 */
/*===========================================================================*/
kvs::Mat4 StochasticMultipleTetrahedraCompositor::object_xform()
{
    return m_scene->objectManager()->hasActiveObject() ?
        m_scene->objectManager()->activeObject()->xform().toMatrix() :
        m_scene->objectManager()->xform().toMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Calls the create method of the engine.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::engine_create()
{
    typedef kvs::UnstructuredVolumeObject Object;
    typedef kvs::StochasticMultipleTetrahedraRenderer Renderer;

    kvs::ObjectBase* objects[2] = { NULL, NULL };
    size_t counter = 0;
    const size_t size = m_scene->IDManager()->size();
    for ( size_t i = 0; i < size; i++ )
    {
        kvs::IDManager::IDPair id = m_scene->IDManager()->id( i );
        kvs::ObjectBase* object = m_scene->objectManager()->object( id.first );
        kvs::RendererBase* renderer = m_scene->rendererManager()->renderer( id.second );
        if ( Renderer* stochastic_renderer = Renderer::DownCast( renderer ) )
        {
            // Copy the pointer to the renderer used for drawing the objects.
            if ( counter == 0 )
            {
                m_renderer = stochastic_renderer;
            }

            // Drawing object.
            objects[ counter++ ] = object;
        }

        if ( counter == 2 ) break;
    }

    if ( counter == 2 )
    {
        const Object* volume0 = Object::DownCast( objects[0] );
        const Object* volume1 = Object::DownCast( objects[1] );
        m_renderer->attachVolume( volume0, 0 );
        m_renderer->attachVolume( volume1, 1 );
        m_renderer->setExtraTexture( m_extra_texture );
        m_renderer->engine().setDepthTexture( m_ensemble_buffer.currentDepthTexture() );
        m_renderer->engine().setShader( &m_renderer->shader() );
        m_renderer->engine().setRepetitionLevel( m_repetition_level );
        m_renderer->engine().setEnabledShading( m_enable_shading );

        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( volume0 );
        m_renderer->engine().create( NULL, m_scene->camera(), m_scene->light() );
        kvs::OpenGL::PopMatrix();
    }
}

/*===========================================================================*/
/**
 *  @brief  Calls the update method of the engine.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::engine_update()
{
    const kvs::UnstructuredVolumeObject* volume0 = m_renderer->volume(0);

    kvs::OpenGL::PushMatrix();
    m_scene->updateGLModelingMatrix( volume0 );
    m_renderer->engine().update( NULL, m_scene->camera(), m_scene->light() );
    kvs::OpenGL::PopMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Calls the setup method of the engine.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::engine_setup()
{
    const kvs::UnstructuredVolumeObject* volume0 = m_renderer->volume(0);
    const bool reset_count = !m_enable_refinement;

    kvs::OpenGL::PushMatrix();
    m_scene->updateGLModelingMatrix( volume0 );
    if ( reset_count ) m_renderer->engine().resetRepetitions();
    m_renderer->engine().setup( NULL, m_scene->camera(), m_scene->light() );
    kvs::OpenGL::PopMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Calls the draw method of the engine.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::engine_draw()
{
    const kvs::UnstructuredVolumeObject* volume0 = m_renderer->volume(0);
    const kvs::UnstructuredVolumeObject* volume1 = m_renderer->volume(1);
    if ( volume0->isShown() && volume1->isShown() )
    {
        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( volume0 );
        kvs::OpenGL::PopMatrix();

        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( volume1 );
        m_renderer->engine().draw( NULL, m_scene->camera(), m_scene->light() );
        m_renderer->engine().countRepetitions();
        kvs::OpenGL::PopMatrix();
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates extra texture.
 *  @param  width [in] window width
 *  @param  height [in] window height
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraCompositor::create_extra_texture( const size_t width, const size_t height )
{
    m_extra_texture.release();
    m_extra_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_extra_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_extra_texture.setMagFilter( GL_LINEAR );
    m_extra_texture.setMinFilter( GL_LINEAR );
    m_extra_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT );
    m_extra_texture.create( width, height );
    m_ensemble_buffer.currentFrameBufferObject().attachColorTexture( m_extra_texture, 1 );
}

} // end of namespace kvs
