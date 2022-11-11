/*****************************************************************************/
/**
 *  @file   ScreenCaptureEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/KeyPressEventListener>
#include <string>
#include <functional>
#include <kvs/ColorImage>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Screen capture event class.
 */
/*===========================================================================*/
class ScreenCaptureEvent : public kvs::KeyPressEventListener
{
public:
    using CaptureFunc = std::function<void(const kvs::ColorImage&)>;

private:
    int m_key = 0; ///< key for capturing screen image
    std::string m_filename = ""; ///< filename of captured image
    std::string m_basename = "screenshot"; ///< basename of captured image
    CaptureFunc m_capture_func = nullptr;

public:
    ScreenCaptureEvent( const int key = kvs::Key::s );
    ScreenCaptureEvent( CaptureFunc func, const int key = kvs::Key::s );

    void setKey( const int key ) { m_key = key; }
    void setFilename( const std::string& filename ) { m_filename = filename; }
    void setBasename( const std::string& basename ) { m_basename = basename; }
    void update( CaptureFunc func ) { m_capture_func = func; }
    void update( kvs::KeyEvent* event );

private:
    std::string output_filename() const;
};

} // end of namespace kvs
