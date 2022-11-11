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
    MousePressEventListener(): kvs::EventListener( kvs::EventBase::MousePressEvent ) {}
    MousePressEventListener( MousePressEventFunc func ):
        kvs::EventListener( kvs::EventBase::MousePressEvent ) { this->update( func ); }
    virtual ~MousePressEventListener() = default;

    void update( MousePressEventFunc func ) { mousePressEvent( func ); }
    virtual void update( kvs::MouseEvent* event ) { mousePressEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::MouseEvent*>(event) ); }
};

} // end of namespace kvs
