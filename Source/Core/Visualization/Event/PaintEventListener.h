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
    virtual ~PaintEventListener();
    virtual void update() = 0;

private:
    void onEvent( kvs::EventBase* event = 0 );
};

} // end of namespace kvs
