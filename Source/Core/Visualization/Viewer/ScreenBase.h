/****************************************************************************/
/**
 *  @file   ScreenBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScreenBase.h 1570 2013-05-09 08:32:56Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <string>
#include <kvs/DisplayFormat>
#include <kvs/Deprecated>
#include <kvs/EventHandler>
#include <kvs/PaintDevice>
#include <kvs/ColorImage>

/*KVS_DEPRECATED*/
#include <kvs/InitializeEventListener>
#include <kvs/PaintEventListener>
#include <kvs/ResizeEventListener>
#include <kvs/MousePressEventListener>
#include <kvs/MouseMoveEventListener>
#include <kvs/MouseReleaseEventListener>
#include <kvs/MouseDoubleClickEventListener>
#include <kvs/WheelEventListener>
#include <kvs/KeyPressEventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Screen base class.
 */
/*===========================================================================*/
class ScreenBase
{
private:
    int m_x; ///< window position (y position)
    int m_y; ///< window position (x position)
    int m_width; ///< window size (width)
    int m_height; ///< window size (height)
    std::string m_title; ///< window title
    kvs::DisplayFormat m_display_format; ///< display format
    kvs::EventHandler* m_event_handler; ///< event handler
    kvs::PaintDevice* m_paint_device; ///< paint device
    float m_device_pixel_ratio; ///< device pixel ratio

public:
    ScreenBase();
    virtual ~ScreenBase();

    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    const std::string& title() const { return m_title; }
    const kvs::DisplayFormat& displayFormat() const { return m_display_format; }
    kvs::EventHandler* eventHandler() { return m_event_handler; }
    kvs::PaintDevice* paintDevice() { return m_paint_device; }
    float devicePixelRatio() const { return m_device_pixel_ratio; }

    void setPosition( const int x, const int y ) { m_x = x; m_y = y; }
    void setSize( const int width, const int height ) { m_width = width; m_height = height; }
    void setGeometry( const int x, const int y, const int width, const int height ) { m_x = x; m_y = y; m_width = width; m_height = height; }
    void setTitle( const std::string& title ) { m_title = title; }
    void setDisplayFormat( const kvs::DisplayFormat& display_format ) { m_display_format = display_format; }

    void addEvent( kvs::EventListener* event, const std::string& name = "" );
    void removeEvent( const kvs::EventListener* event );
    void removeEvent( const std::string& name );

    virtual void create() {}
    virtual void show() {}
    virtual void hide() {}
    virtual void showFullScreen() {}
    virtual void showNormal() {}
    virtual void popUp() {}
    virtual void pushDown() {}
    virtual void redraw() {}
    virtual void resize( int, int ) {}
    virtual void draw() {}
    virtual bool isFullScreen() const { return false; }
    virtual kvs::ColorImage capture() const { return kvs::ColorImage( this->width(), this->height() ); }
    virtual void enable() {}
    virtual void disable() {}
    virtual void reset() {}

    KVS_DEPRECATED( void addInitializeEvent( kvs::InitializeEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addPaintEvent( kvs::PaintEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addResizeEvent( kvs::ResizeEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addMousePressEvent( kvs::MousePressEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addMouseMoveEvent( kvs::MouseMoveEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addMouseReleaseEvent( kvs::MouseReleaseEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addMouseDoubleClickEvent( kvs::MouseDoubleClickEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addWheelEvent( kvs::WheelEventListener* event ) ) { this->addEvent( event ); }
    KVS_DEPRECATED( void addKeyPressEvent( kvs::KeyPressEventListener* event ) ) { this->addEvent( event ); }

protected:
    void setDevicePixelRatio( const float ratio ) { m_device_pixel_ratio = ratio; }
};

} // end of namespace kvs
