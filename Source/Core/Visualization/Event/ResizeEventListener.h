/*****************************************************************************/
/**
 *  @file   ResizeEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  ResizeEventListener class.
 */
/*===========================================================================*/
class ResizeEventListener : public kvs::EventListener
{
public:
    ResizeEventListener();
    ResizeEventListener( ResizeEvent e ) { this->update( e ); }
    virtual ~ResizeEventListener();

    void update( ResizeEvent e ) { resizeEvent( e ); }
    virtual void update( int width, int height ) { resizeEvent( width, height ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
