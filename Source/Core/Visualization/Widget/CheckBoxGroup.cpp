/*****************************************************************************/
/**
 *  @file   CheckBoxGroup.cpp
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
#include "CheckBoxGroup.h"
#include "CheckBox.h"
#include <kvs/ScreenBase>
#include <kvs/EventBase>
#include <kvs/MouseEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CheckBoxGroup class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
CheckBoxGroup::CheckBoxGroup( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen )
{
    BaseClass::addEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );
}

/*===========================================================================*/
/**
 *  @brief  Adds a check box.
 *  @param  box [in] pointer to the check box
 */
/*===========================================================================*/
void CheckBoxGroup::add( kvs::CheckBox* box )
{
    box->attach_group( this );
    m_boxes.push_back( box );
}

/*===========================================================================*/
/**
 *  @brief  Removes the check box.
 *  @param  box [in] pointer to the check box
 */
/*===========================================================================*/
void CheckBoxGroup::remove( kvs::CheckBox* box )
{
    box->detach_group();
    m_boxes.remove( box );
}

/*===========================================================================*/
/**
 *  @brief  Shows the boxes in the group.
 */
/*===========================================================================*/
void CheckBoxGroup::show()
{
    BaseClass::show();

    std::list<kvs::CheckBox*>::iterator box = m_boxes.begin();
    std::list<kvs::CheckBox*>::iterator last = m_boxes.end();
    while ( box != last ) { (*box++)->show(); }
}

/*===========================================================================*/
/**
 *  @brief  Hides the boxes in the group.
 */
/*===========================================================================*/
void CheckBoxGroup::hide()
{
    BaseClass::hide();

    std::list<kvs::CheckBox*>::iterator box = m_boxes.begin();
    std::list<kvs::CheckBox*>::iterator last = m_boxes.end();
    while ( box != last ) { (*box++)->hide(); }
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void CheckBoxGroup::paintEvent()
{
    this->screenUpdated();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void CheckBoxGroup::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );

    this->screenResized();
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void CheckBoxGroup::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    int id = 0;
    std::list<kvs::CheckBox*>::iterator box = m_boxes.begin();
    std::list<kvs::CheckBox*>::iterator last = m_boxes.end();
    while ( box != last )
    {
        if ( (*box)->contains( event->x(), event->y() ) )
        {
            BaseClass::activate();
            this->pressed( *box );
            this->pressed( id );
        }

        box++;
        id++;
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void CheckBoxGroup::mouseReleaseEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        int id = 0;
        std::list<kvs::CheckBox*>::iterator box = m_boxes.begin();
        std::list<kvs::CheckBox*>::iterator last = m_boxes.end();
        while ( box != last )
        {
            if ( (*box)->contains( event->x(), event->y() ) )
            {
                this->released( *box );
                this->released( id );
                BaseClass::deactivate();
            }

            box++;
            id++;
        }
    }
}

} // end of namespace kvs
