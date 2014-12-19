/****************************************************************************/
/**
 *  @file   ParticleBufferCompositor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBufferCompositor.cpp 1802 2014-08-07 09:22:11Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ParticleBufferCompositor.h"
#include <kvs/DebugNew>
#include <kvs/Vector3>
#include <kvs/Matrix33>
#include <kvs/OpenGL>
#include <kvs/IgnoreUnusedVariable>


// If the following macro is set to 1, the active object will be only renderered.
#define TEST__RENDERING_ACTIVE_OBJECT_ONLY 0


namespace
{

void UpdateGLModelingMatrix( const kvs::ObjectBase* object )
{
    float X[16]; object->xform().toArray( X );
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::MultMatrix( X );
}

}

namespace kvs
{

/*==========================================================================*/
/**
 *  Construct a new class.
 *  @param object_manager [in] pointer to the object manager
 *  @param renderer_manager [in] pointer to the renderer manager
 *  @param id_manager [in] pointer to the ID manager
 */
/*==========================================================================*/
ParticleBufferCompositor::ParticleBufferCompositor(
    kvs::ObjectManager* object_manager,
    kvs::RendererManager* renderer_manager,
    kvs::IDManager* id_manager ):
    kvs::VolumeRendererBase(),
    m_accumulation_time( 0.0 ),
    m_num_projected_particles( 0 ),
    m_num_stored_particles( 0 ),
    m_subpixel_level( 1 ),
    m_object_manager( object_manager ),
    m_renderer_manager( renderer_manager ),
    m_id_manager( id_manager ),
    m_accumulator( 0 )
{
    const kvs::Vector3f obj_min = m_object_manager->minObjectCoord();
    const kvs::Vector3f obj_max = m_object_manager->maxObjectCoord();
    const kvs::Vector3f ext_min = m_object_manager->minExternalCoord();
    const kvs::Vector3f ext_max = m_object_manager->maxExternalCoord();
    kvs::PointObject* object = new kvs::PointObject(); // Dummy object
    object->setXform( m_object_manager->xform() );
    object->saveXform();
    object->setMinMaxObjectCoords( obj_min, obj_max );
    object->setMinMaxExternalCoords( ext_min, ext_max );

    const size_t object_id = m_object_manager->insert( object );
    const size_t renderer_id = m_renderer_manager->insert( this );
    m_id_manager->insert( object_id, renderer_id );
}

/*==========================================================================*/
/**
 *  Destruct this class.
 */
/*==========================================================================*/
ParticleBufferCompositor::~ParticleBufferCompositor()
{
    this->delete_accumulator();
}

/*==========================================================================*/
/**
 *  Rendering method.
 *  @param object [in] pointer to the object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBufferCompositor::exec(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( object );
    if ( m_point_object_list.size() == 0 ) return;

    BaseClass::startTimer();
    {
        this->create_image( camera, light );
        BaseClass::drawImage();
        this->clean_accumulator();
    }
    BaseClass::stopTimer();
}

/*==========================================================================*/
/**
 *  Link a point object and a particle volume renderer.
 *  @param object [in] pointer to the point object
 *  @param renderer [in] pointer to the particle volume renderer
 */
/*==========================================================================*/
void ParticleBufferCompositor::link(
    kvs::PointObject* object,
    kvs::ParticleVolumeRenderer* renderer )
{
    if ( m_point_object_list.size() == 0 )
    {
        m_subpixel_level = renderer->subpixelLevel();
    }
    else
    {
        if ( m_subpixel_level != renderer->subpixelLevel() )
        {
            renderer->deleteParticleBuffer();
            renderer->setSubpixelLevel( m_subpixel_level );
        }
    }

    renderer->disableRendering();

    m_point_object_list.push_back( object );
    m_point_renderer_list.push_back( renderer );
}

/*==========================================================================*/
/**
 *  Clear the object list and the renderer list.
 */
/*==========================================================================*/
void ParticleBufferCompositor::clearList()
{
    m_point_object_list.clear();
    m_point_renderer_list.clear();
}

/*==========================================================================*/
/**
 *  Create the accumulator.
 */
/*==========================================================================*/
bool ParticleBufferCompositor::create_accumulator()
{
    const size_t width = BaseClass::windowWidth();
    const size_t height = BaseClass::windowHeight();
    const size_t level = m_subpixel_level;

    const size_t nrenderers = m_point_renderer_list.size();
    for ( size_t i = 0; i < nrenderers; i++ )
    {
        m_point_renderer_list[i]->createParticleBuffer( width, height, level );
    }

    m_accumulator = new kvs::ParticleBufferAccumulator( width, height, level );

    return m_accumulator ? true : false;
}

/*==========================================================================*/
/**
 *  Clean the accumulator.
 */
/*==========================================================================*/
void ParticleBufferCompositor::clean_accumulator()
{
#if TEST__RENDERING_ACTIVE_OBJECT_ONLY
    const size_t nrenderers = m_point_renderer_list.size();
    for ( size_t i = 0; i < nrenderers; i++ )
    {
        kvs::PointObject* object = m_point_object_list[i];
        if ( m_object_manager->hasActiveObject() )
        {
            if ( m_object_manager->activeObject() != object )
            {
                m_point_renderer_list[i]->clean_point_buffer();
            }
        }
    }
#endif

    m_accumulator->clean();
}

/*==========================================================================*/
/**
 *  Delete the accumulator.
 */
/*==========================================================================*/
void ParticleBufferCompositor::delete_accumulator()
{
    if ( m_accumulator )
    {
        const size_t nrenderers = m_point_renderer_list.size();
        for ( size_t i = 0; i < nrenderers; i++ )
        {
            m_point_renderer_list[i]->deleteParticleBuffer();
        }

        delete m_accumulator;
    }
}

/*==========================================================================*/
/**
 *  Create image.
 *
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBufferCompositor::create_image( kvs::Camera* camera, kvs::Light* light )
{
    // Current rendering window size.
    const size_t current_width = BaseClass::windowWidth();
    const size_t current_height = BaseClass::windowHeight();

    // Updated rendering window size
    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();

    // Create memory region for the buffers, if the screen size is changed.
    if ( ( current_width != width ) || ( current_height != height ) )
    {
        BaseClass::setWindowSize( width, height );
        BaseClass::allocateColorData( width * height * 4 );
        BaseClass::allocateDepthData( width * height );

        this->delete_accumulator();
        this->create_accumulator();
    }

    this->accumulate( camera, light );
}

/*==========================================================================*/
/**
 *  Accumulate point buffers.
 *
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBufferCompositor::accumulate( kvs::Camera* camera, kvs::Light* light )
{
    kvs::Timer timer;
    m_accumulation_time = 0.0;

    const size_t nobjects = m_point_object_list.size();
    for ( size_t id = 0; id < nobjects; id++ )
    {
        kvs::PointObject* object = m_point_object_list[id];
        kvs::ParticleVolumeRenderer* renderer = m_point_renderer_list[id];

        if ( !object->isShown() ) continue;

        // Update the point buffer of the renderer.
        if ( m_object_manager->hasActiveObject() )
        {
            if ( m_object_manager->activeObject() == object ||
                 renderer->particleBuffer()->numberOfStoredParticles() == 0 )
            {
                m_point_renderer_list[id]->cleanParticleBuffer();
                this->update_particle_buffer( object, renderer, camera, light );
            }
        }
        else
        {
            m_point_renderer_list[id]->cleanParticleBuffer();
            this->update_particle_buffer( object, renderer, camera, light );
        }

        // Accumulate the point buffer.
        timer.start();
        m_accumulator->accumulate( id, renderer->particleBuffer() );
        timer.stop();
        m_accumulation_time += timer.msec();
    }

    m_accumulator->createImage(
        m_point_object_list,
        m_point_renderer_list,
        &BaseClass::colorData(),
        &BaseClass::depthData() );

    m_num_projected_particles = m_accumulator->numberOfProjectedParticles();
    m_num_stored_particles = m_accumulator->numberOfStoredParticles();
}

/*==========================================================================*/
/**
 *  Update the particle buffers.
 *  @param object [in] pointer to the point object
 *  @param renderer [in] pointer to the particle volume renderer
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBufferCompositor::update_particle_buffer(
    kvs::PointObject* object,
    kvs::ParticleVolumeRenderer* renderer,
    kvs::Camera* camera,
    kvs::Light* light )
{
    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::PushMatrix();

    UpdateGLModelingMatrix( object );
    renderer->create_image( object, camera, light );

    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::PushMatrix();
}

} // end of namespace kvs
