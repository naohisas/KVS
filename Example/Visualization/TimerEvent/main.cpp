/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::TimerEventListener.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/HydrogenVolumeData>
#include <kvs/Isosurface>
#include <kvs/EventListener>
#include <kvs/RotationMatrix33>


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
    screen.setTitle( "Timer event" );
    screen.create();

    // Create object and renderer.
    auto* volume = new kvs::HydrogenVolumeData( { 64, 64, 64 } );
    auto* object = new kvs::Isosurface( volume, 100 );
    delete volume;
    screen.registerObject( object );

    // Create event listener.
    const int interval = 10; // animation time interval in milli-second.
    kvs::EventListener event;

    // Timer event
    event.timerEvent( [&]( kvs::TimeEvent* e )
    {
        // Rotate the object.
        const float D = 0.5f;
        const auto R =
            kvs::XRotationMatrix33<float>( D ) *
            kvs::YRotationMatrix33<float>( D ) *
            kvs::ZRotationMatrix33<float>( D );
        object->multiplyXform( kvs::Xform::Rotation( R ) );
        screen.redraw();
    }, interval );

    // Key press event
    event.keyPressEvent( [&]( kvs::KeyEvent* e )
    {
        // t: toggle switch timer start and stop
        switch ( e->key() )
        {
        case kvs::Key::t:
        {
            auto* t = event.eventTimer();
            if ( t->isStopped() ) t->start();
            else t->stop();
            break;
        }
        default: break;
        }
    } );

    screen.addEvent( &event );

    return app.run();
}
