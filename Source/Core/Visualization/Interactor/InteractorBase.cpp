/*****************************************************************************/
/**
 *  @file   InteractorBase.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "InteractorBase.h"
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new InteractorBase class.
 */
/*===========================================================================*/
InteractorBase::InteractorBase():
    kvs::EventListener( 0 )
{
    BaseClass::addEventType( kvs::EventBase::InitializeEvent );
    BaseClass::addEventType( kvs::EventBase::PaintEvent );
    BaseClass::addEventType( kvs::EventBase::ResizeEvent );
    BaseClass::addEventType( kvs::EventBase::KeyPressEvent );
}

/*===========================================================================*/
/**
 *  @brief  Initialize event.
 */
/*===========================================================================*/
void InteractorBase::initializeEvent()
{
    auto* s = BaseClass::scene();
    if ( !s ) { return; }

    s->initializeFunction();
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void InteractorBase::paintEvent()
{
    auto* s = BaseClass::scene();
    if ( !s ) { return; }

    s->paintFunction();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  w [in] screen width
 *  @param  h [in] screen height
 */
/*===========================================================================*/
void InteractorBase::resizeEvent( int w, int h )
{
    auto dpr = 1.0f;
    if ( BaseClass::screen() )
    {
        auto* s = BaseClass::screen();
        if ( !s->isFullScreen() ) { s->setSize( w, h ); }
        dpr = s->devicePixelRatio();
    }

    auto* s = BaseClass::scene();
    if ( !s ) { return; }

    s->resizeFunction( w, h, dpr );
}

/*===========================================================================*/
/**
 *  @brief  Key press event.
 *  @param  e [in] key event
 */
/*===========================================================================*/
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
