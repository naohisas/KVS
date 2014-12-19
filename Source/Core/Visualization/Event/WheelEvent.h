/*****************************************************************************/
/**
 *  @file   WheelEvent.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WheelEvent.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__WHEEL_EVENT_H_INCLUDE
#define KVS__WHEEL_EVENT_H_INCLUDE

#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  WheelEvent class.
 */
/*===========================================================================*/
class WheelEvent : public kvs::EventBase
{
protected:

    int m_x; ///< x coordinate value of mouse cursor position
    int m_y; ///< y coordinate value of mouse cursor position
    int m_direction; ///< scroll direction

public:

    WheelEvent();
    virtual ~WheelEvent();

    int x() const;
    int y() const;
    int direction() const;
    int type() const;

    void setPosition( int x, int y );
    void setDirection( int direction );
};

} // end of namespace kvs

#endif // KVS__WHEEL_EVENT_H_INCLUDE
