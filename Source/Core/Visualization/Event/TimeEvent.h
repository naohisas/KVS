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
    TimeEvent();
    virtual ~TimeEvent();
    int type() const;
};

} // end of namespace kvs
