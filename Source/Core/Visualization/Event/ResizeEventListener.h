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
    virtual ~ResizeEventListener();
    virtual void update( int width, int height ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
