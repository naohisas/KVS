/*****************************************************************************/
/**
 *  @file   RadioButton.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RadioButton.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__RADIO_BUTTON_H_INCLUDE
#define KVS__GLUT__RADIO_BUTTON_H_INCLUDE

#include <kvs/glut/WidgetBase>
#include <string>


namespace kvs
{

class ScreenBase;

namespace glut
{

class RadioButtonGroup;

/*===========================================================================*/
/**
 *  @brief  Check box class.
 */
/*===========================================================================*/
class RadioButton : public kvs::glut::WidgetBase
{
public:

    typedef kvs::glut::WidgetBase BaseClass;

    friend class kvs::glut::RadioButtonGroup;

protected:

    std::string m_caption; ///< caption
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color
    bool m_state; ///< check state
    kvs::glut::RadioButtonGroup* m_group; ///< pointer to the radio button group

public:

    RadioButton( kvs::ScreenBase* screen = 0 );

    virtual void pressed( void ) {};
    virtual void released( void ) {};
    virtual void screenUpdated( void ) {};
    virtual void screenResized( void ) {};
    virtual void stateChanged( void ) {};

    const std::string& caption( void ) const;
    bool state( void ) const;

    void setCaption( const std::string caption );
    void setState( const bool state );

private:

    void draw_box( void );
    void draw_mark( void );
    int get_fitted_width( void );
    int get_fitted_height( void );
    bool contains( int x, int y );
    void attach_group( kvs::glut::RadioButtonGroup* group );
    void detach_group( void );

private:

    void paintEvent( void );
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__RADIO_BUTTON_H_INCLUDE
