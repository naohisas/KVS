/****************************************************************************/
/**
 *  @file   Ray.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Ray.h 1640 2013-09-11 01:45:45Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__RAY_H_INCLUDE
#define KVS__RAY_H_INCLUDE

#include <kvs/Vector3>
#include <kvs/Matrix44>
//#include <kvs/OpenGL>


namespace kvs
{

/*==========================================================================*/
/**
 *  Ray class.
 *
 *  Line equation of the ray :
 *
 *  P(t) = Q + t * V
 *
 *  Q : m_from
 *  V : m_direction ( normalize vector )
 *  t : m_t_min ~ m_t_max
 */
/*==========================================================================*/
class Ray
{
private:

    float m_t; ///< Parameter.
    kvs::Vec3 m_from; ///< From point.
    kvs::Vec3 m_direction; ///< Directional vector.
    kvs::Mat4 m_combined; ///< combined matrix
    kvs::Mat4 m_inverse; ///< inverse matrix
    kvs::Vec2 m_delta; ///< variables used to calculate m_from and m_direction
    kvs::Vec2 m_constant; ///< variables used to calculate m_from and m_direction

public:

    Ray( const float modelview[16], const float projection[16], const int viewport[4] );

    void setOrigin( const int win_x, const int win_y );
    void setT( const float t ) { m_t = t; }
    float t() const { return m_t; }
    const kvs::Vec3& from() const { return m_from; }
    const kvs::Vec3& direction() const { return m_direction; }
    const kvs::Vec3 point() const { return m_from + m_direction * m_t; }

    float depth() const
    {
        const kvs::Vec3 p( this->point() );

        const float view2 =
            p.x() * m_combined[2][0] +
            p.y() * m_combined[2][1] +
            p.z() * m_combined[2][2] +
            m_combined[2][3];

        const float view3 =
            p.x() * m_combined[3][0] +
            p.y() * m_combined[3][1] +
            p.z() * m_combined[3][2] +
            m_combined[3][3];

        // depth in window coordinate
        return ( 1.0f + view2 / view3 ) * 0.5f;
    }

    bool isIntersected(
        const kvs::Vec3& v0,
        const kvs::Vec3& v1,
        const kvs::Vec3& v2 );

    bool isIntersected(
        const kvs::Vec3& v0,
        const kvs::Vec3& v1,
        const kvs::Vec3& v2,
        const kvs::Vec3& v3 );

private:

//    void combine_projection_and_modelview( GLfloat projection[16], GLfloat modelview[16] );
    void combine_projection_and_modelview( const float projection[16], const float modelview[16] );

public:

    friend std::ostream& operator << ( std::ostream& os, const Ray& rhs );
};

} // end of namespace kvs

#endif // KVS__RAY_H_INCLUDE
