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
    WheelEventListener( WheelEvent e ) { this->update( e ); }
    virtual ~WheelEventListener();

    void update( WheelEvent e ) { wheelEvent( e ); }
    virtual void update( kvs::WheelEvent* event ) { wheelEvent( event ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
