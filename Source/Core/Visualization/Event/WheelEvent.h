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
    int m_x = 0; ///< x coordinate value of mouse cursor position
    int m_y = 0; ///< y coordinate value of mouse cursor position
    int m_direction = 0; ///< scroll direction

public:
    WheelEvent() = default;
    virtual ~WheelEvent() = default;

    int x() const { return m_x; }
    int y() const { return m_y; }
    int direction() const { return m_direction; }
    int type() const { return kvs::EventBase::WheelEvent; }

    void setPosition( int x, int y ) { m_x = x; m_y = y; }
    void setDirection( int direction ) { m_direction = direction; }
};

} // end of namespace kvs
