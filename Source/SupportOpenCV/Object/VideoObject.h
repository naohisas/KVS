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
#ifndef KVS__OPENCV__VIDEO_OBJECT_H_INCLUDE
#define KVS__OPENCV__VIDEO_OBJECT_H_INCLUDE

#include <kvs/ObjectBase>
#include <kvs/Module>
#include <kvs/opencv/CaptureDevice>


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
    kvsModuleName( kvs::opencv::VideoObject );
    kvsModuleCategory( Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:
    enum PixelType
    {
        Gray8   =  8, ///< 8 bit gray pixel
        Color24 = 24  ///< 24 bit RGB color pixel (8x8x8 bits)
    };

private:
    int m_device_id; ///< capture device ID
    kvs::opencv::CaptureDevice m_device; ///< video capture device
    PixelType m_type; ///< pixel type
    size_t m_width; ///< capture widht
    size_t m_height; ///< capture height
    size_t m_nchannels; ///< number of channels

public:
    VideoObject();
    VideoObject( const int device_id );
    virtual ~VideoObject() {}

    ObjectType objectType() const;
    int deviceID() const { return m_device_id; }
    const kvs::opencv::CaptureDevice& device() const { return m_device; }
    PixelType type() const { return m_type; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    size_t nchannels() const { return m_nchannels; }

public:
    const bool initialize( const size_t device_id );
};

} // end of namespace opencv

} // end of namespace kvs

#endif // KVS__OPENCV__VIDEO_OBJECT_H_INCLUDE
