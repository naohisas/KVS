/*****************************************************************************/
/**
 *  @file   RadioButtonGroup.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RadioButtonGroup.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "RadioButtonGroup.h"
#include "RadioButton.h"
#include <kvs/ScreenBase>
#include <kvs/EventBase>
#include <kvs/MouseEvent>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new RadioButtonGroup class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
RadioButtonGroup::RadioButtonGroup( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );
}

/*===========================================================================*/
/**
 *  @brief  Adds a radio button.
 *  @param  button [in] pointer to the radio button
 */
/*===========================================================================*/
void RadioButtonGroup::add( kvs::glut::RadioButton* button )
{
    button->attach_group( this );
    m_buttons.push_back( button );
}

/*===========================================================================*/
/**
 *  @brief  Removes the radio button.
 *  @param  button [in] pointer to the radio button
 */
/*===========================================================================*/
void RadioButtonGroup::remove( kvs::glut::RadioButton* button )
{
    button->detach_group();
    m_buttons.remove( button );
}

/*===========================================================================*/
/**
 *  @brief  Shows the buttons in the group.
 */
/*===========================================================================*/
void RadioButtonGroup::show()
{
    BaseClass::show();

    std::list<kvs::glut::RadioButton*>::iterator button = m_buttons.begin();
    std::list<kvs::glut::RadioButton*>::iterator last = m_buttons.end();
    while ( button != last )
    {
        (*button++)->show();
    }
}

/*===========================================================================*/
/**
 *  @brief  Hides the buttons in the group.
 */
/*===========================================================================*/
void RadioButtonGroup::hide()
{
    BaseClass::hide();

    std::list<kvs::glut::RadioButton*>::iterator button = m_buttons.begin();
    std::list<kvs::glut::RadioButton*>::iterator last = m_buttons.end();
    while ( button != last )
    {
        (*button++)->hide();
    }
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void RadioButtonGroup::paintEvent()
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
void RadioButtonGroup::resizeEvent( int width, int height )
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
void RadioButtonGroup::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    int id = 0;
    std::list<kvs::glut::RadioButton*>::iterator button = m_buttons.begin();
    std::list<kvs::glut::RadioButton*>::iterator last = m_buttons.end();
    while ( button != last )
    {
        if ( (*button)->contains( event->x(), event->y() ) )
        {
            BaseClass::activate();
            this->pressed( *button );
            this->pressed( id );
        }

        button++;
        id++;
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void RadioButtonGroup::mouseReleaseEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        int id = 0;
        std::list<kvs::glut::RadioButton*>::iterator button = m_buttons.begin();
        std::list<kvs::glut::RadioButton*>::iterator last = m_buttons.end();
        while ( button != last )
        {
            if ( (*button)->contains( event->x(), event->y() ) )
            {
                this->released( *button );
                this->released( id );
                BaseClass::deactivate();
            }

            button++;
            id++;
        }
    }
}

} // end of namespace glut

} // end of namespace kvs
