/*****************************************************************************/
/**
 *  @file   ScreenCaptureEvent.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__SCREEN_CAPTURE_EVENT_H_INCLUDE
#define KVS__SCREEN_CAPTURE_EVENT_H_INCLUDE

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

#endif // KVS__SCREEN_CAPTURE_EVENT_H_INCLUDE
