/*****************************************************************************/
/**
 *  @file   PaintEventListener.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PaintEventListener.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__PAINT_EVENT_LISTENER_H_INCLUDE
#define KVS__PAINT_EVENT_LISTENER_H_INCLUDE

#include <kvs/EventListener>
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PaintEventListener class.
 */
/*===========================================================================*/
class PaintEventListener : public kvs::EventListener
{
public:

    PaintEventListener();
    virtual ~PaintEventListener();

    virtual void update() = 0;

private:

    void onEvent( kvs::EventBase* event = 0 );
};

} // end of namespace kvs

#endif // KVS__PAINT_EVENT_LISTENER_H_INCLUDE
