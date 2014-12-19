/*****************************************************************************/
/**
 *  @file   Command.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Command.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Parameter.h"
#include <kvs/MouseEvent>
#include <kvs/glut/Screen>


/*===========================================================================*/
/**
 *  @brief  Command set.
 */
/*===========================================================================*/
class Command
{
protected:

    Parameter* m_parameter; ///< parameter set
    kvs::glut::Screen* m_screen; ///< pointer to screen

public:

    Command( Parameter* parameter, kvs::glut::Screen* screen );

    void toggleInformationFlag( void );

    void incrementIndex( const unsigned int value = 1 );

    void decrementIndex( const unsigned int value = 1 );

    void updateDicomImage( void );

    void updateDicomWindow( void );

    void resetDicomWindow( void );

    void pressMouse( kvs::MouseEvent* event );

    void moveMouse( kvs::MouseEvent* event );

    void writeScreenImage( void );

    void writeHeader( void );

    void writeData( void );
};

#endif // __COMMAND_H__
