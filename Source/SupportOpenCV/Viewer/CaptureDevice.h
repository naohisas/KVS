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

protected:

    CvCapture* m_handler; ///< handler

public:

    CaptureDevice( void );

    ~CaptureDevice( void );

public:

    bool create( const int index );

    bool create( const std::string filename );

    void release( void );

public:

    const CvCapture* handler( void );

    const double frameWidth( void );

    const double frameHeight( void );

    const double frameRate( void );

public:

    const int grabFrame( void ) const;

    const IplImage* retrieveFrame( void ) const;

    const IplImage* queryFrame( void ) const;
};

} // end of namespace opencv

} // end of namespace kvs

#endif // KVS__OPENCV__CAPTURE_DEVICE_H_INCLUDE
