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
    ResizeEvent();
    ResizeEvent( const ResizeEvent& event );
    ResizeEvent( int width, int height );
    virtual ~ResizeEvent();

    int width() const;
    int height() const;
    int type() const;

    void setSize( int width, int height );
};

} // end of namespace kvs
