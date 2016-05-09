/*****************************************************************************/
/**
 *  @file   Event.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Event.cpp 1518 2013-04-10 01:13:11Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Event.h"
#include "Command.h"
#include <kvs/Dicom>
#include <kvs/ImageObject>
#include <kvs/ImageRenderer>
#include <kvs/RGBColor>
#include <kvs/Key>
#include <kvs/MouseButton>
#include <kvs/Background>
#include <kvs/glut/Label>
#include <kvs/glut/Screen>


namespace Event
{

/*===========================================================================*/
/**
 *  @brief  Initialize event.
 */
/*===========================================================================*/
void Init::update( void )
{
    // Set background color to black.
    const kvs::RGBColor black( 0, 0, 0 );
    static_cast<kvs::glut::Screen*>(screen())->scene()->background()->setColor( black );

    // Register the DICOM image as ImageObject to the screen.
    const kvs::Dicom* dicom = m_parameter->dicom_list[0];
    const size_t width = dicom->column();
    const size_t height = dicom->row();
    const kvs::ValueArray<kvs::UInt8> data = dicom->pixelData();
    const kvs::ImageObject::PixelType type = kvs::ImageObject::Gray8;
    kvs::ImageObject* object = new kvs::ImageObject( width, height, data, type );
    kvs::ImageRenderer* renderer = new kvs::ImageRenderer();
    static_cast<kvs::glut::Screen*>(screen())->registerObject( object, renderer );
}

/*===========================================================================*/
/**
 *  @brief  Key press event
 *  @param  event [in] pointer to the key event
 */
/*===========================================================================*/
void KeyPress::update( kvs::KeyEvent* event )
{
    Command command( m_parameter, static_cast<kvs::glut::Screen*>(screen()) );

    switch( event->key() )
    {
    case kvs::Key::i:     command.toggleInformationFlag(); break;
    case kvs::Key::s:     command.writeScreenImage();      break;
    case kvs::Key::h:     command.writeHeader();           break;
    case kvs::Key::w:     command.writeData();             break;
    case kvs::Key::Up:    command.incrementIndex(5);       break;
    case kvs::Key::Down:  command.decrementIndex(5);       break;
    case kvs::Key::Right: command.incrementIndex(1);       break;
    case kvs::Key::Left:  command.decrementIndex(1);       break;
    default: break;
    }

    screen()->redraw();
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void MousePress::update( kvs::MouseEvent* event )
{
    Command command( m_parameter, static_cast<kvs::glut::Screen*>(screen()) );

    switch( event->button() )
    {
    case kvs::MouseButton::Left: command.pressMouse( event ); break;
    default: break;
    }

    switch( event->state() )
    {
    case kvs::MouseButton::Up:   command.incrementIndex(); break;
    case kvs::MouseButton::Down: command.decrementIndex(); break;
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void MouseMove::update( kvs::MouseEvent* event )
{
    Command command( m_parameter, static_cast<kvs::glut::Screen*>(screen()) );

    command.moveMouse( event );
    screen()->redraw();
}

} // end of namespace Event
