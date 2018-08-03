/*****************************************************************************/
/**
 *  @file   CheckBox.h
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
#include <string>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

class CheckBoxGroup;

/*===========================================================================*/
/**
 *  @brief  Check box class.
 */
/*===========================================================================*/
class CheckBox : public kvs::WidgetBase
{
public:
    typedef kvs::WidgetBase BaseClass;
    friend class kvs::CheckBoxGroup;

private:
    std::string m_caption; ///< caption
    bool m_state; ///< check state
    kvs::CheckBoxGroup* m_group; ///< pointer to the check box group

public:
    CheckBox( kvs::ScreenBase* screen = 0 );

    virtual void pressed() {};
    virtual void released() {};
    virtual void screenUpdated() {};
    virtual void screenResized() {};
    virtual void stateChanged() {};

    const std::string& caption() const { return m_caption; }
    bool state() const { return m_state; }
    void setCaption( const std::string caption ) { m_caption = caption; }
    void setState( const bool state ) { m_state = state; }

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_box();
    void draw_mark();
    bool contains( int x, int y );
    void attach_group( kvs::CheckBoxGroup* group ) { m_group = group; }
    void detach_group() { m_group = NULL; }

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
