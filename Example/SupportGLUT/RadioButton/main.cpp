/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the RadioButton widget.
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
#include <kvs/glut/RadioButton>
#include <kvs/glut/RadioButtonGroup>

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

class GrayButton : public kvs::glut::RadioButton
{
public:

    GrayButton( kvs::glut::Screen* screen ):
        kvs::glut::RadioButton( screen ){};

    void stateChanged( void )
    {
        if ( this->state() ) ::Color = ::Gray;
    }
};

class RedButton : public kvs::glut::RadioButton
{
public:

    RedButton( kvs::glut::Screen* screen ):
        kvs::glut::RadioButton( screen ){};

    void stateChanged( void )
    {
        if ( this->state() ) ::Color = ::Red;
    }
};

class GreenButton : public kvs::glut::RadioButton
{
public:

    GreenButton( kvs::glut::Screen* screen ):
        kvs::glut::RadioButton( screen ){};

    void stateChanged( void )
    {
        if ( this->state() ) ::Color = ::Green;
    }
};

class BlueButton : public kvs::glut::RadioButton
{
public:

    BlueButton( kvs::glut::Screen* screen ):
        kvs::glut::RadioButton( screen ){};

    void stateChanged( void )
    {
        if ( this->state() ) ::Color = ::Blue;
    }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    PaintEvent paint_event;

    kvs::glut::Screen screen( &app );
    screen.addEvent( &paint_event );
    screen.setTitle( "RadioButton" );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.show();

    kvs::glut::RadioButtonGroup group;

    GrayButton gray( &screen );
    gray.setX( 10 );
    gray.setY( 10 );
    gray.setMargin( 10 );
    gray.setCaption("Gray");
    gray.setState( true );

    RedButton red( &screen );
    red.setX( gray.x() );
    red.setY( gray.y() + 20 );
    red.setMargin( 10 );
    red.setCaption("Red");

    GreenButton green( &screen );
    green.setX( red.x() );
    green.setY( red.y() + 20 );
    green.setMargin( 10 );
    green.setCaption("Green");

    BlueButton blue( &screen );
    blue.setX( green.x() );
    blue.setY( green.y() + 20 );
    blue.setMargin( 10 );
    blue.setCaption("Blue");

    group.add( &gray );  // 0
    group.add( &red );   // 1
    group.add( &green ); // 2
    group.add( &blue );  // 3
    group.show();

    return app.run();
}
