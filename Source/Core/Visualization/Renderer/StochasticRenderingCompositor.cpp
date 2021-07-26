/*****************************************************************************/
/**
 *  @file   StochasticRenderingCompositor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "StochasticRenderingCompositor.h"
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
#include "StochasticRendererBase.h"
#include "ParticleBasedRenderer.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Updates the scene.
 */
/*===========================================================================*/
void StochasticRenderingCompositor::update()
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
        else
        {
            m_scene->updateGLModelingMatrix();
        }
    }
}

void StochasticRenderingCompositor::onWindowCreated()
{
    const size_t width = m_scene->camera()->windowWidth();
    const size_t height = m_scene->camera()->windowHeight();
    m_window_width = width;
    m_window_height = height;

    const float dpr = m_scene->camera()->devicePixelRatio();
    const size_t framebuffer_width = static_cast<size_t>( width * dpr );
    const size_t framebuffer_height = static_cast<size_t>( height * dpr );
    m_ensemble_buffer.create( framebuffer_width, framebuffer_height );
    m_ensemble_buffer.clear();

    m_object_xform = this->object_xform();
    m_camera_position = m_scene->camera()->position();
    m_light_position = m_scene->light()->position();

    this->createEngines();
}

void StochasticRenderingCompositor::onWindowResized()
{
    const size_t width = m_scene->camera()->windowWidth();
    const size_t height = m_scene->camera()->windowHeight();
    m_window_width = width;
    m_window_height = height;

    const float dpr = m_scene->camera()->devicePixelRatio();
    const size_t framebuffer_width = static_cast<size_t>( width * dpr );
    const size_t framebuffer_height = static_cast<size_t>( height * dpr );
    m_ensemble_buffer.release();
    m_ensemble_buffer.create( framebuffer_width, framebuffer_height );
    m_ensemble_buffer.clear();

    this->updateEngines();
}

void StochasticRenderingCompositor::onObjectChanged( Object* object, Renderer* renderer )
{
    m_ensemble_buffer.clear();

    auto& engine = renderer->engine();
    if ( engine.object() ) engine.release();
    engine.setDepthTexture( m_ensemble_buffer.currentDepthTexture() );
    engine.setShader( &renderer->shader() );
    engine.setRepetitionLevel( m_repetition_level );
    engine.setShadingEnabled( renderer->isShadingEnabled() );

    kvs::OpenGL::PushMatrix();
    m_scene->updateGLModelingMatrix( object );
    engine.create( object, m_scene->camera(), m_scene->light() );
    kvs::OpenGL::PopMatrix();
}

void StochasticRenderingCompositor::createEngines()
{
    this->for_each_renderer( [&] ( Object* object, Renderer* renderer )
    {
        auto& engine = renderer->engine();
        engine.setDepthTexture( m_ensemble_buffer.currentDepthTexture() );
        engine.setShader( &renderer->shader() );
        engine.setRepetitionLevel( m_repetition_level );
        engine.setShadingEnabled( renderer->isShadingEnabled() );

        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( object );
        engine.create( object, m_scene->camera(), m_scene->light() );
        kvs::OpenGL::PopMatrix();
    } );
}

void StochasticRenderingCompositor::updateEngines()
{
    this->for_each_renderer( [&] ( Object* object, Renderer* renderer )
    {
        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( object );
        renderer->engine().update( object, m_scene->camera(), m_scene->light() );
        kvs::OpenGL::PopMatrix();
    } );
}

void StochasticRenderingCompositor::setupEngines()
{
    this->for_each_renderer( [&] ( Object* object, Renderer* renderer )
    {
        const auto reset_count = !m_enable_refinement;
        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( object );
        if ( reset_count ) renderer->engine().resetRepetitions();
        renderer->engine().setup( object, m_scene->camera(), m_scene->light() );
        kvs::OpenGL::PopMatrix();
    } );
}

void StochasticRenderingCompositor::drawEngines()
{
    this->for_each_renderer( [&] ( Object* object, Renderer* renderer )
    {
        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( object );
        renderer->engine().draw( object, m_scene->camera(), m_scene->light() );
        renderer->engine().countRepetitions();
        kvs::OpenGL::PopMatrix();
    } );
}

void StochasticRenderingCompositor::setupBuffer()
{
    const bool reset_count = !m_enable_refinement;
    if ( reset_count ) m_ensemble_buffer.clear();
}

void StochasticRenderingCompositor::bindBuffer()
{
    m_ensemble_buffer.bind();
}

void StochasticRenderingCompositor::unbindBuffer()
{
    m_ensemble_buffer.unbind();
    m_ensemble_buffer.add();
}

void StochasticRenderingCompositor::drawBuffer()
{
    m_ensemble_buffer.draw();
}

/*===========================================================================*/
/**
 *  @brief  Draws the objects with stochastic renderers.
 */
/*===========================================================================*/
void StochasticRenderingCompositor::draw()
{
    m_timer.start();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    // Window created
    const bool window_created = m_window_width == 0 && m_window_height == 0;
    if ( window_created ) { this->onWindowCreated(); }

    // Window resized
    const auto width = m_scene->camera()->windowWidth();
    const auto height = m_scene->camera()->windowHeight();
    const bool window_resized = m_window_width != width || m_window_height != height;
    if ( window_resized ) { this->onWindowResized(); }

    // Object changed
    this->for_each_renderer( [&] ( Object* object, Renderer* renderer )
    {
        const bool object_changed = renderer->engine().object() != object;
        if ( object_changed )
        {
            this->onObjectChanged( object, renderer );
        }
    } );

    // LOD control.
    const auto repetitions = this->lod_control();

    // Ensemble rendering.
    this->setupEngines();
    this->setupBuffer();
    for ( size_t i = 0; i < repetitions; i++ )
    {
        this->bindBuffer();
        this->drawEngines();
        this->unbindBuffer();
    }
    this->drawBuffer();

    kvs::OpenGL::Finish();
    m_timer.stop();
}

void StochasticRenderingCompositor::for_each_renderer( Function function )
{
    const auto size = m_scene->IDManager()->size();
    for ( size_t i = 0; i < size; i++ )
    {
        auto id = m_scene->IDManager()->id( i );
        auto* object = m_scene->objectManager()->object( id.first );
        auto* r = m_scene->rendererManager()->renderer( id.second );
        if ( auto* renderer = Renderer::DownCast( r ) )
        {
            function( object, renderer );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns LOD-controlled repetitons.
 *  @return number of repetitions
 */
/*===========================================================================*/
size_t StochasticRenderingCompositor::lod_control()
{
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
    return repetitions;
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform matrix of the active object.
 *  @return xform matrix
 */
/*===========================================================================*/
kvs::Mat4 StochasticRenderingCompositor::object_xform()
{
    return m_scene->objectManager()->hasActiveObject() ?
        m_scene->objectManager()->activeObject()->xform().toMatrix() :
        m_scene->objectManager()->xform().toMatrix();
}

} // end of namespace kvs
