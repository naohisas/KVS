/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program to display a simple triangle using GLUT.
 *  @author Naohisa Sakamoto
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
// KVS
#include <kvs/OpenGL>
#include <kvs/PaintEventListener>
// SupportGLUT
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Label>


/*===========================================================================*/
/**
 *  @brief  User-defined paint event.
 *
 *  Users can define a paint event by inheriting the kvs::glut::PaintEventListener
 *  class. Users have to implement a update function 'void update( void )' in
 *  the paint event class to render a object in the screen.
 */
/*===========================================================================*/
class PaintEvent : public kvs::PaintEventListener
{
    void update()
    {
        /* This sample method draws a rainbow colored triangle in the screen by
         * using GL_TRIANGLES.
         */
        glBegin( GL_TRIANGLES );
        glColor3ub( 255,   0,   0 ); glVertex3d(  0.0,  3.0, 0.0 );
        glColor3ub(   0, 255,   0 ); glVertex3d(  3.0, -3.0, 0.0 );
        glColor3ub(   0,   0, 255 ); glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    }
};

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    // Application.
    kvs::glut::Application app( argc, argv );

    // User specified event.
    PaintEvent paint_event;

    // Screen.
    kvs::glut::Screen screen;
    screen.addEvent( &paint_event );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "SimpleTriangle" );
    screen.show();

    // Message.
    kvs::glut::Label message( &screen );
    message.setMargin( 10 );
    message.addText( "Rotation:    [Left Button]" );
    message.addText( "Translation: [Right Button] or [Ctrl] + [Left Button]" );
    message.addText( "Scaling:     [Shift] + [Left Button]" );
    message.addText( "Quit:        [Esc] or [q]" );
    message.show();

    return app.run();
}
