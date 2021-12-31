/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program to display a simple triangle using GLUT.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
// KVS
#include <kvs/OpenGL>
#include <kvs/PaintEventListener>
// SupportGLUT
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Label>


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

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "SimpleTriangle" );
    screen.create();

    // User specified event.
    kvs::PaintEventListener paint_event( [] ()
    {
        glBegin( GL_TRIANGLES );
        glColor3ub( 255,   0,   0 ); glVertex3d(  0.0,  3.0, 0.0 );
        glColor3ub(   0, 255,   0 ); glVertex3d(  3.0, -3.0, 0.0 );
        glColor3ub(   0,   0, 255 ); glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    } );
    screen.addEvent( &paint_event );

    // Message.
    kvs::Label message( &screen );
    message.setMargin( 10 );
    message.addText( "Rotation:  [Left Button]" );
    message.addText( "Translation:  [Right Button] or [Ctrl] + [Left Button]" );
    message.addText( "Scaling:  [Shift] + [Left Button]" );
    message.addText( "Quit:  [Esc] or [q]" );
    message.show();

    return app.run();
}
