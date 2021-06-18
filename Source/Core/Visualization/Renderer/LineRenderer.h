/****************************************************************************/
/**
 *  @file   LineRenderer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Line renderer.
 */
/*==========================================================================*/
class LineRenderer : public kvs::RendererBase
{
    kvsModule( kvs::LineRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing = false; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing = false; ///< flag for multisample anti-aliasing (MSAA)
    kvs::Vec2 m_depth_offset{ 0.0f, 0.0f }; ///< depth offset {factor, units}

public:
    LineRenderer() { BaseClass::disableShading(); }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const kvs::Vec2& depthOffset() const { return m_depth_offset; }
    void setDepthOffset( const kvs::Vec2& offset ) { m_depth_offset = offset; }
    void setDepthOffset( const float factor, const float units = 0.0f );
    void setAntiAliasingEnabled( const bool enable = true, const bool multisample = false ) const;
    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;

private:
    void initialize();
};

} // end of namespace kvs

#include "LineRendererGLSL.h"
