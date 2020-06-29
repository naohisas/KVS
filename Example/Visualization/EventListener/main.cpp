/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for EventListener class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/EventListener>
#include <kvs/InitializeEventListener>
#include <kvs/PaintEventListener>
#include <kvs/ResizeEventListener>
#include <kvs/MousePressEventListener>
#include <kvs/MouseMoveEventListener>
#include <kvs/MouseReleaseEventListener>
#include <kvs/MouseDoubleClickEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/KeyRepeatEventListener>
#include <kvs/KeyReleaseEventListener>
#include <kvs/WheelEventListener>
#include <kvs/TimerEventListener>
#include <kvs/OpenGL>


namespace Integrated
{

class Event : public kvs::EventListener
{
public:
    Event( int msec )
    {
        setTimerInterval( msec ); ///< interval time in millisecond for timer event
    }

    void initializeEvent()
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::initializeEvent()" << std::endl;
    }

    void paintEvent()
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::paintEvent()" << std::endl;
    }

    void resizeEvent( int w, int h )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::resizeEvent( " << w << ", " << h << " )" << std::endl;
    }

    void mousePressEvent( kvs::MouseEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::mousePressEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
    }

    void mouseMoveEvent( kvs::MouseEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::mouseMoveEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
    }

    void mouseReleaseEvent( kvs::MouseEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::mouseReleaseEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
    }

    void mouseDoubleClickEvent( kvs::MouseEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::mouseDoubleClickEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
    }

    void wheelEvent( kvs::WheelEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::wheelEvent( " << e->direction() << " )" << std::endl;
    }

    void keyPressEvent( kvs::KeyEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::keyPressEvent( " << char( e->key() ) << " )" << std::endl;
        switch ( e->key() )
        {
        case kvs::Key::t:
            if ( eventTimer()->isStopped() ) { eventTimer()->start(); }
            else { eventTimer()->stop(); }
            break;
        default:
            break;
        }
    }

    void keyRepeatEvent( kvs::KeyEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::keyRepeatEvent( " << char( e->key() ) << " )" << std::endl;
    }

    void keyReleaseEvent( kvs::KeyEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::keyReleaseEvent( " << char( e->key() ) << " )" << std::endl;
    }

    void timerEvent( kvs::TimeEvent* e )
    {
        std::cout << "Intergrated" << std::endl;
        std::cout << "\t" << "Event::timerEvent()" << std::endl;
    }
};

} // end of namespace Integrated


namespace Separated
{

class InitializeEvent : public kvs::InitializeEventListener
{
    void update()
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "InitializeEvent::update()" << std::endl;
    }
};

class PaintEvent : public kvs::PaintEventListener
{
    void update()
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "PaintEvent::update()" << std::endl;
    }
};

class ResizeEvent : public kvs::ResizeEventListener
{
    void update( int w, int h )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "ResizeEvent::update( " << w << ", " << h << " )" << std::endl;
    }
};

class MousePressEvent : public kvs::MousePressEventListener
{
    void update( kvs::MouseEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "MousePressEvent::update( " << e->x() << ", " << e->y() << " )" << std::endl;
    }
};

class MouseMoveEvent : public kvs::MouseMoveEventListener
{
    void update( kvs::MouseEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "MouseMoveEvent::update( " << e->x() << ", " << e->y() << " )" << std::endl;
    }
};

class MouseReleaseEvent : public kvs::MouseReleaseEventListener
{
    void update( kvs::MouseEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "MouseReleaseEvent::update( " << e->x() << ", " << e->y() << " )" << std::endl;
    }
};

class MouseDoubleClickEvent : public kvs::MouseDoubleClickEventListener
{
    void update( kvs::MouseEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "MouseDoubleClickEvent::update( " << e->x() << ", " << e->y() << " )" << std::endl;
    }
};

class KeyPressEvent : public kvs::KeyPressEventListener
{
    void update( kvs::KeyEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "KeyPressEvent::update( " << char( e->key() ) << " )" << std::endl;
    }
};

class KeyRepeatEvent : public kvs::KeyRepeatEventListener
{
    void update( kvs::KeyEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "KeyRepeatEvent::update( " << char( e->key() ) << " )" << std::endl;
    }
};

class KeyReleaseEvent : public kvs::KeyReleaseEventListener
{
    void update( kvs::KeyEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "KeyReleaseEvent::update( " << char( e->key() ) << " )" << std::endl;
    }
};

class WheelEvent : public kvs::WheelEventListener
{
    void update( kvs::WheelEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "WheelEvent::update( " << e->direction() << " )" << std::endl;
    }
};

class TimerEvent : public kvs::TimerEventListener
{
public:
    TimerEvent( int msec ): kvs::TimerEventListener( msec ) {}

    void update( kvs::TimeEvent* e )
    {
        std::cout << "Separated" << std::endl;
        std::cout << "\t" << "TimerEvent::update()" << std::endl;
    }
};

} // end of namespace Separated


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
    screen.setTitle( "Event listener" );
    screen.create();

    // User specified events.
    const int interval1 = 1000; // interval time in millisecond for the timer event
    kvs::EventListener event;
    event.initializeEvent(
        []()
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "initializeEvent()" << std::endl;
        } );
    event.paintEvent(
        []()
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "paintEvent()" << std::endl;
            glBegin( GL_TRIANGLES );
            glColor3ub( 255,   0,   0 ); glVertex3d(  0.0,  3.0, 0.0 );
            glColor3ub(   0, 255,   0 ); glVertex3d(  3.0, -3.0, 0.0 );
            glColor3ub(   0,   0, 255 ); glVertex3d( -3.0, -3.0, 0.0 );
            glEnd();
        } );
    event.resizeEvent(
        []( int w, int h )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "resizeEvent( " << w << ", " << h << " )" << std::endl;
        } );
    event.mousePressEvent(
        []( kvs::MouseEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "mousePressEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
        } );
    event.mouseMoveEvent(
        []( kvs::MouseEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "mouseMoveEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
        } );
    event.mouseReleaseEvent(
        []( kvs::MouseEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "mouseReleaseEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
        } );
    event.mouseDoubleClickEvent(
        []( kvs::MouseEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "mouseDoubleClickEvent( " << e->x() << ", " << e->y() << " )" << std::endl;
        } );
    event.wheelEvent(
        []( kvs::WheelEvent* e )
        {
            std::cout << "\t" << "Event::wheelEvent( " << e->direction() << " )" << std::endl;
        } );
    event.keyPressEvent(
        [&]( kvs::KeyEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "keyPressEvent( " << char( e->key() ) << " )" << std::endl;
            switch ( e->key() )
            {
            case kvs::Key::t:
                if ( event.eventTimer()->isStopped() ) { event.eventTimer()->start(); }
                else { event.eventTimer()->stop(); }
                break;
            default:
                break;
            }
        } );
    event.keyRepeatEvent(
        []( kvs::KeyEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "keyRepeatEvent( " << char( e->key() ) << " )" << std::endl;
        } );
    event.keyReleaseEvent(
        []( kvs::KeyEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "keyReleaseEvent( " << char( e->key() ) << " )" << std::endl;
        } );
    event.timerEvent(
        []( kvs::TimeEvent* e )
        {
            std::cout << "kvs::EventListener" << std::endl;
            std::cout << "\t" << "timerEvent()" << std::endl;
        }, interval1 );
    screen.addEvent( &event );

    // User specified events.
    const int interval2 = 2000; // interval time in millisecond for the timer event
    Integrated::Event integrated_event( interval2 );
    screen.addEvent( &integrated_event );

    // User specified events.
    const int interval3 = 3000; // interval time in millisecond for the timer event
    Separated::InitializeEvent initialize_event;
    Separated::PaintEvent paint_event;
    Separated::ResizeEvent resize_event;
    Separated::MousePressEvent mouse_press_event;
    Separated::MouseMoveEvent mouse_move_event;
    Separated::MouseReleaseEvent mouse_release_event;
    Separated::MouseDoubleClickEvent mouse_double_click_event;
    Separated::KeyPressEvent key_press_event;
    Separated::KeyRepeatEvent key_repeat_event;
    Separated::KeyReleaseEvent key_release_event;
    Separated::WheelEvent wheel_event;
    Separated::TimerEvent timer_event( interval3 );
    screen.addEvent( &initialize_event );
    screen.addEvent( &paint_event );
    screen.addEvent( &resize_event );
    screen.addEvent( &mouse_press_event );
    screen.addEvent( &mouse_move_event );
    screen.addEvent( &mouse_release_event );
    screen.addEvent( &mouse_double_click_event );
    screen.addEvent( &key_press_event );
    screen.addEvent( &key_repeat_event );
    screen.addEvent( &key_release_event );
    screen.addEvent( &wheel_event );
    screen.addEvent( &timer_event );

    return app.run();
}
