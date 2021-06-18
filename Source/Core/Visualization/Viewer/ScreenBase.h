/****************************************************************************/
/**
 *  @file   ScreenBase.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/DisplayFormat>
#include <kvs/Deprecated>
#include <kvs/EventHandler>
#include <kvs/PaintDevice>
#include <kvs/ColorImage>


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
    bool m_visible; ///< visibility of the screen
    bool m_fullscreen; ///< flag for fullscreen display mode

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

    virtual void setEvent( kvs::EventListener* event, const std::string& name = "" );
    virtual void addEvent( kvs::EventListener* event, const std::string& name = "" );
    virtual void removeEvent( const kvs::EventListener* event );
    virtual void removeEvent( const std::string& name );

    virtual void create() {}
    virtual void show() { m_visible = true; }
    virtual void hide() { m_visible = false; }
    virtual void showFullScreen() { m_visible = true; m_fullscreen = true; }
    virtual void showNormal() { m_visible = true; m_fullscreen = false; }
    virtual void popUp() {}
    virtual void pushDown() {}
    virtual void redraw() {}
    virtual void resize( int, int ) {}
    virtual void draw() {}
    virtual bool isFullScreen() const { return m_fullscreen; }
    virtual bool isVisible() const { return m_visible; }
    virtual kvs::ColorImage capture() const { return kvs::ColorImage( this->width(), this->height() ); }

    virtual void enable() {}
    virtual void disable() {}
    virtual void reset() {}

protected:
    void setDevicePixelRatio( const float ratio ) { m_device_pixel_ratio = ratio; }
};

} // end of namespace kvs
