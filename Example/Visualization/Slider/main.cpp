/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the Slider widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/RGBColor>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/Math>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Slider>


namespace
{
kvs::RGBColor Color( 0, 0, 0 );
float Opacity( 1.0f );
void ChangeR( const int r ) { ::Color = kvs::RGBColor( r, ::Color.g(), ::Color.b() ); }
void ChangeG( const int g ) { ::Color = kvs::RGBColor( ::Color.r(), g, ::Color.b() ); }
void ChangeB( const int b ) { ::Color = kvs::RGBColor( ::Color.r(), ::Color.g(), b ); }
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

        glBegin( GL_TRIANGLES );
        glColor4ub( ::Color.r(), ::Color.g(), ::Color.b(), (GLubyte)( ::Opacity * 255 ) );
        glVertex3d(  0.0,  3.0, 0.0 );
        glVertex3d(  3.0, -3.0, 0.0 );
        glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    }
};

/*===========================================================================*/
/**
 *  @brief  Slider for changing the color of red.
 */
/*===========================================================================*/
class RedSlider : public kvs::Slider
{
public:
    RedSlider( kvs::glut::Screen* screen ): kvs::Slider( screen ){};
    void sliderMoved() { ::ChangeR( kvs::Math::Round( this->value() ) ); }
};

/*===========================================================================*/
/**
 *  @brief  Slider for changing the color of green.
 */
/*===========================================================================*/
class GreenSlider : public kvs::Slider
{
public:
    GreenSlider( kvs::glut::Screen* screen ): kvs::Slider( screen ){};
    void sliderMoved() { ::ChangeG( kvs::Math::Round( this->value() ) ); }
};

/*===========================================================================*/
/**
 *  @brief  Slider for changing the color of blue.
 */
/*===========================================================================*/
class BlueSlider : public kvs::Slider
{
public:
    BlueSlider( kvs::glut::Screen* screen ): kvs::Slider( screen ){};
    void sliderMoved() { ::ChangeB( kvs::Math::Round( this->value() ) ); }
};

/*===========================================================================*/
/**
 *  @brief  Slider for changing the opacity.
 */
/*===========================================================================*/
class OpacitySlider : public kvs::Slider
{
public:
    OpacitySlider( kvs::glut::Screen* screen ): kvs::Slider( screen ){};
    void valueChanged() { ::Opacity = this->value(); }
};

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
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
