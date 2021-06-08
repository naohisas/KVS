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
    screen.setTitle( "kvs::CheckBox" );
    screen.create();

    // Flags
    struct { bool edge = true; bool plane = true; } flag;

    // Paint event (drawing a triangle with edges)
    kvs::PaintEventListener paint_event( [&]
    {
        glEnable( GL_DEPTH_TEST );

        if ( flag.edge )
        {
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );

            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

            const auto c = kvs::UIColor::Label();
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

        if ( flag.plane )
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
    } );
    screen.addEvent( &paint_event );

    // Check box for edge drawing
    kvs::CheckBox edge( &screen );
    edge.setCaption( "Edge" );
    edge.setState( flag.edge );
    edge.setMargin( 10 );
    edge.anchorToTopLeft();
    edge.stateChanged( [&] () { flag.edge = edge.state(); } );

    // Check box for plane drawing
    kvs::CheckBox plane( &screen );
    plane.setCaption( "Plane" );
    plane.setState( flag.plane );
    plane.setMargin( 10 );
    plane.anchorToBottom( &edge );
    plane.stateChanged( [&] () { flag.plane = plane.state(); } );

    // Check box group
    kvs::CheckBoxGroup group;
    group.add( &edge );
    group.add( &plane );
    group.show();

    return app.run();
}
