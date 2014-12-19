/*****************************************************************************/
/**
 *  @file   EventBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: EventBase.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__EVENT_BASE_H_INCLUDE
#define KVS__EVENT_BASE_H_INCLUDE

#include <kvs/Binary>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Event base class.
 */
/*===========================================================================*/
class EventBase
{
public:

    enum EventType
    {
        InitializeEvent       = kvsBinary12(0000,0000,0001),
        PaintEvent            = kvsBinary12(0000,0000,0010),
        ResizeEvent           = kvsBinary12(0000,0000,0100),
        MousePressEvent       = kvsBinary12(0000,0000,1000),
        MouseMoveEvent        = kvsBinary12(0000,0001,0000),
        MouseReleaseEvent     = kvsBinary12(0000,0010,0000),
        MouseDoubleClickEvent = kvsBinary12(0000,0100,0000),
        WheelEvent            = kvsBinary12(0000,1000,0000),
        TimerEvent            = kvsBinary12(0001,0000,0000),
        KeyPressEvent         = kvsBinary12(0010,0000,0000),
        AllEvents             = kvsBinary12(0011,1111,1111)
    };

public:

    EventBase();
    virtual ~EventBase();

    virtual int type() const = 0;
};

} // end of namespace kvs

#endif // KVS__EVENT_BASE_H_INCLUDE
