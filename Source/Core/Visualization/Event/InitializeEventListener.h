/*****************************************************************************/
/**
 *  @file   InitializeEventListener.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: InitializeEventListener.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__INTITALIZE_EVENT_LISTENER_H_INCLUDE
#define KVS__INTITALIZE_EVENT_LISTENER_H_INCLUDE

#include <kvs/EventListener>
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  InitializeEventListener class.
 */
/*===========================================================================*/
class InitializeEventListener : public kvs::EventListener
{
public:

    InitializeEventListener();
    virtual ~InitializeEventListener();

    virtual void update() = 0;

private:

    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs

#endif // KVS__INTITALIZE_EVENT_LISTENER_H_INCLUDE
