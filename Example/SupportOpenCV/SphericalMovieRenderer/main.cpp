#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/opencv/MovieObject>
#include <kvs/opencv/SphericalMovieRenderer>
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
    kvs::opencv::SphericalMovieRenderer* renderer = new kvs::opencv::SphericalMovieRenderer();
    renderer->enableAutoPlay();
    renderer->enableLoopPlay();
    renderer->disableReversePlay();

    kvs::glut::Screen screen( &app );
    screen.setTitle( "kvs::opencv::SphericalMovieRenderer" );
    screen.registerObject( object, renderer );
    screen.addEvent( new IdleEvent() );
    screen.show();

    return app.run();
}
