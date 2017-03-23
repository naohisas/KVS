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
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <list>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include "WidgetBase.h"
#include "RadioButton.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Radio button group class.
 */
/*===========================================================================*/
class RadioButtonGroup : public kvs::WidgetBase
{
public:
    typedef kvs::WidgetBase BaseClass;

private:
    std::list<kvs::RadioButton*> m_buttons; ///< radio button list

public:
    RadioButtonGroup( kvs::ScreenBase* screen = 0 );

    virtual void pressed( kvs::RadioButton* button ) { kvs::IgnoreUnusedVariable( button ); };
    virtual void pressed( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void released( kvs::RadioButton* button ) { kvs::IgnoreUnusedVariable( button ); };
    virtual void released( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::list<kvs::RadioButton*>& radioButtons() const { return m_buttons; }

    void add( kvs::RadioButton* button );
    void remove( kvs::RadioButton* button );
    void show();
    void hide();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
