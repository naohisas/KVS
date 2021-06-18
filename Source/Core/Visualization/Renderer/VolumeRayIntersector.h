/****************************************************************************/
/**
 *  @file   VolumeRayIntersector.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    kvs::Vec3 m_vertex[8];

public:
    VolumeRayIntersector(
        const kvs::VolumeObjectBase* volume,
        const float modelview[16],
        const float projection[16],
        const int viewport[4] );

    bool isIntersected()
    {
        return
            kvs::Ray::isIntersected( m_vertex[0], m_vertex[3], m_vertex[2], m_vertex[1] ) ||
            kvs::Ray::isIntersected( m_vertex[0], m_vertex[1], m_vertex[5], m_vertex[4] ) ||
            kvs::Ray::isIntersected( m_vertex[1], m_vertex[2], m_vertex[6], m_vertex[5] ) ||
            kvs::Ray::isIntersected( m_vertex[2], m_vertex[3], m_vertex[7], m_vertex[6] ) ||
            kvs::Ray::isIntersected( m_vertex[3], m_vertex[0], m_vertex[4], m_vertex[7] ) ||
            kvs::Ray::isIntersected( m_vertex[4], m_vertex[5], m_vertex[6], m_vertex[7] );
    }

    bool isInside() const
    {
        const kvs::Vec3 point( this->point() );
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
