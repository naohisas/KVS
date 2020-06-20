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
    MouseMoveEventListener();
    MouseMoveEventListener( MouseMoveEvent e ) { this->update( e ); }
    virtual ~MouseMoveEventListener();

    void update( MouseMoveEvent e ) { mouseMoveEvent( e ); }
    virtual void update( kvs::MouseEvent* event ) { mouseMoveEvent( event ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
