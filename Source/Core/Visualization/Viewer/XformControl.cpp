/****************************************************************************/
/**
 *  @file   XformControl.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "XformControl.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Rotates the current xform.
 *  @param  R [in] rotation matrix
 */
/*===========================================================================*/
void XformControl::rotate( const kvs::Mat3& R )
{
    const auto T = this->xform().translation();
    const auto X =
        kvs::Xform::Translation( T ) *
        kvs::Xform::Rotation( R ) *
        kvs::Xform::Translation( -T );
    this->multiplyXform( X );
}

/*===========================================================================*/
/**
 *  @brief  Translates the current xform.
 *  @param  T [in] translation vector
 */
/*===========================================================================*/
void XformControl::translate( const kvs::Vec3& T )
{
    this->multiplyXform( kvs::Xform::Translation( T ) );
}

/*===========================================================================*/
/**
 *  @brief  Scales the current xform.
 *  @param  S [in] scaling vector
 */
/*===========================================================================*/
void XformControl::scale( const kvs::Vec3& S )
{
    this->multiplyXform( kvs::Xform::Scaling( S ) );
}

} // end of namespace kvs
