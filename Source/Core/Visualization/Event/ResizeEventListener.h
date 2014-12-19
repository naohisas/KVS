/*****************************************************************************/
/**
 *  @file   ResizeEventListener.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResizeEventListener.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__RESIZE_EVENT_LISTENER_H_INCLUDE
#define KVS__RESIZE_EVENT_LISTENER_H_INCLUDE

#include <kvs/EventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  ResizeEventListener class.
 */
/*===========================================================================*/
class ResizeEventListener : public kvs::EventListener
{
public:

    ResizeEventListener();
    virtual ~ResizeEventListener();

    virtual void update( int width, int height ) = 0;

private:

    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs

#endif // KVS__RESIZE_EVENT_LISTENER_H_INCLUDE
