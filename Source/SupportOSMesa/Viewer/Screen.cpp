#include "Screen.h"
#include <string>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/ColorImage>
#include <kvs/Scene>
#include <kvs/OpenGL>
#include <kvs/InitializeEvent>
#include <kvs/PaintEvent>
#include <kvs/EventHandler>
#include <kvs/Background>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>


namespace kvs
{

namespace osmesa
{

Screen::Screen():
    m_enable_default_paint_event( true ),
    m_scene( new kvs::Scene( this ) )
{
}

Screen::~Screen()
{
    if ( m_scene )
    {
        delete m_scene;
        m_scene = NULL;
    }
}

void Screen::setBackgroundColor( const kvs::RGBColor& color )
{
    m_scene->background()->setColor( color );
}

void Screen::setBackgroundColor( const kvs::RGBColor& color1, const kvs::RGBColor& color2 )
{
    m_scene->background()->setColor( color1, color2 );
}

void Screen::setBackgroundImage( const kvs::ColorImage& image )
{
    m_scene->background()->setImage( image );
}

void Screen::setEvent( kvs::EventListener* event, const std::string& name )
{
    switch ( event->eventType() )
    {
    case kvs::EventBase::PaintEvent: m_enable_default_paint_event = false; break;
//    case kvs::EventBase::ResizeEvent: m_enable_default_resize_event =false; break;
//    case kvs::EventBase::MousePressEvent: m_enable_default_mouse_press_event =false; break;
//    case kvs::EventBase::MouseMoveEvent: m_enable_default_mouse_move_event =false; break;
//    case kvs::EventBase::MouseReleaseEvent: m_enable_default_mouse_release_event =false; break;
//    case kvs::EventBase::WheelEvent: m_enable_default_wheel_event = false; break;
//    case kvs::EventBase::KeyPressEvent: m_enable_default_key_press_event = false; break;
    default: break;
    }
    event->setScene( m_scene );
    BaseClass::addEvent( event, name );
}

void Screen::addEvent( kvs::EventListener* event, const std::string& name )
{
    event->setScene( m_scene );
    BaseClass::addEvent( event, name );
}

const std::pair<int,int> Screen::registerObject( kvs::ObjectBase* object, kvs::RendererBase* renderer )
{
    return m_scene->registerObject( object, renderer );
}

const std::pair<int,int> Screen::registerObject( kvs::VisualizationPipeline* pipeline )
{
    // WARNING: It is necessary to increment the reference counter of the
    // pipeline.object() and the pipeline.renderer().
    kvs::ObjectBase* object = const_cast<kvs::ObjectBase*>( pipeline->object() );
    kvs::RendererBase* renderer = const_cast<kvs::RendererBase*>( pipeline->renderer() );

    const int object_id = m_scene->objectManager()->insert( object );
    const int renderer_id = m_scene->rendererManager()->insert( renderer );
    m_scene->IDManager()->insert( object_id, renderer_id );

    return std::pair<int,int>( object_id, renderer_id );
}

void Screen::enable()
{
    m_scene->enableObjectOperation();
}

void Screen::disable()
{
    m_scene->disableObjectOperation();
}

void Screen::reset()
{
    m_scene->reset();
}

void Screen::initializeEvent()
{
    m_scene->initializeFunction();

    kvs::InitializeEvent event;
    BaseClass::eventHandler()->notify( &event );
}

void Screen::paintEvent()
{
    if ( m_enable_default_paint_event ) this->defaultPaintEvent();
    else
    {
        kvs::PaintEvent event;
        BaseClass::eventHandler()->notify( &event );
    }
}

void Screen::defaultPaintEvent()
{
    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();
    {
        m_scene->paintFunction();
        kvs::PaintEvent event;
        BaseClass::eventHandler()->notify( &event );
    }

    kvs::OpenGL::Flush();
}

} // end of namespace osmesa

} // end of namespace kvs
