/****************************************************************************/
/**
 *  @file   RayCastingRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RayCastingRenderer.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__RAY_CASTING_RENDERER_H_INCLUDE
#define KVS__RAY_CASTING_RENDERER_H_INCLUDE

#include <kvs/VolumeRendererBase>
#include <kvs/TransferFunction>
#include <kvs/StructuredVolumeObject>
#include <kvs/Module>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Ray casting volume renderer.
 */
/*==========================================================================*/
class RayCastingRenderer : public kvs::VolumeRendererBase
{
    kvsModule( kvs::RayCastingRenderer, Renderer );
    kvsModuleBaseClass( kvs::VolumeRendererBase );

private:

    float m_step; ///< sampling step
    float m_opaque; ///< opaque value for early ray termination
    size_t m_ray_width; ///< ray width
    bool m_enable_lod; ///< enable LOD rendering
    float m_modelview[16]; ///< modelview matrix

public:

    RayCastingRenderer();
    RayCastingRenderer( const kvs::TransferFunction& tfunc );
    template <typename ShadingType>
    RayCastingRenderer( const ShadingType shader );

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setSamplingStep( const float step ) { m_step = step; }
    void setOpaqueValue( const float opaque ) { m_opaque = opaque; }
    void enableLODControl( const size_t ray_width = 3 ) { m_enable_lod = true; m_ray_width = ray_width; }
    void disableLODControl() { m_enable_lod = false; m_ray_width = 1; }

private:

    template <typename T>
    void rasterize(
        const kvs::StructuredVolumeObject* volume,
        const kvs::Camera* camera,
        const kvs::Light* light );

public:
    KVS_DEPRECATED( void enableCoarseRendering( const size_t ray_width = 3 ) ) { m_ray_width = ray_width; }
    KVS_DEPRECATED( void disableCoarseRendering() ) { m_ray_width = 1; }
};

} // end of namespace kvs

#include "RayCastingRendererGLSL.h"

#endif // KVS__RAY_CASTING_RENDERER_H_INCLUDE
