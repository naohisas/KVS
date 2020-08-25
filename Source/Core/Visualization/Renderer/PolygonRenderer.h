/****************************************************************************/
/**
 *  @file   PolygonRenderer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>


namespace kvs
{

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
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing; ///< flag for multisample anti-aliasing (MSAA)
    mutable bool m_enable_two_side_lighting; ///< flag for two-side lighting
    float m_polygon_offset; ///< polygon offset

public:
    PolygonRenderer();
    virtual ~PolygonRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    float polygonOffset() const { return m_polygon_offset; }
    void setPolygonOffset( const float offset ) { m_polygon_offset = offset; }

    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;
    void enableTwoSideLighting() const;
    void disableTwoSideLighting() const;
    bool isTwoSideLighting() const;

private:
    void initialize();
};

} // end of namespace kvs

#include "PolygonRendererGLSL.h"
