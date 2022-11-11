/*****************************************************************************/
/**
 *  @file   TimeEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  TimeEvent class.
 */
/*===========================================================================*/
class TimeEvent : public kvs::EventBase
{
public:
    TimeEvent() = default;
    virtual ~TimeEvent() = default;
    int type() const { return kvs::EventBase::TimerEvent; }
};

} // end of namespace kvs
