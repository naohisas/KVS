/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  DICOM viewer
 *  @author Naohisa Sakamoto
 *
 *  Compile:\
 *  \code
 *  % kvsmake -G
 *  % kvsmake
 *  \endcode
 *
 *  Usage:\n
 *  \code
 *  % ./DICOMViewer [-h|--help] <DICOM file or directory>
 *  \endcode
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1518 2013-04-10 01:13:11Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Argument.h"
#include "Parameter.h"
#include "Event.h"
#include "Widget.h"
#include <kvs/Message>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    // Parse command line arguments.
    Argument arg( argc, argv );
    if( !arg.parse() ) exit( EXIT_FAILURE );

    // Setup a rendering screen.
    kvs::glut::Screen screen( &app );

    // Read parameters from the arguments.
    Parameter parameter;
    if( !parameter.read( arg ) )
    {
        kvsMessageError("Cannot read the DICOM file(s).");
        exit( EXIT_FAILURE );
    }

    // Label widgets.
    Widget::CounterLabel counter_label( &parameter, &screen );
    Widget::ModalityLabel modality_label( &parameter, &screen );
    Widget::ImageInfoLabel image_info_label( &parameter, &screen );
    counter_label.show();
    modality_label.show();
    image_info_label.show();

    // User specified events.
    Event::Init init( &parameter );
    Event::KeyPress key_press( &parameter );
    Event::MousePress mouse_press( &parameter );
    Event::MouseMove mouse_move( &parameter );

    screen.addEvent( &init );
    screen.addEvent( &key_press );
    screen.setEvent( &mouse_press );
    screen.setEvent( &mouse_move );
    screen.setGeometry( 0, 0, parameter.width, parameter.height );
    screen.setTitle( "DICOM viewer" );
    screen.show();

    return( app.run() );
}
