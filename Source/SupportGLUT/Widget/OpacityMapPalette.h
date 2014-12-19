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
#include <kvs/glut/Rectangle>
#include <kvs/glut/ColorMapPalette>


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

protected:

    std::string m_caption; ///< caption
    kvs::OpacityMap m_opacity_map; ///< opacity map
    kvs::Texture1D m_texture; ///< opacity map texture
    kvs::Texture2D m_checkerboard; ///< checkerboard texture
    kvs::glut::Rectangle m_palette; ///< palette
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color
    kvs::Vector2i m_pressed_position; ///< mouse pressed position
    kvs::Vector2i m_previous_position; ///< mouse previous position
    const kvs::glut::ColorMapPalette* m_color_map_palette; ///< pointer to the color map palette

public:

    OpacityMapPalette( kvs::ScreenBase* screen = 0 );
    virtual ~OpacityMapPalette( void );

    virtual void screenUpdated( void ) {};
    virtual void screenResized( void ) {};

    const std::string& caption( void ) const;
    const kvs::glut::Rectangle& palette( void ) const;
    const kvs::OpacityMap opacityMap( void ) const;

    void setCaption( const std::string& caption );
    void setOpacityMap( const kvs::OpacityMap& opacity_map );

public:

    void paintEvent( void );
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:

    int get_fitted_width( void );
    int get_fitted_height( void );
    void initialize_texture( const kvs::OpacityMap& opacity_map );
    void initialize_checkerboard( void );
    void draw_palette( void );
    void draw_free_hand_line( kvs::MouseEvent* event );
    void draw_straight_line( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__OPACITY_MAP_PALETTE_H_INCLUDE
