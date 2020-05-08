/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the PushButton widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/RGBColor>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/PushButton>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


namespace
{
const kvs::RGBColor Gray( 100, 100, 100 );
const kvs::RGBColor Red( 255, 0, 0 );
const kvs::RGBColor Green( 0, 255, 0 );
const kvs::RGBColor Blue( 0, 0, 255 );
kvs::RGBColor Color = Gray;
}


/*===========================================================================*/
/**
 *  @brief  User-defined paint event class.
 */
/*===========================================================================*/
class PaintEvent : public kvs::PaintEventListener
{
    void update()
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

/*===========================================================================*/
/**
 *  @brief  Push button for specifying the color of red.
 */
/*===========================================================================*/
class RedButton : public kvs::PushButton
{
public:
    RedButton( kvs::glut::Screen* screen ): kvs::PushButton( screen ){};
    void pressed() { ::Color = ::Red; }
};

/*===========================================================================*/
/**
 *  @brief  Push button for specifying the color of green.
 */
/*===========================================================================*/
class GreenButton : public kvs::PushButton
{
public:
    GreenButton( kvs::glut::Screen* screen ): kvs::PushButton( screen ){};
    void pressed() { ::Color = ::Green; }
};

/*===========================================================================*/
/**
 *  @brief  Push button for specifying the color of blue.
 */
/*===========================================================================*/
class BlueButton : public kvs::PushButton
{
public:
    BlueButton( kvs::glut::Screen* screen ): kvs::PushButton( screen ){};
    void pressed() { ::Color = ::Blue; }
};

/*===========================================================================*/
/**
 *  @brief  Push button for reset.
 */
/*===========================================================================*/
class ResetButton : public kvs::PushButton
{
public:
    ResetButton( kvs::glut::Screen* screen ): kvs::PushButton( screen ){};
    void pressed() { ::Color = ::Gray; screen()->reset(); }
};

/*===========================================================================*/
/**
 *  @brief  Push button for quit.
 */
/*===========================================================================*/
class QuitButton : public kvs::PushButton
{
public:
    QuitButton( kvs::glut::Screen* screen ): kvs::PushButton( screen ){};
    void pressed() { exit( EXIT_SUCCESS ); }
};

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
