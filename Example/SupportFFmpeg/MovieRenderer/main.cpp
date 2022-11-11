#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/EventListener>
#include <kvs/Slider>
#include <kvs/Label>
#include <kvs/String>
#include <kvs/ffmpeg/MovieObject>
#include <kvs/ffmpeg/MovieRenderer>


int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );

    // Movie object and renderer.
    using Object = kvs::ffmpeg::MovieObject;
    using Renderer = kvs::ffmpeg::MovieRenderer;
    auto* object = new Object( argv[1] );
    auto* renderer = new Renderer();

    // Create screen.
    auto width = object->width();
    auto height = object->height();
    screen.setSize( width, height );
    screen.create();
    screen.scene()->mouse()->disableAutoUpdating();

    // Frame slider.
    kvs::Slider slider( &screen );
    slider.anchorToBottomRight();
    slider.setCaption( "Frame: " + kvs::String::From( 0, 6, '0' ) );
    slider.setRange( 0, object->numberOfFrames() - 1 );
    slider.setValue( 0 );
    slider.sliderMoved( [&]()
    {
        auto index = kvs::Math::Round( slider.value() );
        auto frame = kvs::String::From( index, 6, '0' );
        slider.setValue( index );
        slider.setCaption( "Frame: " + frame );
    } );
    slider.sliderPressed( [&]()
    {
        renderer->pause();
    } );
    slider.sliderReleased( [&]()
    {
        auto index = kvs::Math::Round( slider.value() );
        object->jumpToFrame( index );
        renderer->play();
        screen.redraw();
        renderer->pause();
    } );
    slider.show();

    // Key-press and timer events.
    const int interval = 1000 / object->frameRate(); // msec
    kvs::EventListener event;
    event.keyPressEvent( [&]( kvs::KeyEvent* e )
    {
        switch ( e->key() )
        {
        case kvs::Key::s: { renderer->stop(); break; }
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
    event.timerEvent( [&]( kvs::TimeEvent* e )
    {
        if ( renderer->isPlaying() )
        {
            auto index = static_cast<kvs::UInt32>( object->currentFrameIndex() );
            auto frame = kvs::String::From( index, 6, '0' );
            slider.setValue( index );
            slider.setCaption( "Frame: " + frame );
            screen.redraw();
        }
    }, interval );
    screen.addEvent( &event );

    // Register object and renderer.
    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
