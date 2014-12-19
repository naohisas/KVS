/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.h
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
#ifndef KVS__PARTICLE_BASED_RENDERER_H_INCLUDE
#define KVS__PARTICLE_BASED_RENDERER_H_INCLUDE

#include <kvs/VolumeRendererBase>
#include <kvs/ParticleBuffer>
#include <kvs/Module>
#include <kvs/Deprecated>


namespace kvs
{

class ParticleBufferCompositor;

/*==========================================================================*/
/**
 *  Particle based volume renderer.
 */
/*==========================================================================*/
class ParticleBasedRenderer : public kvs::VolumeRendererBase
{
    friend class kvs::ParticleBufferCompositor;

    kvsModule( kvs::ParticleBasedRenderer, Renderer );
    kvsModuleBaseClass( kvs::VolumeRendererBase );

protected:

    // Reference data (NOTE: not allocated in thie class).
    const kvs::PointObject* m_ref_point; ///< pointer to the point data

    bool m_enable_rendering; ///< rendering flag
    size_t m_subpixel_level; ///< number of divisions in a pixel
    kvs::ParticleBuffer* m_buffer; ///< particle buffer

public:

    ParticleBasedRenderer();
    ParticleBasedRenderer( const kvs::PointObject* point, const size_t subpixel_level = 1 );
    virtual ~ParticleBasedRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void attachPointObject( const kvs::PointObject* point ) { m_ref_point = point; }
    void setSubpixelLevel( const size_t subpixel_level ) { m_subpixel_level = subpixel_level; }
    const kvs::ParticleBuffer* particleBuffer() const { return m_buffer; }
    size_t subpixelLevel() const { return m_subpixel_level; }
    void enableRendering() { m_enable_rendering = true; }
    void disableRendering() { m_enable_rendering = false; }

protected:

    bool createParticleBuffer( const size_t width, const size_t height, const size_t subpixel_level );
    void cleanParticleBuffer();
    void deleteParticleBuffer();

private:

    void create_image( const kvs::PointObject* point, const kvs::Camera* camera, const kvs::Light* light );
    void project_particle( const kvs::PointObject* point, const kvs::Camera* camera, const kvs::Light* light );

public:
    KVS_DEPRECATED( void initialize() ) { m_enable_rendering = true; m_subpixel_level = 1; m_buffer = NULL; }
};

} // end of namespace kvs

#include "ParticleBasedRendererGLSL.h"

#endif // KVS__PARTICLE_BASED_RENDERER_H_INCLUDE
