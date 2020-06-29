/*****************************************************************************/
/**
 *  @file   PaintEventListener.h
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
 *  @brief  PaintEventListener class.
 */
/*===========================================================================*/
class PaintEventListener : public kvs::EventListener
{
public:
    PaintEventListener(): kvs::EventListener( kvs::EventBase::PaintEvent ) {}
    PaintEventListener( PaintEventFunc func ):
        kvs::EventListener( kvs::EventBase::PaintEvent ) { this->update( func ); }
    virtual ~PaintEventListener() {}

    void update( PaintEventFunc func ) { paintEvent( func ); }
    virtual void update() { paintEvent(); }

private:
    void onEvent( kvs::EventBase* event = 0 ) { this->update(); }
};

} // end of namespace kvs
