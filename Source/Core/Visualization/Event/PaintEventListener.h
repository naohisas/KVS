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
    PaintEventListener();
    PaintEventListener( PaintEvent e ) { this->update( e ); }
    virtual ~PaintEventListener();

    void update( PaintEvent e ) { paintEvent( e ); }
    virtual void update() { paintEvent(); }

private:
    void onEvent( kvs::EventBase* event = 0 );
};

} // end of namespace kvs
