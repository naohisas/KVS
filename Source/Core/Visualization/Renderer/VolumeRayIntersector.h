/****************************************************************************/
/**
 *  @file   VolumeRayIntersector.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VolumeRayIntersector.h 1698 2014-01-16 10:49:03Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__VOLUME_RAY_INTERSECTOR_H_INCLUDE
#define KVS__VOLUME_RAY_INTERSECTOR_H_INCLUDE

#include <kvs/Ray>
#include <kvs/Vector3>
#include <kvs/VolumeObjectBase>


namespace kvs
{

/*==========================================================================*/
/**
 *  RayInUniformVolume class.
 */
/*==========================================================================*/
class VolumeRayIntersector : public kvs::Ray
{
private:

    kvs::Vector3f m_vertex[8];

public:

    VolumeRayIntersector(
        const kvs::VolumeObjectBase* volume,
        const float modelview[16],
        const float projection[16],
        const int viewport[4] );

    bool isIntersected()
    {
        return(
            kvs::Ray::isIntersected( m_vertex[0], m_vertex[3], m_vertex[2], m_vertex[1] ) ||
            kvs::Ray::isIntersected( m_vertex[0], m_vertex[1], m_vertex[5], m_vertex[4] ) ||
            kvs::Ray::isIntersected( m_vertex[1], m_vertex[2], m_vertex[6], m_vertex[5] ) ||
            kvs::Ray::isIntersected( m_vertex[2], m_vertex[3], m_vertex[7], m_vertex[6] ) ||
            kvs::Ray::isIntersected( m_vertex[3], m_vertex[0], m_vertex[4], m_vertex[7] ) ||
            kvs::Ray::isIntersected( m_vertex[4], m_vertex[5], m_vertex[6], m_vertex[7] ) );
    }

    bool isInside() const
    {
        const kvs::Vector3f point( this->point() );

        if ( m_vertex[0].z() < point.z() && point.z() < m_vertex[6].z() )
        {
            if ( m_vertex[0].y() < point.y() && point.y() < m_vertex[6].y() )
            {
                if ( m_vertex[0].x() < point.x() && point.x() < m_vertex[6].x() )
                {
                    return true;
                }
            }
        }

        return false;
    }

    void step( const float delta_t )
    {
        this->setT( this->t() + delta_t );
    }
};

} // end of namespace kvs

#endif // KVS__VOLUME_RAY_INTERSECTOR_H_INCLUDE
