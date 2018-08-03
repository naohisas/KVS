/*****************************************************************************/
/**
 *  @file   RadioButtonGroup.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RadioButtonGroup.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__RADIO_BUTTON_GROUP_H_INCLUDE
#define KVS__GLUT__RADIO_BUTTON_GROUP_H_INCLUDE

#include <list>
#include <kvs/glut/WidgetBase>
#include <kvs/IgnoreUnusedVariable>
#include "RadioButton.h"


namespace kvs
{

class ScreenBase;

namespace glut
{

class RadioButton;

/*===========================================================================*/
/**
 *  @brief  Radio button group class.
 */
/*===========================================================================*/
class RadioButtonGroup : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;

private:
    std::list<kvs::glut::RadioButton*> m_buttons; ///< radio button list

public:
    RadioButtonGroup( kvs::ScreenBase* screen = 0 );

    virtual void pressed( kvs::glut::RadioButton* button ) { kvs::IgnoreUnusedVariable( button ); };
    virtual void pressed( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void released( kvs::glut::RadioButton* button ) { kvs::IgnoreUnusedVariable( button ); };
    virtual void released( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::list<kvs::glut::RadioButton*>& radioButtons() const { return m_buttons; }

    void add( kvs::glut::RadioButton* button );
    void remove( kvs::glut::RadioButton* button );
    void show();
    void hide();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__RADIO_BUTTON_GROUP_H_INCLUDE
