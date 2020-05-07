/****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.cpp
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
/****************************************************************************/
#include "ParticleBasedRenderer.h"
#include <kvs/ParticleBuffer>
#include <kvs/PointObject>
#include <kvs/Camera>
#include <kvs/Assert>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
ParticleBasedRenderer::ParticleBasedRenderer():
    m_ref_point( NULL ),
    m_enable_rendering( true ),
    m_subpixel_level( 1 ),
    m_buffer( NULL )
{
    BaseClass::setShader( kvs::Shader::Lambert() );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param point [in] pointer to the point object
 *  @param subpixel_level [in] subpixel level
 */
/*==========================================================================*/
ParticleBasedRenderer::ParticleBasedRenderer(
    const kvs::PointObject* point,
    const size_t subpixel_level ):
    m_ref_point( NULL ),
    m_enable_rendering( true ),
    m_subpixel_level( 1 ),
    m_buffer( NULL )
{
    BaseClass::setShader( kvs::Shader::Lambert() );
    this->setSubpixelLevel( subpixel_level );
    this->attachPointObject( point );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
ParticleBasedRenderer::~ParticleBasedRenderer()
{
    this->deleteParticleBuffer();
}

/*==========================================================================*/
/**
 *  Rendering.
 *  @param object [in] pointer to the object (PointObject)
 *  @param camera [in] pointet to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBasedRenderer::exec(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    if ( !m_enable_rendering ) return;

    kvs::PointObject* point = kvs::PointObject::DownCast( object );
    if ( !m_ref_point ) this->attachPointObject( point );
    if ( point->normals().size() == 0 ) BaseClass::disableShading();

    BaseClass::startTimer();
    {
        this->create_image( point, camera, light );
        BaseClass::drawImage();
        this->cleanParticleBuffer();
    }
    BaseClass::stopTimer();
}

/*==========================================================================*/
/**
 *  Create the point buffer.
 */
/*==========================================================================*/
bool ParticleBasedRenderer::createParticleBuffer(
    const size_t width,
    const size_t height,
    const size_t subpixel_level )
{
    const size_t dpr = BaseClass::devicePixelRatio();
    m_buffer = new kvs::ParticleBuffer( width, height, subpixel_level, dpr );
    if ( !m_buffer ) return( false );

    return( true );
}

/*==========================================================================*/
/**
 *  Clean the particle buffer.
 */
/*==========================================================================*/
void ParticleBasedRenderer::cleanParticleBuffer()
{
    m_buffer->clean();
}

/*==========================================================================*/
/**
 *  Delete the particle buffer.
 */
/*==========================================================================*/
void ParticleBasedRenderer::deleteParticleBuffer()
{
    if ( m_buffer ) { delete m_buffer; m_buffer = NULL; }
}

/*==========================================================================*/
/**
 *  Create the rendering image.
 *  @param point [in] pointer to the point object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBasedRenderer::create_image(
    const kvs::PointObject* point,
    const kvs::Camera* camera,
    const kvs::Light* light )
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
        BaseClass::setDevicePixelRatio( camera->devicePixelRatio() );
        const int framebuffer_width = BaseClass::framebufferWidth();
        const int framebuffer_height = BaseClass::framebufferHeight();
        const size_t npixels = framebuffer_width * framebuffer_height;
        BaseClass::allocateColorData( npixels * 4 );
        BaseClass::allocateDepthData( npixels );

        this->deleteParticleBuffer();
        this->createParticleBuffer( width, height, m_subpixel_level );
    }

    // Initialize the frame buffers.
    BaseClass::fillColorData( 0 );
    BaseClass::fillDepthData( 0 );

    this->project_particle( point, camera, light );
}

/*==========================================================================*/
/**
 *  Project the particles.
 *  @param point [in] pointer to the point object
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 */
/*==========================================================================*/
void ParticleBasedRenderer::project_particle(
    const kvs::PointObject* point,
    const kvs::Camera* camera,
    const kvs::Light* light )
{
    kvs::Xform pvm( camera->projectionMatrix() * camera->viewingMatrix() * point->modelingMatrix() );
    float t[16]; pvm.toArray( t );

    const size_t w = camera->windowWidth() / 2;
    const size_t h = camera->windowHeight() / 2;

    // Set shader initial parameters.
    BaseClass::shader().set( camera, light, point );

    // Attach the shader and the point object to the point buffer.
    m_buffer->attachShader( &BaseClass::shader() );
    m_buffer->attachPointObject( point );

    // Aliases.
    const size_t nv = point->numberOfVertices();
    const kvs::Real32* v = point->coords().data();

    size_t index3 = 0;
    const size_t bounds_width = BaseClass::windowWidth() - 1;
    const size_t bounds_height = BaseClass::windowHeight() - 1;
    for ( size_t index = 0; index < nv; index++, index3 += 3 )
    {
        /* Calculate the projected point position in the window coordinate system.
         * Ex.) Camera::projectObjectToWindow().
         */
        float p_tmp[4] = {
            v[index3]*t[0] + v[index3+1]*t[4] + v[index3+2]*t[ 8] + t[12],
            v[index3]*t[1] + v[index3+1]*t[5] + v[index3+2]*t[ 9] + t[13],
            v[index3]*t[2] + v[index3+1]*t[6] + v[index3+2]*t[10] + t[14],
            v[index3]*t[3] + v[index3+1]*t[7] + v[index3+2]*t[11] + t[15] };
        p_tmp[3] = 1.0f / p_tmp[3];
        p_tmp[0] *= p_tmp[3];
        p_tmp[1] *= p_tmp[3];
        p_tmp[2] *= p_tmp[3];

        const float p_win_x = ( 1.0f + p_tmp[0] ) * w;
        const float p_win_y = ( 1.0f + p_tmp[1] ) * h;
        const float depth   = ( 1.0f + p_tmp[2] ) * 0.5f;

        // Store the projected point in the point buffer.
        if ( ( 0 < p_win_x ) & ( 0 < p_win_y ) )
        {
            if ( ( p_win_x < bounds_width ) & ( p_win_y < bounds_height ) )
            {
                m_buffer->add( p_win_x, p_win_y, depth, index );
            }
        }
    }

    // Shading calculation.
    if ( BaseClass::isEnabledShading() ) m_buffer->enableShading();
    else m_buffer->disableShading();

    m_buffer->createImage( &BaseClass::colorData(), &BaseClass::depthData() );
}

} // end of namespace kvs
