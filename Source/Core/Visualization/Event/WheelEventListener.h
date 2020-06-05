/*****************************************************************************/
/**
 *  @file   WheelEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>
#include <kvs/WheelEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  WheelEventListener class.
 */
/*===========================================================================*/
class WheelEventListener : public kvs::EventListener
{
public:
    WheelEventListener();
    virtual ~WheelEventListener();
    virtual void update( kvs::WheelEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
