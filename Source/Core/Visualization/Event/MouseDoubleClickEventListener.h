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
    MouseDoubleClickEventListener();
    MouseDoubleClickEventListener( MouseDoubleClickEvent e ) { this->update( e ); }
    virtual ~MouseDoubleClickEventListener();

    void update( MouseDoubleClickEvent e ) { mouseDoubleClickEvent( e ); }
    virtual void update( kvs::MouseEvent* event ) { mouseDoubleClickEvent( event ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
