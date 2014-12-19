/*****************************************************************************/
/**
 *  @file   ScreenCaptureEvent.cpp
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
 */
/*===========================================================================*/
ScreenCaptureEvent::ScreenCaptureEvent():
    m_key( kvs::Key::s ),
    m_filename( "" ),
    m_basename( "screenshot" )
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
        std::string filename = m_filename;
        if ( filename.empty() )
        {
            kvs::Date today;
            kvs::Time now;
            filename = m_basename + "_" + today.toString("") + now.toString("") + ".bmp";
        }

        scene()->camera()->snapshot().write( filename );
    }
}

} // end of namespace kvs
