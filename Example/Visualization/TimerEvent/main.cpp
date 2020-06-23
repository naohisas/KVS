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
#include <kvs/StructuredVolumeObject>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
#include <kvs/TimerEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/RotationMatrix33>
#include <kvs/ObjectManager>

class TimerEvent : public kvs::TimerEventListener
{
public:
    TimerEvent( int msec )
    {
        setTimerInterval( msec );
    }

    void update( kvs::TimeEvent* e )
    {
        const float D = 0.5f;
        const auto R =
            kvs::XRotationMatrix33<float>( D ) *
            kvs::YRotationMatrix33<float>( D ) *
            kvs::ZRotationMatrix33<float>( D );

        auto* manager = scene()->objectManager();
        manager->object()->multiplyXform( kvs::Xform::Rotation( R ) );
        screen()->redraw();
    }
};

class KeyPressEvent : public kvs::KeyPressEventListener
{
    void update( kvs::KeyEvent* e )
    {
        switch ( e->key() )
        {
        case kvs::Key::t:
        {
            auto* l = screen()->eventHandler()->listener("TimerEvent");
            auto* t = l->eventTimer();
            if ( t->isStopped() ) t->start();
            else t->stop();
            break;
        }
        default: break;
        }
    }
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
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "Timer event" );
    screen.create();

    auto* volume = new kvs::HydrogenVolumeData( kvs::Vec3u::Constant( 64 ) );
    auto* object = new kvs::Isosurface( volume, 100 );
    delete volume;
    screen.registerObject( object );

    KeyPressEvent key_press_event;
    TimerEvent timer_event( 1 );
    timer_event.setName("TimerEvent");
    screen.addEvent( &key_press_event );
    screen.addEvent( &timer_event );

    return app.run();
}
