/*****************************************************************************/
/**
 *  @file   MousePressEventListener.h
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
 *  @brief  MousePressEventListener class.
 */
/*===========================================================================*/
class MousePressEventListener : public kvs::EventListener
{
public:
    MousePressEventListener();
    MousePressEventListener( MousePressEvent e ) { this->update( e ); }
    virtual ~MousePressEventListener();

    void update( MousePressEvent e ) { mousePressEvent( e ); }
    virtual void update( kvs::MouseEvent* event ) { mousePressEvent( event ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
