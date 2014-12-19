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
#ifndef KVS__OPENCV__VIDEO_RENDERER_H_INCLUDE
#define KVS__OPENCV__VIDEO_RENDERER_H_INCLUDE

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
    kvsModuleName( kvs::opencv::VideoRenderer );
    kvsModuleCategory( Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

public:

    enum Type
    {
        Stretching = 0,
        Centering  = 1
    };

protected:

    double         m_initial_aspect_ratio; ///< initial aspect ratio
    double         m_left;                 ///< screen left position
    double         m_right;                ///< screen right position
    double         m_bottom;               ///< screen bottom position
    double         m_top;                  ///< screen top position
    Type           m_type;                 ///< rendering type
    kvs::Texture2D m_texture;              ///< texture image

public:

    VideoRenderer( const Type type = VideoRenderer::Centering );

    virtual ~VideoRenderer( void );

public:

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:

    void create_texture( const kvs::opencv::VideoObject* video );

    void centering( const double width, const double height );
};

} // end of namespace opencv

} // end of namespace kvs

#endif // KVS__OPENCV__VIDEO_RENDERER_H_INCLUDE
