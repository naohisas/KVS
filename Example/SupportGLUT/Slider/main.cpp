/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the Slider widget.
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
#include <kvs/RGBColor>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Slider>



namespace
{
kvs::RGBColor Color( 0, 0, 0 );
float Opacity( 1.0f );
}


class PaintEvent : public kvs::PaintEventListener
{
    void update( void )
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glBegin( GL_TRIANGLES );
        glColor4ub( ::Color.r(), ::Color.g(), ::Color.b(), (GLubyte)( ::Opacity * 255 ) );
        glVertex3d(  0.0,  3.0, 0.0 );
        glVertex3d(  3.0, -3.0, 0.0 );
        glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    }
};

class RedSlider : public kvs::glut::Slider
{
public:

    RedSlider( kvs::glut::Screen* screen ):
        kvs::glut::Slider( screen ){};

    void sliderMoved( void )
    {
        const kvs::UInt8 r = static_cast<int>( this->value() + 0.5f );
        const kvs::UInt8 g = ::Color.g();
        const kvs::UInt8 b = ::Color.b();
        ::Color = kvs::RGBColor( r, g, b );
    }
};

class GreenSlider : public kvs::glut::Slider
{
public:

    GreenSlider( kvs::glut::Screen* screen ):
        kvs::glut::Slider( screen ){};

    void sliderMoved( void )
    {
        const kvs::UInt8 r = ::Color.r();
        const kvs::UInt8 g = static_cast<kvs::UInt8>( this->value() + 0.5f );
        const kvs::UInt8 b = ::Color.b();
        ::Color = kvs::RGBColor( r, g, b );
    }
};

class BlueSlider : public kvs::glut::Slider
{
public:

    BlueSlider( kvs::glut::Screen* screen ):
        kvs::glut::Slider( screen ){};

    void sliderMoved( void )
    {
        const kvs::UInt8 r = ::Color.r();
        const kvs::UInt8 g = ::Color.g();
        const kvs::UInt8 b = static_cast<kvs::UInt8>( this->value() + 0.5f );
        ::Color = kvs::RGBColor( r, g, b );
    }
};

class OpacitySlider : public kvs::glut::Slider
{
public:

    OpacitySlider( kvs::glut::Screen* screen ):
        kvs::glut::Slider( screen ){};

    void valueChanged( void )
    {
        ::Opacity = this->value();
    }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    PaintEvent paint_event;

    kvs::glut::Screen screen( &app );
    screen.addEvent( &paint_event );
    screen.setTitle( "Slider" );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.show();

    RedSlider red( &screen );
    red.setSliderColor( kvs::RGBColor( 255, 0, 0 ) );
    red.setX( 0 );
    red.setY( 0 );
    red.setWidth( 150 );
    red.setValue( 0 );
    red.setRange( 0, 255 );
    red.setMargin( 10 );
    red.setCaption("Red");
    red.show();

    GreenSlider green( &screen );
    green.setSliderColor( kvs::RGBColor( 0, 255, 0 ) );
    green.setX( red.x() );
    green.setY( red.y() + red.height() );
    green.setWidth( 150 );
    green.setValue( 0 );
    green.setRange( 0, 255 );
    green.setMargin( 10 );
    green.setCaption("Green");
    green.show();

    BlueSlider blue( &screen );
    blue.setSliderColor( kvs::RGBColor( 0, 0, 255 ) );
    blue.setX( green.x() );
    blue.setY( green.y() + green.height() );
    blue.setWidth( 150 );
    blue.setValue( 0 );
    blue.setRange( 0, 255 );
    blue.setMargin( 10 );
    blue.setCaption("Blue");
    blue.show();

    OpacitySlider opacity( &screen );
    opacity.setX( blue.x() );
    opacity.setY( blue.y() + blue.height() );
    opacity.setWidth( 150 );
    opacity.setValue( 1 );
    opacity.setRange( 0, 1 );
    opacity.setMargin( 10 );
    opacity.setCaption("Opacity");
    opacity.show();

    return app.run();
}
