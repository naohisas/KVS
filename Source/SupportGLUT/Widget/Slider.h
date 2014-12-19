/*****************************************************************************/
/**
 *  @file   Slider.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Slider.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__SLIDER_H_INCLUDE
#define KVS__GLUT__SLIDER_H_INCLUDE

#include "WidgetBase.h"
#include <kvs/RGBColor>


namespace kvs
{

class ScreenBase;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Slider widget.
 */
/*===========================================================================*/
class Slider : public kvs::glut::WidgetBase
{
public:

    typedef kvs::glut::WidgetBase BaseClass;

protected:

    std::string m_caption; ///< caption
    float m_value; ///< value
    float m_min_value; ///< min. value
    float m_max_value; ///< max. value
    bool m_change_value; ///< value change flag
    bool m_show_range_value; ///< range value showing flag
    kvs::RGBColor m_slider_color; ///< slider (cursor) color
    kvs::RGBColor m_clicked_slider_color; ///< clicked slider (cursor) color
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color

public:

    Slider( kvs::ScreenBase* screen = 0 );

    virtual void sliderPressed( void ){};
    virtual void sliderMoved( void ){};
    virtual void sliderReleased( void ){};
    virtual void valueChanged( void ){};
    virtual void screenUpdated( void ){};
    virtual void screenResized( void ){};

    const std::string& caption( void ) const;
    float value( void ) const;
    float minValue( void ) const;
    float maxValue( void ) const;

    void setCaption( const std::string caption );
    void setValue( const float value );
    void setRange( const float min_value, const float max_value );
    void setSliderColor( const kvs::RGBColor& color );
    void showRange( void );
    void hideRange( void );

private:

    void draw_slider_bar( const int x, const int y, const int width );
    void draw_cursor( const int x, const int y, const int width );
    bool is_in_slider( const int x, const int y, const bool proper = false );
    bool is_in_cursor( const int x, const int y, const bool proper = false );
    float get_value( const int x );
    int get_fitted_width( void );
    int get_fitted_height( void );

private:

    void paintEvent( void );
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__SLIDER_H_INCLUDE
