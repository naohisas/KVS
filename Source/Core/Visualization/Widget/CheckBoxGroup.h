/*****************************************************************************/
/**
 *  @file   CheckBoxGroup.h
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
#include "CheckBox.h"


namespace kvs
{

class CheckBox;

/*===========================================================================*/
/**
 *  @brief  Check box group class.
 */
/*===========================================================================*/
class CheckBoxGroup : public kvs::WidgetBase
{
public:
    typedef kvs::WidgetBase BaseClass;

protected:
    std::list<kvs::CheckBox*> m_boxes; ///< check box list

public:
    CheckBoxGroup( kvs::ScreenBase* screen = 0 );

    virtual void pressed( kvs::CheckBox* box ) { kvs::IgnoreUnusedVariable( box ); };
    virtual void pressed( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void released( kvs::CheckBox* box ) { kvs::IgnoreUnusedVariable( box ); };
    virtual void released( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::list<kvs::CheckBox*>& checkBoxes() const { return m_boxes; }
    void add( kvs::CheckBox* box );
    void remove( kvs::CheckBox* box );
    void show();
    void hide();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
