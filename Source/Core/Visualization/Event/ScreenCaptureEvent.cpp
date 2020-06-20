/*****************************************************************************/
/**
 *  @file   ScreenCaptureEvent.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ScreenCaptureEvent.h"
#include <kvs/KeyEvent>
#include <kvs/Scene>
#include <kvs/Camera>
#include <kvs/Date>
#include <kvs/Time>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ScreenCaptureEvent class.
 *  @param  key [in] key for capturing
 */
/*===========================================================================*/
ScreenCaptureEvent::ScreenCaptureEvent( const int key ):
    m_key( key ),
    m_filename( "" ),
    m_basename( "screenshot" ),
    m_capture_func( nullptr )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ScreenCaptureEvent class.
 *  @param  func [in] capture function
 *  @param  key [in] key for capturing
 */
/*===========================================================================*/
ScreenCaptureEvent::ScreenCaptureEvent( CaptureFunc func, const int key ):
    m_key( key ),
    m_filename( "" ),
    m_basename( "screenshot" ),
    m_capture_func( func )
{
}

/*===========================================================================*/
/**
 *  @brief  Captures screen image.
 *  @param  event [in] pointer to key event
 */
/*===========================================================================*/
void ScreenCaptureEvent::update( kvs::KeyEvent* event )
{
    KVS_ASSERT( scene() != NULL );

    if ( event->key() == m_key )
    {
        auto image = scene()->camera()->snapshot();
        if ( m_capture_func ) { m_capture_func( image ); }
        else { image.write( this->output_filename() ); }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns output image filename.
 *  @return output image filename
 */
/*===========================================================================*/
std::string ScreenCaptureEvent::output_filename() const
{
    std::string filename = m_filename;
    if ( filename.empty() )
    {
        kvs::Date today;
        kvs::Time now;
        filename = m_basename + "_" + today.toString("") + now.toString("") + ".bmp";
    }
    return filename;
}

} // end of namespace kvs
