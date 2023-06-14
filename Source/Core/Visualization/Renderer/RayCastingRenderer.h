/****************************************************************************/
/**
 *  @file   RayCastingRenderer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    float m_step = 0.5f; ///< sampling step
    float m_opaque = 0.97f; ///< opaque value for early ray termination
    size_t m_ray_width = 1; ///< ray width
    bool m_enable_lod = false; ///< enable LOD rendering
    float m_modelview[16] = {0}; ///< modelview matrix

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
