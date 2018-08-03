/*****************************************************************************/
/**
 *  @file   OpacityMapPalette.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpacityMapPalette.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__OPACITY_MAP_PALETTE_H_INCLUDE
#define KVS__GLUT__OPACITY_MAP_PALETTE_H_INCLUDE

#include <string>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/OpacityMap>
#include <kvs/glut/WidgetBase>


namespace kvs
{

class ScreenBase;
class MouseEvent;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Opacity map palette class.
 */
/*===========================================================================*/
class OpacityMapPalette : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;

private:
    std::string m_caption; ///< caption
    kvs::OpacityMap m_opacity_map; ///< opacity map
    kvs::Texture1D m_texture; ///< opacity map texture
    kvs::Texture2D m_checkerboard; ///< checkerboard texture
    kvs::glut::WidgetBase m_palette; ///< palette
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color
    kvs::Vec2i m_pressed_position; ///< mouse pressed position
    kvs::Vec2i m_previous_position; ///< mouse previous position

public:
    OpacityMapPalette( kvs::ScreenBase* screen = 0 );
    virtual ~OpacityMapPalette();

    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::string& caption() const { return m_caption; }
    const kvs::glut::WidgetBase& palette() const { return m_palette; }
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
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__OPACITY_MAP_PALETTE_H_INCLUDE
