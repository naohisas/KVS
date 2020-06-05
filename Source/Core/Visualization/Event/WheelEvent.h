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
