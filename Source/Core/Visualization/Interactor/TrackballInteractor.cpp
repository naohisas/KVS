/*****************************************************************************/
/**
 *  @file   TrackballInteractor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "TrackballInteractor.h"
#include <kvs/EventBase>
#include <kvs/Assert>
#include <kvs/ObjectManager>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TrackballInteractor class.
 */
/*===========================================================================*/
TrackballInteractor::TrackballInteractor()
{
    BaseClass::addEventType( kvs::EventBase::MousePressEvent );
    BaseClass::addEventType( kvs::EventBase::MouseMoveEvent );
    BaseClass::addEventType( kvs::EventBase::MouseReleaseEvent );
    BaseClass::addEventType( kvs::EventBase::WheelEvent );
    BaseClass::addEventType( kvs::EventBase::KeyPressEvent );
    BaseClass::addEventType( kvs::EventBase::TimerEvent );
    BaseClass::setTimerInterval( 10 ); // 10 msec
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  e [in] mouse event
 */
/*===========================================================================*/
void TrackballInteractor::mousePressEvent( kvs::MouseEvent* e )
{
    auto* s = BaseClass::scene();
    KVS_ASSERT( s != nullptr );

    if ( !s->isActiveMove( e->x(), e->y() ) ) { return; }

    switch ( e->button() )
    {
    case kvs::MouseButton::Left:
    {
        kvs::Mouse::OperationMode mode;
        switch ( e->modifiers() )
        {
        case kvs::Key::ShiftModifier:
            mode = kvs::Mouse::Scaling;
            break;
        case kvs::Key::ControlModifier:
            mode = kvs::Mouse::Translation;
            break;
        default:
            mode = kvs::Mouse::Rotation;
            s->updateCenterOfRotation();
            break;
        }
        s->mousePressFunction( e->x(), e->y(), mode );
        break;
    }
    case kvs::MouseButton::Middle:
    {
        const kvs::Mouse::OperationMode mode = kvs::Mouse::Scaling;
        s->mousePressFunction( e->x(), e->y(), mode );
        break;
    }
    case kvs::MouseButton::Right:
    {
        const kvs::Mouse::OperationMode mode = kvs::Mouse::Translation;
        s->mousePressFunction( e->x(), e->y(), mode );
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 *  @param  e [in] mouse event
 */
/*===========================================================================*/
void TrackballInteractor::mouseMoveEvent( kvs::MouseEvent* e )
{
    auto* s = BaseClass::scene();
    KVS_ASSERT( s != nullptr );

    if ( s->controlTarget() == kvs::Scene::TargetObject )
    {
        if ( !s->isEnabledObjectOperation() )
        {
            if ( !s->objectManager()->hasActiveObject() )
            {
                return;
            }
        }
    }

    s->mouseMoveFunction( e->x(), e->y() );
    BaseClass::screen()->redraw();
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  e [in] mouse event
 */
/*===========================================================================*/
void TrackballInteractor::mouseReleaseEvent( kvs::MouseEvent* e )
{
    auto* s = BaseClass::scene();
    KVS_ASSERT( s != nullptr );

    s->mouseReleaseFunction( e->x(), e->y() );
    BaseClass::screen()->redraw();
}

/*===========================================================================*/
/**
 *  @brief  Wheel event.
 *  @param  e [in] wheel event
 */
/*===========================================================================*/
void TrackballInteractor::wheelEvent( kvs::WheelEvent* e )
{
    auto* s = BaseClass::scene();
    KVS_ASSERT( s != nullptr );

    if ( !s->isActiveMove( e->x(), e->y() ) ) { return; }

    const int value = ( e->direction() > 0 ) ? 50 : -50;
    s->wheelFunction( value );
    BaseClass::screen()->redraw();
}

/*===========================================================================*/
/**
 *  @brief  Key press event.
 *  @param  e [in] key event
 */
/*===========================================================================*/
void TrackballInteractor::keyPressEvent( kvs::KeyEvent* e )
{
    auto* s = BaseClass::scene();
    KVS_ASSERT( s != nullptr );

    BaseClass::keyPressEvent( e );
    switch ( e->key() )
    {
    case kvs::Key::Tab:
        s->enableCollisionDetection();
        break;
    default:
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Time event.
 *  @param  e [in] time event
 */
/*===========================================================================*/
void TrackballInteractor::timerEvent( kvs::TimeEvent* e )
{
    auto* s = BaseClass::scene();
    KVS_ASSERT( s != nullptr );

    if ( s->mouse()->idle() )
    {
        if ( !( s->controlTarget() == kvs::Scene::TargetObject &&
                !s->isEnabledObjectOperation() &&
                !s->objectManager()->hasActiveObject() ) )
        {
            s->updateXform();
            BaseClass::screen()->redraw();
        }
    }
}

} // end of namespace kvs
