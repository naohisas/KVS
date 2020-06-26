#pragma once
#include <string>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/OpacityMap>
#include <kvs/WidgetBase>


namespace kvs
{

class ScreenBase;
class MouseEvent;

/*===========================================================================*/
/**
 *  @brief  Opacity map palette class.
 */
/*===========================================================================*/
class OpacityMapPalette : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    std::string m_caption; ///< caption
    kvs::OpacityMap m_opacity_map; ///< opacity map
    kvs::Texture1D m_texture; ///< opacity map texture
    kvs::Texture2D m_checkerboard; ///< checkerboard texture
    kvs::WidgetBase m_palette; ///< palette
    kvs::Vec2i m_pressed_position; ///< mouse pressed position
    kvs::Vec2i m_previous_position; ///< mouse previous position
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    OpacityMapPalette( kvs::ScreenBase* screen = 0 );
    virtual ~OpacityMapPalette();

    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

    const std::string& caption() const { return m_caption; }
    const kvs::WidgetBase& palette() const { return m_palette; }
    const kvs::OpacityMap opacityMap() const;
    void setCaption( const std::string& caption ) { m_caption = caption; }
    void setOpacityMap( const kvs::OpacityMap& opacity_map );
    void update() { this->initialize_texture( m_opacity_map ); }

    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void initialize_texture( const kvs::OpacityMap& opacity_map );
    void initialize_checkerboard();
    void draw_palette();
    void draw_free_hand_line( kvs::MouseEvent* event );
    void draw_straight_line( kvs::MouseEvent* event );
    void draw_border( const kvs::Rectangle& rect );
};

} // end of namespace kvs
