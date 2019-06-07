/*****************************************************************************/
/**
 *  @file   VideoRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VideoRenderer.h 1398 2012-12-05 09:33:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Texture2D>
#include <kvs/Module>
#include <kvs/opencv/VideoObject>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Video renderer.
 */
/*===========================================================================*/
class VideoRenderer : public kvs::RendererBase
{
    kvsModule( kvs::opencv::VideoRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    double m_initial_aspect_ratio; ///< initial aspect ratio
    double m_left; ///< screen left position
    double m_right; ///< screen right position
    double m_bottom; ///< screen bottom position
    double m_top; ///< screen top position
    bool m_enable_centering; ///< enable center alignment
    bool m_enable_mirroring; ///< enable mirror mapping
    kvs::Texture2D m_texture; ///< texture image

public:
    VideoRenderer();

    bool isEnabledCentering() const { return m_enable_centering; }
    bool isEnabledMirroring() const { return m_enable_mirroring; }
    void setEnabledCentering( const bool enable ) { m_enable_centering = enable; }
    void setEnabledMirroring( const bool enable ) { m_enable_mirroring = enable; }
    void enableCentering() { this->setEnabledCentering( true ); }
    void enableMirroring() { this->setEnabledMirroring( true ); }
    void disableCentering() { this->setEnabledCentering( false ); }
    void disableMirroring() { this->setEnabledMirroring( false ); }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Texture2D& texture() { return m_texture; }
    void createTexture( const kvs::opencv::VideoObject* video );
    void alignCenter( const kvs::Camera* camera );
    void textureMapping();
};

} // end of namespace opencv

} // end of namespace kvs
