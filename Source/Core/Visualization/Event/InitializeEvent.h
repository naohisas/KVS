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
    InitializeEvent() = default;
    virtual ~InitializeEvent() = default;
    int type() const { return kvs::EventBase::InitializeEvent; }
};

} // end of namespace kvs
