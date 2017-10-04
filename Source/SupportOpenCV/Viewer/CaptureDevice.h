/*****************************************************************************/
/**
 *  @file   CaptureDevice.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CaptureDevice.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__OPENCV__CAPTURE_DEVICE_H_INCLUDE
#define KVS__OPENCV__CAPTURE_DEVICE_H_INCLUDE

#include <kvs/opencv/OpenCV>
#include <kvs/ClassName>


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Capture device class.
 */
/*===========================================================================*/
class CaptureDevice
{
    kvsClassName_without_virtual( kvs::opencv::CaptureDevice );

private:
    CvCapture* m_handler; ///< handler

public:
    CaptureDevice();
    ~CaptureDevice( );

    const CvCapture* handler();
    double property( const int property_id ) const;
    int setProperty( const int property_id, const double value ) const;

    double frameWidth() const;
    double frameHeight() const;
    double frameRate() const;
    double numberOfFrames() const;

    double currentPosition() const;
    double relativePosition() const;
    double nextFrameIndex() const;

    int setCurrentPosition( const double msec ) const;
    int setRelativePosition( const double pos ) const;
    int setNextFrameIndex( const double index ) const;

    bool create( const int index );
    bool create( const std::string filename );
    void release();

    int grabFrame() const;
    const IplImage* retrieveFrame() const;
    const IplImage* queryFrame() const;
};

} // end of namespace opencv

} // end of namespace kvs

#endif // KVS__OPENCV__CAPTURE_DEVICE_H_INCLUDE
