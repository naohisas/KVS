/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the RadioButton widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/UIColor>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/RadioButton>
#include <kvs/RadioButtonGroup>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::RadioButton" );
    screen.create();

    // Triangle color.
    auto color = kvs::UIColor::Gray();

    // Paint event (drawing a triangle)
    kvs::PaintEventListener paint_event( [&]
    {
        glBegin( GL_TRIANGLES );
        glColor3ub( color.r(), color.g(), color.b() );
        glVertex3d(  0.0,  3.0, 0.0 );
        glVertex3d(  3.0, -3.0, 0.0 );
        glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    } );
    screen.addEvent( &paint_event );

    // Button margin.
    const size_t margin = 10;

    // Button for changing triangle color to gray.
    kvs::RadioButton gray( &screen );
    gray.setCaption("Gray");
    gray.setMargin( margin );
    gray.setState( true );
    gray.anchorToTopLeft();
    gray.stateChanged( [&] { if ( gray.state() ) { color = kvs::UIColor::Gray(); } } );

    // Button for changing triangle color to red.
    kvs::RadioButton red( &screen );
    red.setCaption("Red");
    red.setMargin( margin );
    red.anchorToBottom( &gray );
    red.stateChanged( [&] { if ( red.state() ) { color = kvs::UIColor::Red(); } } );

    // Button for changing triangle color to green.
    kvs::RadioButton green( &screen );
    green.setCaption("Green");
    green.setMargin( margin );
    green.anchorToBottom( &red );
    green.stateChanged( [&] { if ( green.state() ) { color = kvs::UIColor::Green(); } } );

    // Button for changing triangle color to blue.
    kvs::RadioButton blue( &screen );
    blue.setCaption("Blue");
    blue.setMargin( margin );
    blue.anchorToBottom( &green );
    blue.stateChanged( [&] { if ( blue.state() ) { color = kvs::UIColor::Blue(); } } );

    // Button group.
    kvs::RadioButtonGroup group;
    group.add( &gray );  // 0
    group.add( &red );   // 1
    group.add( &green ); // 2
    group.add( &blue );  // 3
    group.show();

    return app.run();
}
