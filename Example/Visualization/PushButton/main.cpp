/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the PushButton widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/UIColor>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/PushButton>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "PushButton" );
    screen.create();

    const size_t width = 150;
    const size_t margin = 10;
    const kvs::RGBColor gray = kvs::UIColor::Gray();
    const kvs::RGBColor red = kvs::UIColor::Red();
    const kvs::RGBColor green = kvs::UIColor::Green();
    const kvs::RGBColor blue = kvs::UIColor::Blue();

    kvs::RGBColor color = gray;
    kvs::PaintEventListener paint_event(
        [&] {
            glLineWidth( 3 );
            glBegin( GL_TRIANGLES );
            glColor3ub( color.r(), color.g(), color.b() );
            glVertex3d(  0.0,  3.0, 0.0 );
            glVertex3d(  3.0, -3.0, 0.0 );
            glVertex3d( -3.0, -3.0, 0.0 );
            glEnd();
        });
    screen.addEvent( &paint_event );

    kvs::PushButton red_button( &screen );
    red_button.setCaption( "Red" );
    red_button.setButtonColor( red );
    red_button.setWidth( width );
    red_button.setMargin( margin );
    red_button.anchorToTopLeft();
    red_button.pressed( [&] { color = red; } );
    red_button.show();

    kvs::PushButton green_button( &screen );
    green_button.setCaption( "Green" );
    green_button.setButtonColor( green );
    green_button.setWidth( width );
    green_button.setMargin( margin );
    green_button.anchorToBottom( &red_button );
    green_button.pressed( [&] { color = green; } );
    green_button.show();

    kvs::PushButton blue_button( &screen );
    blue_button.setCaption( "Blue" );
    blue_button.setButtonColor( blue );
    blue_button.setWidth( width );
    blue_button.setMargin( margin );
    blue_button.anchorToBottom( &green_button );
    blue_button.pressed( [&] { color = blue; } );
    blue_button.show();

    kvs::PushButton quit_button( &screen );
    quit_button.setCaption( "Quit" );
    quit_button.setWidth( width );
    quit_button.setMargin( margin );
    quit_button.anchorToBottomRight();
    quit_button.pressed( [&] { exit( EXIT_SUCCESS ); } );
    quit_button.show();

    kvs::PushButton reset_button( &screen );
    reset_button.setCaption( "Reset" );
    reset_button.setWidth( width );
    reset_button.setMargin( margin );
    reset_button.anchorToTop( &quit_button );
    reset_button.pressed( [&] { color = gray; screen.reset(); } );
    reset_button.show();

    return app.run();
}
