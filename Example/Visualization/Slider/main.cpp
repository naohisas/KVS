/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the Slider widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/RGBColor>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/Slider>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );

    kvs::Screen screen( &app );
    screen.setTitle( "kvs::Slider" );
    screen.create();

    // Triangle color
    auto color = kvs::RGBColor::Black();
    auto alpha = 1.0f;

    // Change color component functors.
    auto SetR = [&] ( float r ) { color.set( kvs::UInt8(r), color.g(), color.b() ); };
    auto SetG = [&] ( float g ) { color.set( color.r(), kvs::UInt8(g), color.b() ); };
    auto SetB = [&] ( float b ) { color.set( color.r(), color.g(), kvs::UInt8(b) ); };

    // Paint event (drawing a triangle)
    kvs::PaintEventListener paint_event( [&]
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glBegin( GL_TRIANGLES );
        glColor4ub( color.r(), color.g(), color.b(), (GLubyte)( alpha * 255 ) );
        glVertex3d(  0.0,  3.0, 0.0 );
        glVertex3d(  3.0, -3.0, 0.0 );
        glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    } );
    screen.addEvent( &paint_event );

    // Slider for changing red component of triangle color.
    kvs::Slider red( &screen );
    red.setSliderColor( kvs::RGBColor::Red() );
    red.setWidth( 150 );
    red.setValue( 0 );
    red.setRange( 0, 255 );
    red.setMargin( 10 );
    red.setCaption("Red");
    red.anchorToTopLeft();
    red.valueChanged( [&] { SetR( red.value() ); } );
    red.show();

    // Slider for changing green component of triangle color.
    kvs::Slider green( &screen );
    green.setSliderColor( kvs::RGBColor::Green() );
    green.setWidth( 150 );
    green.setValue( 0 );
    green.setRange( 0, 255 );
    green.setCaption("Green");
    green.anchorToBottom( &red );
    green.valueChanged( [&] { SetG( green.value() ); } );
    green.show();

    // Slider for changing blue component of triangle color.
    kvs::Slider blue( &screen );
    blue.setSliderColor( kvs::RGBColor::Blue() );
    blue.setWidth( 150 );
    blue.setValue( 0 );
    blue.setRange( 0, 255 );
    blue.setCaption("Blue");
    blue.anchorToBottom( &green );
    blue.valueChanged( [&] { SetB( blue.value() ); } );
    blue.show();

    // Slider for changing alpha component of triangle color.
    kvs::Slider opacity( &screen );
    opacity.setWidth( 150 );
    opacity.setValue( 1 );
    opacity.setRange( 0, 1 );
    opacity.setCaption("Opacity");
    opacity.anchorToBottom( &blue );
    opacity.valueChanged( [&] { alpha = opacity.value(); } );
    opacity.show();

    return app.run();
}
