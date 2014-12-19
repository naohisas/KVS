/****************************************************************************/
/**
 *  @file XformControl.cpp
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
#if KVS_ENABLE_DEPRECATED
#include <kvs/OpenGL>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 *  @param collision [in] conllision flag
 */
/*==========================================================================*/
XformControl::XformControl()
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
XformControl::~XformControl()
{
}

/*==========================================================================*/
/**
 *  Save xform.
 */
/*==========================================================================*/
void XformControl::saveXform()
{
    m_initial_xform = m_current_xform;
}

/*==========================================================================*/
/**
 *  Reset xform.
 */
/*==========================================================================*/
void XformControl::resetXform()
{
    m_current_xform = m_initial_xform;
}

/*==========================================================================*/
/**
 *  Set xform.
 *  @param xform [in] xform matrix.
 */
/*==========================================================================*/
void XformControl::setXform( const kvs::Xform& xform )
{
    m_current_xform = xform;
}

void XformControl::multiplyXform( const kvs::Xform& xform )
{
    this->setXform( xform * this->xform() );
}

/*==========================================================================*/
/**
 *  Get xform.
 *  @return xform matrix
 */
/*==========================================================================*/
const kvs::Xform XformControl::xform() const
{
    return m_current_xform;
}

/*==========================================================================*/
/**
 *  Rotate.
 *  @param rotation [in] current rotation matrix.
 */
/*==========================================================================*/
void XformControl::rotate( const kvs::Matrix33f& rotation )
{
    const kvs::Vector3f t = this->xform().translation();
    const kvs::Xform x = kvs::Xform::Translation( t )
                       * kvs::Xform::Rotation( rotation )
                       * kvs::Xform::Translation( -t );
    this->multiplyXform( x );
}

/*==========================================================================*/
/**
 *  Translate.
 *  @param translation [in] current translation vector.
 */
/*==========================================================================*/
void XformControl::translate( const kvs::Vector3f& translation )
{
    this->multiplyXform( kvs::Xform::Translation( translation ) );
}

/*==========================================================================*/
/**
 *  Scaling.
 *  @param scale [in] current scaling value.
 */
/*==========================================================================*/
void XformControl::scale( const kvs::Vector3f& scaling )
{
    this->multiplyXform( kvs::Xform::Scaling( scaling ) );
}

} // end of namespace kvs
