/*****************************************************************************/
/**
 *  @file   MouseDoubleClickEventListener.h
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
 *  @brief  MouseDoubleClickEventListener class.
 */
/*===========================================================================*/
class MouseDoubleClickEventListener : public kvs::EventListener
{
public:
    MouseDoubleClickEventListener();
    virtual ~MouseDoubleClickEventListener();
    virtual void update( kvs::MouseEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
