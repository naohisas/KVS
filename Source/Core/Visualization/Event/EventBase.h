/*****************************************************************************/
/**
 *  @file   EventBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
        InitializeEvent       = kvsBinary12(0000,0000,0001), //    1
        PaintEvent            = kvsBinary12(0000,0000,0010), //    2
        ResizeEvent           = kvsBinary12(0000,0000,0100), //    4
        MousePressEvent       = kvsBinary12(0000,0000,1000), //    8
        MouseMoveEvent        = kvsBinary12(0000,0001,0000), //   16
        MouseReleaseEvent     = kvsBinary12(0000,0010,0000), //   32
        MouseDoubleClickEvent = kvsBinary12(0000,0100,0000), //   64
        WheelEvent            = kvsBinary12(0000,1000,0000), //  128
        TimerEvent            = kvsBinary12(0001,0000,0000), //  256
        KeyPressEvent         = kvsBinary12(0010,0000,0000), //  512
        KeyRepeatEvent        = kvsBinary12(0100,0000,0000), // 1024
        KeyReleaseEvent       = kvsBinary12(1000,0000,0000), // 2048
        AllEvents             = kvsBinary12(1111,1111,1111)
    };

public:
    EventBase() = default;
    virtual ~EventBase() = default;
    virtual int type() const = 0;
};

} // end of namespace kvs
