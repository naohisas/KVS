/*****************************************************************************/
/**
 *  @file   ScreenCaptureEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/KeyPressEventListener>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Screen capture event class.
 */
/*===========================================================================*/
class ScreenCaptureEvent : public kvs::KeyPressEventListener
{
private:
    int m_key; ///< key for capturing screen image
    std::string m_filename; ///< filename of captured image
    std::string m_basename; ///< basename of captured image

public:
    ScreenCaptureEvent();

    void setKey( const int key ) { m_key = key; }
    void setFilename( const std::string& filename ) { m_filename = filename; }
    void setBasename( const std::string& basename ) { m_basename = basename; }
    void update( kvs::KeyEvent* event );
};

} // end of namespace kvs
