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
            this->render_objects();
        }
        else
        {
            m_scene->updateGLModelingMatrix();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Renders the objects with stochastic renderers.
 */
/*===========================================================================*/
void StochasticRenderingCompositor::render_objects()
{
    m_timer.start();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    // Check for window/object state changes
    if ( this->is_window_created() ) { this->onWindowCreated(); }
    if ( this->is_window_resized() ) { this->onWindowResized(); }
    this->for_each_object( [&] ( Object* object, Renderer* renderer )
    {
        if ( this->is_object_changed( object, renderer ) )
        {
            this->onObjectChanged( object, renderer );
        }
    } );

    // Ensemble rendering.
    this->firstRenderPass( m_ensemble_buffer );
    this->for_each_ensemble( [&] ( kvs::EnsembleAverageBuffer& buffer )
    {
        this->ensembleRenderPass( buffer );
    } );
    this->lastRenderPass( m_ensemble_buffer );

    kvs::OpenGL::Finish();
    m_timer.stop();
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

void StochasticRenderingCompositor::firstRenderPass( kvs::EnsembleAverageBuffer& buffer )
{
    this->setupEngines();
    const auto reset_count = !this->isRefinementEnabled();
    if ( reset_count ) buffer.clear();
}

void StochasticRenderingCompositor::ensembleRenderPass( kvs::EnsembleAverageBuffer& buffer )
{
    buffer.bind();
    this->drawEngines();
    buffer.unbind();
    buffer.add();
}

void StochasticRenderingCompositor::lastRenderPass( kvs::EnsembleAverageBuffer& buffer )
{
    buffer.draw();
}

void StochasticRenderingCompositor::createEngines()
{
    this->for_each_object( [&] ( Object* object, Renderer* renderer )
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
    this->for_each_object( [&] ( Object* object, Renderer* renderer )
    {
        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( object );
        renderer->engine().update( object, m_scene->camera(), m_scene->light() );
        kvs::OpenGL::PopMatrix();
    } );
}

void StochasticRenderingCompositor::setupEngines()
{
    this->for_each_object( [&] ( Object* object, Renderer* renderer )
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
    this->for_each_object( [&] ( Object* object, Renderer* renderer )
    {
        kvs::OpenGL::PushMatrix();
        m_scene->updateGLModelingMatrix( object );
        renderer->engine().draw( object, m_scene->camera(), m_scene->light() );
        renderer->engine().countRepetitions();
        kvs::OpenGL::PopMatrix();
    } );
}

bool StochasticRenderingCompositor::is_window_created() const
{
    return m_window_width == 0 && m_window_height == 0;
}

bool StochasticRenderingCompositor::is_window_resized() const
{
    const auto width = m_scene->camera()->windowWidth();
    const auto height = m_scene->camera()->windowHeight();
    return m_window_width != width || m_window_height != height;
}

bool StochasticRenderingCompositor::is_object_changed( Object* object, Renderer* renderer )
{
    return renderer->engine().object() != object;
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
