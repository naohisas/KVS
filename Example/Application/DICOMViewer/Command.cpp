/*****************************************************************************/
/**
 *  @file   Command.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Command.cpp 1518 2013-04-10 01:13:11Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Command.h"
#include "Parameter.h"
#include <string>
#include <kvs/MouseEvent>
#include <kvs/Dicom>
#include <kvs/ImageObject>
#include <kvs/ImageImporter>
#include <kvs/ImageRenderer>
#include <kvs/Date>
#include <kvs/Time>
#include <kvs/File>
#include <kvs/Vector2>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/Camera>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns an output filename.
 *  @param  parameter [in] pointer to the parameters
 *  @param  extension [in] file extension
 *  @return output filename
 */
/*===========================================================================*/
const std::string GetOutputFilename( Parameter* parameter, const std::string& extension )
{
    const kvs::Dicom* dicom  = parameter->dicom_list[ parameter->index ];
    const kvs::File file( dicom->filename() );

    const kvs::Date today;
    const kvs::Time now;
    const std::string basename = file.baseName();
    const std::string today_str = today.toString();
    const std::string now_str = now.toString("-");

    return( basename + "_" + today_str + "_" + now_str + "." + extension );
}

} // end of namespace

/*===========================================================================*/
/**
 *  @brief  Constructs a new Command class.
 *  @param  parameter [in] pointer to the parameters
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
Command::Command( Parameter* parameter, kvs::glut::Screen* screen ):
    m_parameter( parameter ),
    m_screen( screen )
{
}

/*===========================================================================*/
/**
 *  @brief  Toggle for the DICOM information.
 */
/*===========================================================================*/
void Command::toggleInformationFlag( void )
{
    m_parameter->enable_show_information = !m_parameter->enable_show_information;
}

/*===========================================================================*/
/**
 *  @brief  Increment the index of the DICOM list.
 *  @param  value [in] incremental value
 */
/*===========================================================================*/
void Command::incrementIndex( const unsigned int value )
{
    const int nslices = m_parameter->dicom_list.size();

    m_parameter->index += value;
    m_parameter->index = kvs::Math::Min( m_parameter->index, nslices - 1 );

    this->updateDicomWindow();
    this->updateDicomImage();
}

/*===========================================================================*/
/**
 *  @brief  Decrement the index of the DICOM list.
 *  @param  value [in] decremental value
 *  @param  parameter [in] parameter
 */
/*===========================================================================*/
void Command::decrementIndex( const unsigned int value )
{
    m_parameter->index -= value;
    m_parameter->index = kvs::Math::Max( m_parameter->index, 0 );

    this->updateDicomWindow();
    this->updateDicomImage();
}

/*===========================================================================*/
/**
 *  @brief  Update DICOM image.
 */
/*===========================================================================*/
void Command::updateDicomImage( void )
{
    m_screen->scene()->objectManager()->erase();

    const kvs::Dicom* dicom  = m_parameter->dicom_list[ m_parameter->index ];
    kvs::ImageObject* object = new kvs::ImageImporter( dicom );
    kvs::ImageRenderer* renderer = new kvs::ImageRenderer();

    const int object_id = m_screen->scene()->objectManager()->insert( object );
    const int renderer_id = m_screen->scene()->rendererManager()->insert( renderer );
    m_screen->scene()->IDManager()->changeObject( object_id );
    m_screen->scene()->IDManager()->changeRenderer( renderer_id );
}

/*===========================================================================*/
/**
 *  @brief  Update DICOM window.
 */
/*===========================================================================*/
void Command::updateDicomWindow( void )
{
    kvs::Dicom* dicom = m_parameter->dicom_list[ m_parameter->index ];
    dicom->changeWindow( m_parameter->window_level, m_parameter->window_width );
}

/*===========================================================================*/
/**
 *  @brief  Reset DICOM image.
 */
/*===========================================================================*/
void Command::resetDicomWindow( void )
{
    const kvs::Dicom* dicom = m_parameter->dicom_list[ m_parameter->index ];
    m_parameter->window_width = dicom->windowWidth();
    m_parameter->window_level = dicom->windowLevel();

    this->updateDicomWindow();
    this->updateDicomImage();
}

/*===========================================================================*/
/**
 *  @brief  Function for the mouse press event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Command::pressMouse( kvs::MouseEvent* event )
{
    m_parameter->mouse.set( event->x(), event->y() );
}

/*===========================================================================*/
/**
 *  @brief  Function for the mouse move event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Command::moveMouse( kvs::MouseEvent* event )
{
    kvs::Vector2i diff( kvs::Vector2i( event->x(), event->y() ) - m_parameter->mouse );

    m_parameter->mouse.set( event->x(), event->y() );

    m_parameter->window_width += diff.x();
    m_parameter->window_level += diff.y();

    const kvs::Dicom* dicom = m_parameter->dicom_list[ m_parameter->index ];
    int min_window_value = dicom->minWindowValue();
    int max_window_value = dicom->maxWindowValue();
    int min_window_width = 0;
    int max_window_width = max_window_value - min_window_value + 1;
    m_parameter->window_width =
        kvs::Math::Clamp( m_parameter->window_width, min_window_width, max_window_width );
    m_parameter->window_level =
        kvs::Math::Clamp( m_parameter->window_level, min_window_value, max_window_value );

    this->updateDicomWindow();
    this->updateDicomImage();
}

/*===========================================================================*/
/**
 *  @brief  Writes a screen image.
 */
/*===========================================================================*/
void Command::writeScreenImage( void )
{
    const std::string filename = ::GetOutputFilename( m_parameter, "bmp" );
    m_screen->scene()->camera()->snapshot().write( filename.c_str() );
}

/*===========================================================================*/
/**
 *  @brief  Writes a DICOM header data as ascii data.
 */
/*===========================================================================*/
void Command::writeHeader( void )
{
    const std::string filename = ::GetOutputFilename( m_parameter, "csv" );
    m_parameter->dicom_list[ m_parameter->index ]->write( filename.c_str() );
}

/*===========================================================================*/
/**
 *  @brief  Writes a DICOM data as raw data.
 */
/*===========================================================================*/
void Command::writeData( void )
{
    const std::string filename = ::GetOutputFilename( m_parameter, "raw" );
    m_parameter->dicom_list[ m_parameter->index ]->write( filename.c_str() );
}
