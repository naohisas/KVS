/*****************************************************************************/
/**
 *  @file   MouseDoubleClickEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>
#include <kvs/MouseEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  MouseDoubleClickEventListener class.
 */
/*===========================================================================*/
class MouseDoubleClickEventListener : public kvs::EventListener
{
public:
    MouseDoubleClickEventListener(): kvs::EventListener( kvs::EventBase::MouseDoubleClickEvent ) {}
    MouseDoubleClickEventListener( MouseDoubleClickEventFunc func ) { this->update( func ); }
    virtual ~MouseDoubleClickEventListener() {}

    void update( MouseDoubleClickEventFunc func ) { mouseDoubleClickEvent( func ); }
    virtual void update( kvs::MouseEvent* event ) { mouseDoubleClickEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::MouseEvent*>(event) ); }
};

} // end of namespace kvs
