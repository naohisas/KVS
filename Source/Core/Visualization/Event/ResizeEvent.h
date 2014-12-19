/*****************************************************************************/
/**
 *  @file   ResizeEvent.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResizeEvent.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__RESIZE_EVENT_H_INCLUDE
#define KVS__RESIZE_EVENT_H_INCLUDE

#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Resize event class.
 */
/*===========================================================================*/
class ResizeEvent : public kvs::EventBase
{
protected:

    int m_width; ///< window width
    int m_height; ///< window height

public:

    ResizeEvent();
    ResizeEvent( const ResizeEvent& event );
    ResizeEvent( int width, int height );
    virtual ~ResizeEvent();

    int width() const;
    int height() const;
    int type() const;

    void setSize( int width, int height );
};

} // end of namespace kvs

#endif // KVS__RESIZE_EVENT_H_INCLUDE
