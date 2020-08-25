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
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing; ///< flag for multisample anti-aliasing (MSAA)

public:
    LineRenderer();
    virtual ~LineRenderer();

    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void initialize();
};

} // end of namespace kvs

#include "LineRendererGLSL.h"
