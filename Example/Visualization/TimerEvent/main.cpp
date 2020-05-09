/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::TimerEventListener.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1526 2013-04-11 03:03:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Timer>
#include <kvs/HydrogenVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
#include <kvs/TimerEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/RotationMatrix33>
#include <kvs/ObjectManager>


/*===========================================================================*/
/**
 *  @brief  User-defined key press event.
 */
/*===========================================================================*/
class KeyPressEvent : public kvs::KeyPressEventListener
{
    kvs::glut::Timer* m_timer;

public:
    KeyPressEvent( kvs::glut::Timer* timer ): m_timer( timer ) {}

    void update( kvs::KeyEvent* event )
    {
        switch ( event->key() )
        {
        case kvs::Key::s:
            if ( m_timer->isStopped() ) m_timer->start();
            else m_timer->stop();
            break;
        default:
            break;
        }
    }
};

/*===========================================================================*/
/**
 *  @brief  User-defined timer event.
 */
/*===========================================================================*/
class TimerEvent : public kvs::TimerEventListener
{
    void update( kvs::TimeEvent* event )
    {
        static float deg = 0.0f;
        //const kvs::Vector3f center( 0, 0, 0 );
        kvs::Matrix33f rotation = kvs::XRotationMatrix33<float>( deg );
        rotation *= kvs::YRotationMatrix33<float>( deg );
        rotation *= kvs::ZRotationMatrix33<float>( deg );

        kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
        object_manager->object()->multiplyXform( kvs::Xform::Rotation( rotation ) );
        screen()->redraw();

        deg += 0.1f;
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
    kvs::glut::Application app( argc, argv );

    // Object.
    kvs::StructuredVolumeObject* volume = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, 100 );
    delete volume;

    // Event.
    int msec = 200;
    kvs::glut::Timer timer( msec );
    TimerEvent timer_event;
    KeyPressEvent key_press_event( &timer );

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "Timer event" );
    screen.addEvent( &key_press_event );
    screen.addTimerEvent( &timer_event, &timer );
    screen.registerObject( object );
    screen.show();

    return app.run();
}
