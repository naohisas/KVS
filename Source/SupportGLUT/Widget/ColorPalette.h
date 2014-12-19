/*****************************************************************************/
/**
 *  @file   ColorPalette.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorPalette.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__COLOR_PALETTE_H_INCLUDE
#define KVS__GLUT__COLOR_PALETTE_H_INCLUDE

#include <string>
#include <kvs/glut/WidgetBase>
#include <kvs/glut/Rectangle>


namespace kvs
{

class ScreenBase;
class MouseEvent;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Color palette class.
 */
/*===========================================================================*/
class ColorPalette : public kvs::glut::WidgetBase
{
public:

    typedef kvs::glut::WidgetBase BaseClass;

protected:

    std::string m_caption; ///< caption
    kvs::glut::Rectangle m_SV_palette; ///< SV (saturation/value) palette
    kvs::glut::Rectangle m_H_palette; ///< H (hue) palette
    kvs::glut::Rectangle m_selected_color_box; ///< selected color box
    int m_H_indicator; ///< hue indicator
    int m_S_indicator; ///< saturation indicator
    int m_V_indicator; ///< value indicator
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color

public:

    ColorPalette( kvs::ScreenBase* screen = 0 );
    virtual ~ColorPalette( void );

    virtual void screenUpdated( void ) {};
    virtual void screenResized( void ) {};

    const std::string& caption( void ) const;
    const kvs::RGBColor color( void ) const;

    void setCaption( const std::string& caption );

public:

    void paintEvent( void );
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:

    void draw_SV_palette( void );
    void draw_H_palette( void );
    void draw_selected_color_box( void );
    void draw_selected_color_value( void );
    int get_fitted_width( void );
    int get_fitted_height( void );
    float get_H_value( void ) const;
    float get_S_value( void ) const;
    float get_V_value( void ) const;
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__COLOR_PALETTE_H_INCLUDE
