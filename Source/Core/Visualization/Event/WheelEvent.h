/*****************************************************************************/
/**
 *  @file   WheelEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
private:
    int m_x; ///< x coordinate value of mouse cursor position
    int m_y; ///< y coordinate value of mouse cursor position
    int m_direction; ///< scroll direction

public:
    WheelEvent() {}
    virtual ~WheelEvent() {}

    int x() const { return m_x; }
    int y() const { return m_y; }
    int direction() const { return m_direction; }
    int type() const { return kvs::EventBase::WheelEvent; }

    void setPosition( int x, int y ) { m_x = x; m_y = y; }
    void setDirection( int direction ) { m_direction = direction; }
};

} // end of namespace kvs
