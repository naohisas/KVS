/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for the CheckBox widget.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/PaintEventListener>
#include <kvs/OpenGL>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/CheckBox>
#include <kvs/glut/CheckBoxGroup>


namespace
{
bool EdgeFlag = true;
bool PlaneFlag = true;
}

class PaintEvent : public kvs::PaintEventListener
{
    void update()
    {
        glEnable( GL_DEPTH_TEST );

        if ( ::EdgeFlag )
        {
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

            glLineWidth( 2 );
            glBegin( GL_LINE_STRIP );
            glColor3ub( 0, 0, 0 );
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

class EdgeBox : public kvs::glut::CheckBox
{
public:
    EdgeBox( kvs::glut::Screen* screen ): kvs::glut::CheckBox( screen ){};
    void stateChanged() { ::EdgeFlag = this->state(); }
};

class PlaneBox : public kvs::glut::CheckBox
{
public:
    PlaneBox( kvs::glut::Screen* screen ): kvs::glut::CheckBox( screen ){};
    void stateChanged() { ::PlaneFlag = this->state(); }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    PaintEvent paint_event;

    kvs::glut::Screen screen( &app );
    screen.addEvent( &paint_event );
    screen.setTitle( "CheckBox" );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.show();

    kvs::glut::CheckBoxGroup group;

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

    group.add( &edge );
    group.add( &plane );
    group.show();

    return app.run();
}
