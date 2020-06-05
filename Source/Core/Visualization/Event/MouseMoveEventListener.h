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
    virtual ~MouseMoveEventListener();
    virtual void update( kvs::MouseEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
