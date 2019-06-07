/****************************************************************************/
/**
 *  @file   ImageRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageRenderer.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__IMAGE_RENDERER_H_INCLUDE
#define KVS__IMAGE_RENDERER_H_INCLUDE

#include "RendererBase.h"
#include <kvs/Texture2D>
#include <kvs/ImageObject>
#include <kvs/Module>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Image renderer class.
 */
/*==========================================================================*/
class ImageRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ImageRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    double m_initial_aspect_ratio; ///< initial aspect ratio
    double m_left; ///< screen left position
    double m_right; ///< screen right position
    double m_bottom; ///< screen bottom position
    double m_top; ///< screen top position
    bool m_enable_centering; ///< enable center alignment
    kvs::Texture2D m_texture; ///< texture image

public:
    ImageRenderer();

    bool isEnabledCentering() const { return m_enable_centering; }
    void setEnabledCentering( const bool enable ) { m_enable_centering = enable; }
    void enableCentering() { this->setEnabledCentering( true ); }
    void disableCentering() { this->setEnabledCentering( false ); }
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Texture2D& texture() { return m_texture; }
    void createTexture( const kvs::ImageObject* image );
    void alignCenter( const kvs::Camera* camera );
    void textureMapping();
};

} // end of namespace kvs

#endif // KVS__IMAGE_RENDERER_H_INCLUDE
