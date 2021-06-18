/*****************************************************************************/
/**
 *  @file   StochasticRendererBase.cpp
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/*****************************************************************************/
#include "StochasticRendererBase.h"
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticRendererBase class.
 *  @param  engine [in] pointer to the stochastic rendering engine
 */
/*===========================================================================*/
StochasticRendererBase::StochasticRendererBase( kvs::StochasticRenderingEngine* engine ):
    m_shader( new kvs::Shader::Lambert() ),
    m_engine( engine )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the StochasticRendererBase class.
 */
/*===========================================================================*/
StochasticRendererBase::~StochasticRendererBase()
{
    if ( m_shader ) delete m_shader;
    if ( m_engine ) delete m_engine;
}

/*===========================================================================*/
/**
 *  @brief  Releases object resouces from GPU.
 */
/*===========================================================================*/
void StochasticRendererBase::release()
{
    KVS_ASSERT( m_engine );

    if ( m_engine->object() )
    {
        m_engine->detachObject();
        m_engine->release();
    }
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camra
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticRendererBase::exec(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();
    if ( this->isWindowCreated() )
    {
        this->setWindowSize( width, height );
        this->setDevicePixelRatio( camera->devicePixelRatio() );
        this->setModelView( kvs::OpenGL::ModelViewMatrix() );
        this->setLightPosition( light->position() );

        const auto frame_width = this->framebufferWidth();
        const auto frame_height = this->framebufferHeight();
        this->createEnsembleBuffer( frame_width, frame_height );
        this->createEngine( object, camera, light );
    }

    if ( this->isWindowResized( width, height ) )
    {
        this->setWindowSize( width, height );

        // Update ensemble buffer
        this->ensembleBuffer().release();
        const auto frame_width = this->framebufferWidth();
        const auto frame_height = this->framebufferHeight();
        this->createEnsembleBuffer( frame_width, frame_height );

        // Update engine
        this->engine().update( object, camera, light );
    }

    if ( this->isObjectChanged( object ) )
    {
        // Clear ensemble buffer
        this->ensembleBuffer().clear();

        // Recreate engine
        this->engine().release();
        this->createEngine( object, camera, light );
    }

    this->setupEngine( object, camera, light );

    // Ensemble rendering.
    const auto m = kvs::OpenGL::ModelViewMatrix();
    const auto l = light->position();
    const size_t r = this->controllledRepetitions( m, l );
    for ( size_t i = 0; i < r; i++ )
    {
        // Render to the ensemble buffer.
        m_ensemble_buffer.bind();
        m_engine->draw( object, camera, light );
        m_engine->countRepetitions();
        m_ensemble_buffer.unbind();

        // Progressive averaging.
        m_ensemble_buffer.add();
    }

    // Render to the framebuffer.
    m_ensemble_buffer.draw();

    kvs::OpenGL::Finish();
    stopTimer();
}

size_t StochasticRendererBase::controllledRepetitions(
    const kvs::Mat4& modelview,
    const kvs::Vec3& light_position )
{
    size_t repetitions = m_repetition_level;
    if ( m_light_position != light_position || m_modelview != modelview )
    {
        if ( m_enable_lod )
        {
            repetitions = m_coarse_level;
        }
        m_light_position = light_position;
        m_modelview = modelview;
        m_ensemble_buffer.clear();
    }

    return repetitions;
}

void StochasticRendererBase::createEnsembleBuffer(
    const size_t frame_width,
    const size_t frame_height )
{
    m_ensemble_buffer.create( frame_width, frame_height );
    m_ensemble_buffer.clear();
}

void StochasticRendererBase::createEngine(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    m_engine->setShader( &shader() );
    m_engine->setRepetitionLevel( m_repetition_level );
    m_engine->setShadingEnabled( kvs::RendererBase::isShadingEnabled() );
    m_engine->create( object, camera, light );
}

void StochasticRendererBase::setupEngine(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    const bool reset_count = !m_enable_refinement;
    if ( reset_count )
    {
        m_engine->resetRepetitions();
        m_ensemble_buffer.clear();
    }

    m_engine->setup( object, camera, light );
}

} // end of namespace kvs
