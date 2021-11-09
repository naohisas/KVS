#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/EventListener>
#include <kvs/Slider>
#include <kvs/Label>
#include <kvs/String>
#include <kvs/ffmpeg/MovieObject>
#include <kvs/ffmpeg/SphericalMovieRenderer>


int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );

    using Object = kvs::ffmpeg::MovieObject;
    using Renderer = kvs::ffmpeg::SphericalMovieRenderer;
    auto* object = new Object( argv[1] );
    auto* renderer = new Renderer();
    screen.create();

    kvs::Label label( &screen );
    label.anchorToTopLeft();
    label.screenUpdated(
        [&]()
        {
            auto index = static_cast<kvs::UInt32>( object->currentFrameIndex() );
            auto frame = kvs::String::From( index, 6, '0' );
            label.setText( "  Frame: " + frame );
        } );
    label.show();

    const int interval = 1000 / object->frameRate(); // msec
    kvs::EventListener event;
    event.keyPressEvent(
        [&]( kvs::KeyEvent* e )
        {
            switch ( e->key() )
            {
            case kvs::Key::s:
            {
                renderer->stop();
                break;
            }
            case kvs::Key::Space:
            {
                if ( renderer->isPaused() ) { renderer->play(); }
                else if ( renderer->isPlaying() ) { renderer->pause(); }
                else { renderer->play(); }
                break;
            }
            default: break;
            }
        } );
    event.timerEvent(
        [&]( kvs::TimeEvent* e )
        {
            if ( !object->isLastFrame() ) { screen.redraw(); kvs::OpenGL::Flush(); }
            else { event.eventTimer()->stop(); }
        }, interval );
    screen.addEvent( &event );

    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
