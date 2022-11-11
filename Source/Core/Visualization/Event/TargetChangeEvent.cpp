/*****************************************************************************/
/**
 *  @file   TargetChangeEvent.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "TargetChangeEvent.h"
#include <kvs/KeyEvent>
#include <kvs/Scene>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Changes control target.
 *  @param  event [in] pointer to the key event
 */
/*===========================================================================*/
void TargetChangeEvent::update( kvs::KeyEvent* event )
{
    KVS_ASSERT( scene() != nullptr );

    if ( event->key() == m_object_key )
    {
        scene()->controlTarget() = kvs::Scene::TargetObject;
    }
    else if ( event->key() == m_light_key )
    {
        scene()->controlTarget() = kvs::Scene::TargetLight;
    }
    else if ( event->key() == m_camera_key )
    {
        scene()->controlTarget() = kvs::Scene::TargetCamera;
    }
}

} // end of namespace kvs
