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
    InitializeEventListener(): kvs::EventListener( kvs::EventBase::InitializeEvent ) {}
    InitializeEventListener( InitializeEventFunc func ):
        kvs::EventListener( kvs::EventBase::InitializeEvent ) { this->update( func ); }
    virtual ~InitializeEventListener() {}

    void update( InitializeEventFunc func ) { initializeEvent( func ); }
    virtual void update() { initializeEvent(); }

private:
    void onEvent( kvs::EventBase* /* event */ ) { this->update(); }
};

} // end of namespace kvs
