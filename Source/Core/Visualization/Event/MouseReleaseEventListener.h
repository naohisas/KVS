/*****************************************************************************/
/**
 *  @file   MouseReleaseEventListener.h
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
 *  @brief  MouseReleaseEventListener class.
 */
/*===========================================================================*/
class MouseReleaseEventListener : public kvs::EventListener
{
public:
    MouseReleaseEventListener();
    MouseReleaseEventListener( MouseReleaseEvent e ) { this->update( e ); }
    virtual ~MouseReleaseEventListener();

    void update( MouseReleaseEvent e ) { mouseReleaseEvent( e ); }
    virtual void update( kvs::MouseEvent* event ) { mouseReleaseEvent( event ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
