/****************************************************************************/
/**
 *  @file   VolumeRayIntersector.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VolumeRayIntersector.cpp 1698 2014-01-16 10:49:03Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "VolumeRayIntersector.h"
#include <cfloat>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new VolumeRayIntersector class.
 *  @param  volume [in] pointer to the volume object
 *  @param  modelview [in] modelview matrix
 *  @param  projection [in] projection matrix
 *  @param  viewport [in] viewport
*/
/*===========================================================================*/
VolumeRayIntersector::VolumeRayIntersector(
    const kvs::VolumeObjectBase* volume,
    const float modelview[16],
    const float projection[16],
    const int viewport[4] ):
    Ray( modelview, projection, viewport )
{
    const float epsilon = 0.003f; // 1e-3

    const kvs::Vector3f min(
        volume->minExternalCoord().x() + epsilon,
        volume->minExternalCoord().y() + epsilon,
        volume->minExternalCoord().z() + epsilon );

    const kvs::Vector3f max(
        volume->maxExternalCoord().x() - epsilon,
        volume->maxExternalCoord().y() - epsilon,
        volume->maxExternalCoord().z() - epsilon );

    m_vertex[0].set( min.x(), min.y(), min.z() );
    m_vertex[1].set( max.x(), min.y(), min.z() );
    m_vertex[2].set( max.x(), max.y(), min.z() );
    m_vertex[3].set( min.x(), max.y(), min.z() );
    m_vertex[4].set( min.x(), min.y(), max.z() );
    m_vertex[5].set( max.x(), min.y(), max.z() );
    m_vertex[6].set( max.x(), max.y(), max.z() );
    m_vertex[7].set( min.x(), max.y(), max.z() );
}

} // end of namespace kvs
