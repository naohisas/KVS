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
    WheelEventListener(): kvs::EventListener( kvs::EventBase::WheelEvent ) {}
    WheelEventListener( WheelEventFunc func ):
        kvs::EventListener( kvs::EventBase::WheelEvent ) { this->update( func ); }
    virtual ~WheelEventListener() {}

    void update( WheelEventFunc func ) { wheelEvent( func ); }
    virtual void update( kvs::WheelEvent* event ) { wheelEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::WheelEvent*>(event) ); }
};

} // end of namespace kvs
