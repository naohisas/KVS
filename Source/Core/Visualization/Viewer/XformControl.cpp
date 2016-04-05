/****************************************************************************/
/**
 *  @file   XformControl.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XformControl.cpp 1539 2013-04-16 11:52:28Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "XformControl.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Saves the current xform.
 */
/*===========================================================================*/
void XformControl::saveXform()
{
    m_initial_xform = m_current_xform;
}

/*===========================================================================*/
/**
 *  @brief  Resets xform.
 */
/*===========================================================================*/
void XformControl::resetXform()
{
    m_current_xform = m_initial_xform;
}

/*===========================================================================*/
/**
 *  @brief  Sets an xform to the current xform
 *  @param  xform [in] xform
 */
/*===========================================================================*/
void XformControl::setXform( const kvs::Xform& xform )
{
    m_current_xform = xform;
}

/*===========================================================================*/
/**
 *  @brief  Multiplies an xform to the current xform.
 *  @param  xform [in] xform
 */
/*===========================================================================*/
void XformControl::multiplyXform( const kvs::Xform& xform )
{
    this->setXform( xform * this->xform() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the current xform.
 *  @return the current xform
 */
/*===========================================================================*/
const kvs::Xform XformControl::xform() const
{
    return m_current_xform;
}

/*===========================================================================*/
/**
 *  @brief  Rotates the current xform.
 *  @param  rotation [in] rotation matrix
 */
/*===========================================================================*/
void XformControl::rotate( const kvs::Mat3& rotation )
{
    const kvs::Vec3 t = this->xform().translation();
    const kvs::Xform x = kvs::Xform::Translation( t )
                       * kvs::Xform::Rotation( rotation )
                       * kvs::Xform::Translation( -t );
    this->multiplyXform( x );
}

/*===========================================================================*/
/**
 *  @brief  Translates the current xform.
 *  @param  translation [in] translation vector
 */
/*===========================================================================*/
void XformControl::translate( const kvs::Vec3& translation )
{
    this->multiplyXform( kvs::Xform::Translation( translation ) );
}

/*===========================================================================*/
/**
 *  @brief  Scales the current xform.
 *  @param  scaling [in] scaling vector
 */
/*===========================================================================*/
void XformControl::scale( const kvs::Vec3& scaling )
{
    this->multiplyXform( kvs::Xform::Scaling( scaling ) );
}

} // end of namespace kvs
