/*****************************************************************************/
/**
 *  @file   ResizeEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>
#include <kvs/ResizeEvent>


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
    ResizeEventListener(): kvs::EventListener( kvs::EventBase::ResizeEvent ) {}
    ResizeEventListener( ResizeEvent e ) { this->update( e ); }
    virtual ~ResizeEventListener() {}

    void update( ResizeEvent e ) { resizeEvent( e ); }
    virtual void update( int width, int height ) { resizeEvent( width, height ); }

private:
    void onEvent( kvs::EventBase* event )
    {
        auto* e = static_cast<kvs::ResizeEvent*>(event);
        this->update( e->width(), e->height() );
    }
};

} // end of namespace kvs
