/*****************************************************************************/
/**
 *  @file   PaintEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PaintEvent class.
 */
/*===========================================================================*/
class PaintEvent : public kvs::EventBase
{
public:
    PaintEvent() = default;
    virtual ~PaintEvent() = default;
    int type() const { return kvs::EventBase::PaintEvent; }
};

} // end of namespace kvs
