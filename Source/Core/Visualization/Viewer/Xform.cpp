/****************************************************************************/
/**
 *  @file Xform.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Xform.cpp 1539 2013-04-16 11:52:28Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Xform.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new xform matrix.
 */
/*===========================================================================*/
Xform::Xform(): m_matrix( kvs::Mat4::Identity() )
{
}

/*==========================================================================*/
/**
 *  @brief Constructs a new xform matrix.
 *  @param t [in] translation vector
 *  @param s [in] scaling vector
 *  @param r [in] rotation matrix
 */
/*==========================================================================*/
Xform::Xform( const kvs::Vec3& t, const kvs::Vec3& s, const kvs::Mat3& r ):
    m_matrix( s[0] * r[0][0], s[1] * r[0][1], s[2] * r[0][2], t[0],
              s[0] * r[1][0], s[1] * r[1][1], s[2] * r[1][2], t[1],
              s[0] * r[2][0], s[1] * r[2][1], s[2] * r[2][2], t[2],
              0, 0, 0, 1 )
{
}

/*==========================================================================*/
/**
 *  @brief Constructs a new xform matrix.
 *  @param m [in] xform matrix
 */
/*==========================================================================*/
Xform::Xform( const kvs::Mat4& m ):
    m_matrix( m )
{
}

/*==========================================================================*/
/**
 *  @brief  Returns the translation vector.
 *  @return translation vector
 */
/*==========================================================================*/
const kvs::Vec3 Xform::translation() const
{
    return kvs::Vec3( m_matrix[0][3], m_matrix[1][3], m_matrix[2][3] );
}

/*==========================================================================*/
/**
 *  @brief  Returns the rotation matrix.
 *  @return rotation matrix
 */
/*==========================================================================*/
const kvs::Mat3 Xform::rotation() const
{
    kvs::Vec3 s = this->scaling();
    return kvs::Mat3( m_matrix[0][0] / s.x(), m_matrix[0][1] / s.y(), m_matrix[0][2] / s.z(),
                      m_matrix[1][0] / s.x(), m_matrix[1][1] / s.y(), m_matrix[1][2] / s.z(),
                      m_matrix[2][0] / s.x(), m_matrix[2][1] / s.y(), m_matrix[2][2] / s.z() );
}

/*==========================================================================*/
/**
 *  @brief  Returns the scaled rotation matrix.
 *  @return scaled rotation matrix
 */
/*==========================================================================*/
const kvs::Mat3 Xform::scaledRotation() const
{
    return kvs::Mat3( m_matrix[0][0], m_matrix[0][1], m_matrix[0][2],
                      m_matrix[1][0], m_matrix[1][1], m_matrix[1][2],
                      m_matrix[2][0], m_matrix[2][1], m_matrix[2][2] );
}

/*==========================================================================*/
/**
 *  @brief  Returns the scale vector.
 *  @return rotation matrix
 */
/*==========================================================================*/
const kvs::Vector3f Xform::scaling() const
{
    const float sx = (float)kvs::Vec3( m_matrix[0][0], m_matrix[1][0], m_matrix[2][0] ).length();
    const float sy = (float)kvs::Vec3( m_matrix[0][1], m_matrix[1][1], m_matrix[2][1] ).length();
    const float sz = (float)kvs::Vec3( m_matrix[0][2], m_matrix[1][2], m_matrix[2][2] ).length();
    return kvs::Vec3( sx, sy, sz );
}

/*===========================================================================*/
/**
 *  @brief  Returns the transformed position by this xform matrix.
 *  @param  position [in] position
 *  @return transformed position
 */
/*===========================================================================*/
const kvs::Vec3 Xform::transform( const kvs::Vec3& position ) const
{
    kvs::Vec4 p = m_matrix * kvs::Vec4( position, 1 );
    return kvs::Vec3( p.x(), p.y(), p.z() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the transformed normal vector by this xform matrix.
 *  @param  normal [in] normal vector
 *  @return transformed normal vector
 */
/*===========================================================================*/
const kvs::Vec3 Xform::transformNormal( const kvs::Vec3& normal ) const
{
    kvs::Vec4 n = m_matrix * kvs::Vec4( normal, 0 );
    return kvs::Vec3( n.x(), n.y(), n.z() );
}

/*===========================================================================*/
/**
 *  @brief  Projects the position by this xform matrix.
 *  @param  position [in] position
 *  @return projected position
 */
/*===========================================================================*/
const kvs::Vec3 Xform::project( const kvs::Vec3& position ) const
{
    kvs::Vec4 p = m_matrix * kvs::Vec4( position, 1 );
    return kvs::Vec3( p.x(), p.y(), p.z() ) / p.w();
}

const kvs::Xform Xform::inverse() const
{
    return kvs::Xform( m_matrix.inverted() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform matrix.
 *  @return xform matrix
 */
/*===========================================================================*/
const kvs::Mat4 Xform::toMatrix() const
{
    return m_matrix;
}

/*===========================================================================*/
/**
 *  @brief  Gets the xform matrix as array
 *  @param  array [out] pointer to the array
 */
/*===========================================================================*/
void Xform::toArray( float array[16] ) const
{
    array[0]  = m_matrix[0][0];
    array[1]  = m_matrix[1][0];
    array[2]  = m_matrix[2][0];
    array[3]  = m_matrix[3][0];
    array[4]  = m_matrix[0][1];
    array[5]  = m_matrix[1][1];
    array[6]  = m_matrix[2][1];
    array[7]  = m_matrix[3][1];
    array[8]  = m_matrix[0][2];
    array[9]  = m_matrix[1][2];
    array[10] = m_matrix[2][2];
    array[11] = m_matrix[3][2];
    array[12] = m_matrix[0][3];
    array[13] = m_matrix[1][3];
    array[14] = m_matrix[2][3];
    array[15] = m_matrix[3][3];
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform generated from the given array.
 *  @param  ary [in] pointer to the array
 *  @return xform
 */
/*===========================================================================*/
const kvs::Xform Xform::FromArray( const float ary[16] )
{
    kvs::Mat4 m( ary );
    m.transpose();
    return kvs::Xform( m );
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform generated from the given translation vector.
 *  @param  t [in] translation vector
 *  @return translated xform
 */
/*===========================================================================*/
const kvs::Xform Xform::Translation( const kvs::Vec3& t )
{
    kvs::Mat4 m( 1, 0, 0, t[0],
                 0, 1, 0, t[1],
                 0, 0, 1, t[2],
                 0, 0, 0, 1 );
    return kvs::Xform( m );
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform generated from the given rotation matrix.
 *  @param  r [in] rotation matrix
 *  @return rotated xform
 */
/*===========================================================================*/
const kvs::Xform Xform::Rotation( const kvs::Mat3& r )
{
    kvs::Mat4 m( r[0][0], r[0][1], r[0][2], 0,
                 r[1][0], r[1][1], r[1][2], 0,
                 r[2][0], r[2][1], r[2][2], 0,
                 0, 0, 0, 1 );
    return kvs::Xform( m );
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform generated from the given scaling vector.
 *  @param  s [in] scaling vector
 *  @return scaled xform
 */
/*===========================================================================*/
const kvs::Xform Xform::Scaling( const kvs::Vec3& s )
{
    kvs::Mat4 m( s[0], 0, 0, 0,
                 0, s[1], 0, 0,
                 0, 0, s[2], 0,
                 0, 0, 0, 1 );
    return kvs::Xform( m );
}

/*===========================================================================*/
/**
 *  @brief  Returns the xform generated from the given scaling value.
 *  @param  s [in] scaling value
 *  @return scaled xform
 */
/*===========================================================================*/
const kvs::Xform Xform::Scaling( float s )
{
    kvs::Mat4 m( s, 0, 0, 0,
                 0, s, 0, 0,
                 0, 0, s, 0,
                 0, 0, 0, 1 );
    return kvs::Xform( m );
}

} // end of namepsace kvs
