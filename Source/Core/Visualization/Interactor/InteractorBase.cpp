#include "InteractorBase.h"
#include <kvs/OpenGL>


namespace kvs
{

InteractorBase::InteractorBase():
    kvs::EventListener( 0 )
{
    BaseClass::addEventType( kvs::EventBase::InitializeEvent );
    BaseClass::addEventType( kvs::EventBase::PaintEvent );
    BaseClass::addEventType( kvs::EventBase::ResizeEvent );
    BaseClass::addEventType( kvs::EventBase::KeyPressEvent );
}

void InteractorBase::initializeEvent()
{
    auto* s = BaseClass::scene();
    if ( !s ) { return; }

    s->initializeFunction();
}

void InteractorBase::paintEvent()
{
    auto* s = BaseClass::scene();
    if ( !s ) { return; }

//    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
//    p.loadIdentity();
    s->paintFunction();
}

void InteractorBase::resizeEvent( int w, int h )
{
    if ( BaseClass::screen() )
    {
        auto* s = BaseClass::screen();
        if ( !s->isFullScreen() ) { s->setSize( w, h ); }
    }

    auto* s = BaseClass::scene();
    if ( !s ) { return; }

    s->resizeFunction( w, h );
}

void InteractorBase::keyPressEvent( kvs::KeyEvent* e )
{
    auto* s = BaseClass::scene();
    if ( !s ) { return; }

    switch( e->key() )
    {
    case kvs::Key::Escape:
    case kvs::Key::q:
        exit( 0 );
        break;
    case kvs::Key::Home:
        s->reset();
        break;
    default:
        break;
    }
}

} // end of namespace kvs
