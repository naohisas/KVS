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

public:

    enum Type
    {
        Stretching = 0,
        Centering = 1
    };

private:

    double m_initial_aspect_ratio; ///< initial aspect ratio
    double m_left; ///< screen left position
    double m_right; ///< screen right position
    double m_bottom; ///< screen bottom position
    double m_top; ///< screen top position
    Type m_type; ///< rendering type
    kvs::Texture2D m_texture; ///< texture image

public:

    ImageRenderer( const Type& type = ImageRenderer::Centering );
    virtual ~ImageRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:

    void create_texture( const kvs::ImageObject* image );
    void center_alignment( const double width, const double height );
};

} // end of namespace kvs

#endif // KVS__IMAGE_RENDERER_H_INCLUDE
