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
    virtual ~MousePressEventListener();
    virtual void update( kvs::MouseEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
