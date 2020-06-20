/*****************************************************************************/
/**
 *  @file   MouseMoveEventListener.h
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
 *  @brief  MouseMoveEventListener class.
 */
/*===========================================================================*/
class MouseMoveEventListener : public kvs::EventListener
{
public:
    MouseMoveEventListener(): kvs::EventListener( kvs::EventBase::MouseMoveEvent ) {}
    MouseMoveEventListener( MouseMoveEventFunc func ) { this->update( func ); }
    virtual ~MouseMoveEventListener() {}

    void update( MouseMoveEventFunc func ) { mouseMoveEvent( func ); }
    virtual void update( kvs::MouseEvent* event ) { mouseMoveEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::MouseEvent*>(event) ); }
};

} // end of namespace kvs
