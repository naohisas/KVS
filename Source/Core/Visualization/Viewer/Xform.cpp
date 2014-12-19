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

/*==========================================================================*/
/**
 *  Default constructor.
 */
/*==========================================================================*/
Xform::Xform():
    m_matrix( 1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1 )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param translation [in] translation vector
 *  @param scaling [in] scaling vector
 *  @param rotation [in] rotation matrix
 */
/*==========================================================================*/
Xform::Xform(
    const kvs::Vector3f&  t,
    const kvs::Vector3f&  s,
    const kvs::Matrix33f& r ):
    m_matrix( s[0] * r[0][0], s[1] * r[0][1], s[2] * r[0][2], t[0],
              s[0] * r[1][0], s[1] * r[1][1], s[2] * r[1][2], t[1],
              s[0] * r[2][0], s[1] * r[2][1], s[2] * r[2][2], t[2],
              0, 0, 0, 1 )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param m [in] xform matrix
 */
/*==========================================================================*/
Xform::Xform( const kvs::Matrix44f& m ):
    m_matrix( m )
{
}

/*==========================================================================*/
/**
 *  Get the translation vector.
 *  @return translation vector
 */
/*==========================================================================*/
const kvs::Vector3f Xform::translation() const
{
    return kvs::Vector3f( m_matrix[0][3], m_matrix[1][3], m_matrix[2][3] );
}

/*==========================================================================*/
/**
 *  Get the rotation matrix.
 *  @return rotation matrix
 */
/*==========================================================================*/
const kvs::Matrix33f Xform::rotation() const
{
    kvs::Vector3f s = this->scaling();
    return kvs::Matrix33f( m_matrix[0][0] / s.x(), m_matrix[0][1] / s.y(), m_matrix[0][2] / s.z(),
                           m_matrix[1][0] / s.x(), m_matrix[1][1] / s.y(), m_matrix[1][2] / s.z(),
                           m_matrix[2][0] / s.x(), m_matrix[2][1] / s.y(), m_matrix[2][2] / s.z() );
}

/*==========================================================================*/
/**
 *  Get the scaled rotation matrix.
 *  @return scaled rotation matrix
 */
/*==========================================================================*/
const kvs::Matrix33f Xform::scaledRotation() const
{
    return kvs::Matrix33f( m_matrix[0][0], m_matrix[0][1], m_matrix[0][2],
                           m_matrix[1][0], m_matrix[1][1], m_matrix[1][2],
                           m_matrix[2][0], m_matrix[2][1], m_matrix[2][2] );
}

/*==========================================================================*/
/**
 *  Get the scale vector.
 *  @return rotation matrix
 */
/*==========================================================================*/
const kvs::Vector3f Xform::scaling() const
{
    const float sx = (float)kvs::Vector3f( m_matrix[0][0], m_matrix[1][0], m_matrix[2][0] ).length();
    const float sy = (float)kvs::Vector3f( m_matrix[0][1], m_matrix[1][1], m_matrix[2][1] ).length();
    const float sz = (float)kvs::Vector3f( m_matrix[0][2], m_matrix[1][2], m_matrix[2][2] ).length();
    return kvs::Vector3f( sx, sy, sz );
}

const kvs::Vector3f Xform::transform( const kvs::Vector3f& pos ) const
{
    kvs::Vector4f p = m_matrix * kvs::Vector4f( pos, 1 );
    return kvs::Vector3f( p.x(), p.y(), p.z() );
}

const kvs::Vector3f Xform::transformNormal( const kvs::Vector3f& normal ) const
{
    kvs::Vector4f n = m_matrix * kvs::Vector4f( normal, 0 );
    return kvs::Vector3f( n.x(), n.y(), n.z() );
}

const kvs::Vector3f Xform::project( const kvs::Vector3f& pos ) const
{
    kvs::Vector4f p = m_matrix * kvs::Vector4f( pos, 1 );
    return kvs::Vector3f( p.x(), p.y(), p.z() ) / p.w();
}

const kvs::Xform Xform::inverse() const
{
    return kvs::Xform( m_matrix.inverted() );
}

const kvs::Matrix44f Xform::toMatrix() const
{
    return m_matrix;
}

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

const kvs::Xform Xform::FromArray( const float ary[16] )
{
    kvs::Matrix44f m( ary );
    m.transpose();
    return kvs::Xform( m );
}

const kvs::Xform Xform::Translation( const kvs::Vector3f& t )
{
    kvs::Matrix44f m( 1, 0, 0, t[0],
                      0, 1, 0, t[1],
                      0, 0, 1, t[2],
                      0, 0, 0, 1 );
    return kvs::Xform( m );
}

const kvs::Xform Xform::Rotation( const kvs::Matrix33f& r )
{
    kvs::Matrix44f m( r[0][0], r[0][1], r[0][2], 0,
                      r[1][0], r[1][1], r[1][2], 0,
                      r[2][0], r[2][1], r[2][2], 0,
                      0, 0, 0, 1 );
    return kvs::Xform( m );
}

const kvs::Xform Xform::Scaling( const kvs::Vector3f& s )
{
    kvs::Matrix44f m( s[0], 0, 0, 0,
                      0, s[1], 0, 0,
                      0, 0, s[2], 0,
                      0, 0, 0, 1 );
    return kvs::Xform( m );
}

const kvs::Xform Xform::Scaling( float s )
{
    kvs::Matrix44f m( s, 0, 0, 0,
                      0, s, 0, 0,
                      0, 0, s, 0,
                      0, 0, 0, 1 );
    return kvs::Xform( m );
}

} // end of namepsace kvs
