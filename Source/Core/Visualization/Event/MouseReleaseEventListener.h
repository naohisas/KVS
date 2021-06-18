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
    MouseReleaseEventListener(): kvs::EventListener( kvs::EventBase::MouseReleaseEvent ) {}
    MouseReleaseEventListener( MouseReleaseEventFunc func ):
        kvs::EventListener( kvs::EventBase::MouseReleaseEvent ) { this->update( func ); }
    virtual ~MouseReleaseEventListener() {}

    void update( MouseReleaseEventFunc func ) { mouseReleaseEvent( func ); }
    virtual void update( kvs::MouseEvent* event ) { mouseReleaseEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::MouseEvent*>(event) ); }
};

} // end of namespace kvs
