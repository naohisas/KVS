#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/opencv/MovieObject>
#include <kvs/opencv/MovieRenderer>
#include <kvs/IdleEventListener>


class IdleEvent : public kvs::IdleEventListener
{
    void update() { screen()->redraw(); }
};

/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    kvs::opencv::MovieObject* object = new kvs::opencv::MovieObject( argv[1] );
    kvs::opencv::MovieRenderer* renderer = new kvs::opencv::MovieRenderer();
    renderer->enableAutoPlay();
    renderer->enableLoopPlay();
    renderer->disableReversePlay();

    const size_t width = object->width();
    const size_t height = object->height();
    kvs::glut::Screen screen( &app );
    screen.setTitle( "kvs::opencv::MovieRenderer" );
    screen.setSize( width, height );
    screen.registerObject( object, renderer );
    screen.addEvent( new IdleEvent() );
    screen.show();

    return app.run();
}
