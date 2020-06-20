/*****************************************************************************/
/**
 *  @file   InitializeEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  InitializeEvent class.
 */
/*===========================================================================*/
class InitializeEvent : public kvs::EventBase
{
public:
    InitializeEvent() {}
    virtual ~InitializeEvent() {}
    int type() const { return kvs::EventBase::InitializeEvent; }
};

} // end of namespace kvs
