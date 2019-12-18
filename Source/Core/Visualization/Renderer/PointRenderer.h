/****************************************************************************/
/**
 *  @file   PointRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PointRenderer.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
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
 *  Point renderer.
 */
/*==========================================================================*/
class PointRenderer : public kvs::RendererBase
{
    kvsModule( kvs::PointRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing; ///< flag for multisample anti-aliasing (MSAA)
    mutable bool m_enable_two_side_lighting; ///< flag for two-side lighting

public:
    PointRenderer();
    virtual ~PointRenderer();

    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;
    void enableTwoSideLighting() const;
    void disableTwoSideLighting() const;
    bool isTwoSideLighting() const;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void initialize();
};

} // end of namespace kvs

#include "PointRendererGLSL.h"
