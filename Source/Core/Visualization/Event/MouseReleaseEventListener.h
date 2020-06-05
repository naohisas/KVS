/*****************************************************************************/
/**
 *  @file   MouseReleaseEventListener.h
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
 *  @brief  MouseReleaseEventListener class.
 */
/*===========================================================================*/
class MouseReleaseEventListener : public kvs::EventListener
{
public:
    MouseReleaseEventListener();
    virtual ~MouseReleaseEventListener();
    virtual void update( kvs::MouseEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
