/*****************************************************************************/
/**
 *  @file   IdleEventListener.h
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
 *  @brief  IdleEventListener class.
 */
/*===========================================================================*/
class IdleEventListener : public kvs::EventListener
{
public:
    IdleEventListener(): kvs::EventListener( kvsBinary12(1111,1111,1111) ) {}
    virtual ~IdleEventListener() {}
    virtual void update() = 0;

private:
    void onEvent( kvs::EventBase* event = 0 ) { this->update(); }
};

} // end of namespace kvs
