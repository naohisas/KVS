/*****************************************************************************/
/**
 *  @file   CaptureDevice.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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

public:
#if ( CV_MAJOR_VERSION > 3 )
    using Handler = cv::VideoCapture;
    struct Frame
    {
        cv::Mat mat{};
        int width = 0;
        int height = 0;
        int nChannels = 0;
        int depth = 0;
        unsigned char* imageData = nullptr;
    };
#else
    using Handler = CvCapture;
    using Frame = IplImage;
#endif

private:
    Handler* m_handler = nullptr; ///< handler
#if ( CV_MAJOR_VERSION > 3 )
    mutable Frame m_frame{}; ///< captured frame
#endif

public:
    CaptureDevice() = default;
    ~CaptureDevice() { this->release(); }

    const Handler* handler() { return m_handler; }
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

    bool create( const int index = 0 );
    bool create( const std::string filename );
    void release();

    int grabFrame() const;
    const Frame* retrieveFrame() const;
    const Frame* queryFrame() const;
};

} // end of namespace opencv

} // end of namespace kvs
