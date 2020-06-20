/*****************************************************************************/
/**
 *  @file   ResizeEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Resize event class.
 */
/*===========================================================================*/
class ResizeEvent : public kvs::EventBase
{
private:
    int m_width; ///< window width
    int m_height; ///< window height

public:
    ResizeEvent( const ResizeEvent& e ):
        m_width( e.width() ),
        m_height( e.height() ) {}
    ResizeEvent( int width, int height ):
        m_width( width ),
        m_height( height ) {}
    ResizeEvent() {}
    virtual ~ResizeEvent() {}

    int width() const { return m_width; }
    int height() const { return m_height; }
    void setSize( int width, int height ) { m_width = width; m_height = height; }

    int type() const { return kvs::EventBase::ResizeEvent; }
};

} // end of namespace kvs
