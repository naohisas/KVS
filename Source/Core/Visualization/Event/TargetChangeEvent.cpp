/*****************************************************************************/
/**
 *  @file   TargetChangeEvent.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
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
 *  @brief  Constructs a new TargetChangeEvent class.
 */
/*===========================================================================*/
TargetChangeEvent::TargetChangeEvent():
    m_object_key( kvs::Key::o ),
    m_light_key( kvs::Key::l ),
    m_camera_key( kvs::Key::c )
{
}

/*===========================================================================*/
/**
 *  @brief  Changes control target.
 *  @param  event [in] pointer to the key event
 */
/*===========================================================================*/
void TargetChangeEvent::update( kvs::KeyEvent* event )
{
    KVS_ASSERT( scene() != NULL );

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
