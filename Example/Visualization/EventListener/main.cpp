/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for EventListener class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
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
#include <kvs/OpenGL>


/*===========================================================================*/
/**
 *  @brief  User specified initialization event.
 */
/*===========================================================================*/
class InitializeEvent : public kvs::InitializeEventListener
{
    void update()
    {
        std::cout << "InitializeEvent::update()" << std::endl;
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified paint event.
 */
/*===========================================================================*/
class PaintEvent : public kvs::PaintEventListener
{
    void update()
    {
        std::cout << "PaintEvent::update()" << std::endl;

        glBegin( GL_TRIANGLES );
        glColor3ub( 255,   0,   0 ); glVertex3d(  0.0,  3.0, 0.0 );
        glColor3ub(   0, 255,   0 ); glVertex3d(  3.0, -3.0, 0.0 );
        glColor3ub(   0,   0, 255 ); glVertex3d( -3.0, -3.0, 0.0 );
        glEnd();
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified resize event.
 */
/*===========================================================================*/
class ResizeEvent : public kvs::ResizeEventListener
{
    void update( int width, int height )
    {
        std::cout << "ResizeEvent::update( " << width << ", " << height << " )" << std::endl;
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified mouse press event.
 */
/*===========================================================================*/
class MousePressEvent : public kvs::MousePressEventListener
{
    void update( kvs::MouseEvent* event )
    {
        std::cout << "MousePressEvent::update( " << event->x() << ", " << event->y() << " )" << std::endl;
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified mouse move event.
 */
/*===========================================================================*/
class MouseMoveEvent : public kvs::MouseMoveEventListener
{
    void update( kvs::MouseEvent* event )
    {
        std::cout << "MouseMoveEvent::update( " << event->x() << ", " << event->y() << " )" << std::endl;
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified mouse release event.
 */
/*===========================================================================*/
class MouseReleaseEvent : public kvs::MouseReleaseEventListener
{
    void update( kvs::MouseEvent* event )
    {
        std::cout << "MouseReleaseEvent::update( " << event->x() << ", " << event->y() << " )" << std::endl;
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified mouse double-click event.
 */
/*===========================================================================*/
class MouseDoubleClickEvent : public kvs::MouseDoubleClickEventListener
{
    void update( kvs::MouseEvent* event )
    {
        std::cout << "MouseDoubleClickEvent::update( " << event->x() << ", " << event->y() << " )" << std::endl;
    }
};

/*===========================================================================*/
/**
 *  @brief  User specified key press event.
 */
/*===========================================================================*/
class KeyPressEvent : public kvs::KeyPressEventListener
{
    void update( kvs::KeyEvent* event )
    {
        std::cout << "KeyPressEvent::update( " << char( event->key() ) << " )" << std::endl;
    }
};

class KeyRepeatEvent : public kvs::KeyRepeatEventListener
{
    void update( kvs::KeyEvent* event )
    {
        std::cout << "KeyRepeatEvent::update( " << char( event->key() ) << " )" << std::endl;
    }
};

class KeyReleaseEvent : public kvs::KeyReleaseEventListener
{
    void update( kvs::KeyEvent* event )
    {
        std::cout << "KeyReleaseEvent::update( " << char( event->key() ) << " )" << std::endl;
    }
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
    screen.setTitle( "Event listener" );

    // User specified events.
    InitializeEvent initialize_event;
    PaintEvent paint_event;
    ResizeEvent resize_event;
    MousePressEvent mouse_press_event;
    MouseMoveEvent mouse_move_event;
    MouseReleaseEvent mouse_release_event;
    MouseDoubleClickEvent mouse_double_click_event;
    KeyPressEvent key_press_event;
    KeyRepeatEvent key_repeat_event;
    KeyReleaseEvent key_release_event;

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
    screen.create();

    return app.run();
}
