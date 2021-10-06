/*****************************************************************************/
/**
 *  @file   CaptureDevice.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "CaptureDevice.h"


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Returns the specified capture property.
 *  @param  property_id [i] property ID
 *  @return specified capture property
 */
/*===========================================================================*/
double CaptureDevice::property( const int property_id ) const
{
#if ( CV_MAJOR_VERSION > 3 )
    return m_handler->get( property_id );
#else
    return cvGetCaptureProperty( m_handler, property_id );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Set a capture property.
 *  @param  property_id [i] property ID
 *  @param  value [i] value of the property
 *  @return 1 (true) if success, 0 otherwise
 */
/*===========================================================================*/
int CaptureDevice::setProperty( const int property_id, const double value ) const
{
#if ( CV_MAJOR_VERSION > 3 )
    return m_handler->set( property_id, value );
#else
    return cvSetCaptureProperty( m_handler, property_id, value );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns the frame width.
 *  @return frame width
 */
/*===========================================================================*/
double CaptureDevice::frameWidth() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_FRAME_WIDTH );
#else
    return this->property( CV_CAP_PROP_FRAME_WIDTH );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns the frame height.
 *  @return frame height
 */
/*===========================================================================*/
double CaptureDevice::frameHeight() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_FRAME_HEIGHT );
#else
    return this->property( CV_CAP_PROP_FRAME_HEIGHT );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns the frame rate.
 *  @return frame rate
 */
/*===========================================================================*/
double CaptureDevice::frameRate() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_FPS );
#else
    return this->property( CV_CAP_PROP_FPS );
#endif
}

double CaptureDevice::numberOfFrames() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_FRAME_COUNT );
#else
    return this->property( CV_CAP_PROP_FRAME_COUNT );
#endif
}

double CaptureDevice::currentPosition() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_POS_MSEC );
#else
    return this->property( CV_CAP_PROP_POS_MSEC );
#endif
}

double CaptureDevice::relativePosition() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_POS_AVI_RATIO );
#else
    return this->property( CV_CAP_PROP_POS_AVI_RATIO );
#endif
}

double CaptureDevice::nextFrameIndex() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->property( cv::CAP_PROP_POS_FRAMES );
#else
    return this->property( CV_CAP_PROP_POS_FRAMES );
#endif
}

int CaptureDevice::setCurrentPosition( const double msec ) const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->setProperty( cv::CAP_PROP_POS_MSEC, msec );
#else
    return this->setProperty( CV_CAP_PROP_POS_MSEC, msec );
#endif
}

int CaptureDevice::setRelativePosition( const double pos ) const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->setProperty( cv::CAP_PROP_POS_AVI_RATIO, pos );
#else
    return this->setProperty( CV_CAP_PROP_POS_AVI_RATIO, pos );
#endif
}

int CaptureDevice::setNextFrameIndex( const double index ) const
{
#if ( CV_MAJOR_VERSION > 3 )
    return this->setProperty( cv::CAP_PROP_POS_FRAMES, index );
#else
    return this->setProperty( CV_CAP_PROP_POS_FRAMES, index );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Initializes capturing video from camera.
 *  @param  index [in] index of the camera
 *  @return true, if the the capture device is initialized successfully
 */
/*===========================================================================*/
bool CaptureDevice::create( const int index )
{
#if ( CV_MAJOR_VERSION > 3 )
    m_handler = new Handler( index );
#else
    m_handler = cvCreateCameraCapture( index );
#endif
    return m_handler != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Initializes capturing video from file.
 *  @param  filename [in] video file name
 *  @return true, if the the capture device is initialized successfully
 */
/*===========================================================================*/
bool CaptureDevice::create( const std::string filename )
{
#if ( CV_MAJOR_VERSION > 3 )
    m_handler = new Handler( filename );
#else
    m_handler = cvCreateFileCapture( filename.c_str() );
#endif
    return m_handler != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Releases the capture device.
 */
/*===========================================================================*/
void CaptureDevice::release()
{
#if ( CV_MAJOR_VERSION > 3 )
    if ( m_handler ) { m_handler->release(); delete m_handler; }
#else
    if ( m_handler ) cvReleaseCapture( &m_handler );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Grabs frame and the grabbed frame is stored internally.
 */
/*===========================================================================*/
int CaptureDevice::grabFrame() const
{
#if ( CV_MAJOR_VERSION > 3 )
    return m_handler->grab();
#else
    return cvGrabFrame( m_handler );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the grabbed image.
 *  @return pointer to the grabbed image
 */
/*===========================================================================*/
const CaptureDevice::Frame* CaptureDevice::retrieveFrame() const
{
#if ( CV_MAJOR_VERSION > 3 )
    m_handler->retrieve( m_frame.mat );
    if ( m_frame.mat.empty() ) return nullptr;

    m_frame.width = m_frame.mat.cols;
    m_frame.height = m_frame.mat.rows;
    m_frame.nChannels = m_frame.mat.channels();
    m_frame.depth = m_frame.mat.depth();
    m_frame.imageData = m_frame.mat.data;
    return &m_frame;
#else
    return cvRetrieveFrame( m_handler );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Grabs image and returns the pointer to the image.
 *  @return pointer to the grabbed image
 */
/*===========================================================================*/
const CaptureDevice::Frame* CaptureDevice::queryFrame() const
{
#if ( CV_MAJOR_VERSION > 3 )
    m_handler->read( m_frame.mat );
    if ( m_frame.mat.empty() ) return nullptr;

    m_frame.width = m_frame.mat.cols;
    m_frame.height = m_frame.mat.rows;
    m_frame.nChannels = m_frame.mat.channels();
    m_frame.depth = m_frame.mat.depth();
    m_frame.imageData = m_frame.mat.data;
    return &m_frame;
#else
    return cvQueryFrame( m_handler );
#endif
}

} // end of namespace opencv

} // end of namespace kvs
