/*****************************************************************************/
/**
 *  @file   VideoObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VideoObject.h 1398 2012-12-05 09:33:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/ObjectBase>
#include <kvs/Module>
#include <kvs/opencv/CaptureDevice>
#include <string>


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Video object.
 */
/*===========================================================================*/
class VideoObject : public kvs::ObjectBase
{
    kvsModule( kvs::opencv::VideoObject, Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:
    enum PixelType
    {
        Gray8   =  8, ///< 8 bit gray pixel
        Color24 = 24  ///< 24 bit RGB color pixel (8x8x8 bits)
    };

private:
    kvs::opencv::CaptureDevice m_device; ///< video capture device
    PixelType m_type; ///< pixel type
    size_t m_width; ///< capture widht
    size_t m_height; ///< capture height
    size_t m_nchannels; ///< number of channels

public:
    VideoObject();
    VideoObject( const int device_id );
    virtual ~VideoObject() {}

    const kvs::opencv::CaptureDevice& device() const { return m_device; }
    PixelType type() const { return m_type; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    size_t numberOfChannels() const { return m_nchannels; }

    void shallowCopy( const VideoObject& other );

protected:
    bool createCaptureDevice( const int device_id );
    bool createCaptureDevice( const std::string& filename );
    bool initialize( const kvs::opencv::CaptureDevice& device );
};

} // end of namespace opencv

} // end of namespace kvs
