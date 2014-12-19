/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the PushButton widget.
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
#include <kvs/glut/PushButton>

namespace
{
const kvs::RGBColor Gray( 100, 100, 100 );
const kvs::RGBColor Red( 255, 0, 0 );
const kvs::RGBColor Green( 0, 255, 0 );
const kvs::RGBColor Blue( 0, 0, 255 );
kvs::RGBColor Color = Gray;
}


class PaintEvent : public kvs::PaintEventListener
{
    void update( void )
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glLineWidth( 3 );
        glBegin( GL_TRIANGLES );
        glColor3ub( ::Color.r(), ::Color.g(), ::Color.b() );
        glVertex3d(  0.0,  3.0, 0.0 );
        glVertex3d(  3.0, -3.0, 0.0 );
        glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    }
};

class RedButton : public kvs::glut::PushButton
{
public:

    RedButton( kvs::glut::Screen* screen ):
        kvs::glut::PushButton( screen ){};

    void pressed( void )
    {
        ::Color = ::Red;
    }
};

class GreenButton : public kvs::glut::PushButton
{
public:

    GreenButton( kvs::glut::Screen* screen ):
        kvs::glut::PushButton( screen ){};

    void pressed( void )
    {
        ::Color = ::Green;
    }
};

class BlueButton : public kvs::glut::PushButton
{
public:

    BlueButton( kvs::glut::Screen* screen ):
        kvs::glut::PushButton( screen ){};

    void pressed( void )
    {
        ::Color = ::Blue;
    }
};

class ResetButton : public kvs::glut::PushButton
{
public:

    ResetButton( kvs::glut::Screen* screen ):
        kvs::glut::PushButton( screen ){};

    void pressed( void )
    {
        ::Color = ::Gray;
        screen()->reset();
    }
};

class QuitButton : public kvs::glut::PushButton
{
public:

    QuitButton( kvs::glut::Screen* screen ):
        kvs::glut::PushButton( screen ){};

    void pressed( void )
    {
        exit( EXIT_SUCCESS );
    }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    PaintEvent paint_event;

    kvs::glut::Screen screen( &app );
    screen.addEvent( &paint_event );
    screen.setTitle( "PushButton" );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.show();

    RedButton red( &screen );
    red.setButtonColor( ::Red );
    red.setX( 0 );
    red.setY( 0 );
    red.setWidth( 150 );
    red.setMargin( 10 );
    red.setCaption("Red");
    red.show();

    GreenButton green( &screen );
    green.setButtonColor( ::Green );
    green.setX( red.x() );
    green.setY( red.y() + red.height() );
    green.setWidth( 150 );
    green.setMargin( 10 );
    green.setCaption("Green");
    green.show();

    BlueButton blue( &screen );
    blue.setButtonColor( ::Blue );
    blue.setTextColor( kvs::RGBColor( 255, 255, 255 ) );
    blue.setX( green.x() );
    blue.setY( green.y() + green.height() );
    blue.setWidth( 150 );
    blue.setMargin( 10 );
    blue.setCaption("Blue");
    blue.show();

    ResetButton reset( &screen );
    reset.setX( blue.x() );
    reset.setY( blue.y() + blue.height() * 2 );
    reset.setWidth( 150 );
    reset.setMargin( 10 );
    reset.setCaption("Reset");
    reset.show();

    QuitButton quit( &screen );
    quit.setX( reset.x() );
    quit.setY( reset.y() + reset.height() );
    quit.setWidth( 150 );
    quit.setMargin( 10 );
    quit.setCaption("Quit");
    quit.show();

    return app.run();
}
