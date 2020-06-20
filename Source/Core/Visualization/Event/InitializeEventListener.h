/*****************************************************************************/
/**
 *  @file   InitializeEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  InitializeEventListener class.
 */
/*===========================================================================*/
class InitializeEventListener : public kvs::EventListener
{
public:
    InitializeEventListener();
    InitializeEventListener( InitializeEvent e ) { this->update( e ); }
    virtual ~InitializeEventListener();

    void update( InitializeEvent e ) { initializeEvent( e ); }
    virtual void update() { initializeEvent(); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
