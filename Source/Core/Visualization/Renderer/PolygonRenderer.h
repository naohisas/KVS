/****************************************************************************/
/**
 *  @file   PolygonRenderer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/Deprecated>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Polygon renderer.
 */
/*==========================================================================*/
class PolygonRenderer : public kvs::RendererBase
{
    kvsModule( kvs::PolygonRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing = false; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing = false; ///< flag for multisample anti-aliasing (MSAA)
    mutable bool m_enable_two_side_lighting = true; ///< flag for two-side lighting
    kvs::Vec2 m_depth_offset{ 0.0f, 0.0f }; ///< depth offset {factor, units}

public:
    PolygonRenderer() = default;
    virtual ~PolygonRenderer() = default;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const kvs::Vec2& depthOffset() const { return m_depth_offset; }
    bool isTwoSideLightingEnabled() const { return m_enable_two_side_lighting; }
    void setDepthOffset( const kvs::Vec2& offset ) { m_depth_offset = offset; }
    void setDepthOffset( const float factor, const float units = 0.0f );
    void setAntiAliasingEnabled( const bool enable = true, const bool multisample = false ) const;
    void setTwoSideLightingEnabled( const bool enable = true ) const;
    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;
    void enableTwoSideLighting() const;
    void disableTwoSideLighting() const;
    bool isTwoSideLighting() const;

private:
    void initialize();

public:
    KVS_DEPRECATED( float polygonOffset() const ) { return m_depth_offset[0]; }
    KVS_DEPRECATED( void setPolygonOffset( const float offset ) ) { m_depth_offset[0] = offset; }
};

} // end of namespace kvs

#include "PolygonRendererGLSL.h"
