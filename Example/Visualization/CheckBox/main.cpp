/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the CheckBox widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/CheckBox>
#include <kvs/CheckBoxGroup>
#include <kvs/UIColor>


namespace
{
bool EdgeFlag = true;
bool PlaneFlag = true;
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
        glEnable( GL_DEPTH_TEST );

        if ( ::EdgeFlag )
        {
            const auto c = kvs::UIColor::Label();
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
            glLineWidth( 2 );
            glBegin( GL_LINE_STRIP );
            glColor3ub( c.r(), c.g(), c.b() );
            glVertex3d(  0.0,  3.0, 0.0 );
            glVertex3d(  3.0, -3.0, 0.0 );
            glVertex3d( -3.0, -3.0, 0.0 );
            glVertex3d(  0.0,  3.0, 0.0 );
            glEnd();

            glDisable( GL_LINE_SMOOTH );
            glDisable( GL_BLEND );
        }

        if ( ::PlaneFlag )
        {
            glEnable( GL_POLYGON_OFFSET_FILL );
            glPolygonOffset( 1.0, 0.0 );

            glBegin( GL_TRIANGLES );
            glColor3ub( 255,   0,   0 ); glVertex3d(  0.0,  3.0, 0.0 );
            glColor3ub(   0, 255,   0 ); glVertex3d(  3.0, -3.0, 0.0 );
            glColor3ub(   0,   0, 255 ); glVertex3d( -3.0, -3.0, 0.0 );
            glEnd();

            glDisable( GL_POLYGON_OFFSET_FILL );
        }

        glDisable( GL_DEPTH_TEST );
    }
};

/*===========================================================================*/
/**
 *  @brief  User-defined checkbox.
 */
/*===========================================================================*/
class EdgeBox : public kvs::CheckBox
{
public:
    EdgeBox( kvs::Screen* screen ): kvs::CheckBox( screen ){};
    void stateChanged() { ::EdgeFlag = this->state(); }
};

class PlaneBox : public kvs::CheckBox
{
public:
    PlaneBox( kvs::Screen* screen ): kvs::CheckBox( screen ){};
    void stateChanged() { ::PlaneFlag = this->state(); }
};

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
    screen.setTitle( "CheckBox" );
    screen.setGeometry( 0, 0, 512, 512 );

    PaintEvent paint_event;
    screen.addEvent( &paint_event );
    screen.create();

    EdgeBox edge( &screen );
    edge.setX( 10 );
    edge.setY( 10 );
    edge.setMargin( 10 );
    edge.setCaption("Edge");
    edge.setState( ::EdgeFlag );

    PlaneBox plane( &screen );
    plane.setX( edge.x() );
    plane.setY( edge.y() + 20 );
    plane.setMargin( 10 );
    plane.setCaption("Plane");
    plane.setState( ::PlaneFlag );

    kvs::CheckBoxGroup group;
    group.add( &edge );
    group.add( &plane );
    group.show();

    return app.run();
}
