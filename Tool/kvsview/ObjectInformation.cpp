/*****************************************************************************/
/**
 *  @file   ObjectInformation.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectInformation.cpp 1398 2012-12-05 09:33:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ObjectInformation.h"
#include <kvs/ImageObject>
#include <kvs/PointObject>
#include <kvs/LineObject>
#include <kvs/PolygonObject>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>


namespace kvsview
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ObjectInformation class.
 */
/*===========================================================================*/
ObjectInformation::ObjectInformation():
    m_object( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ObjectInformation class.
 *  @param  object [in] pointer to the object
 */
/*===========================================================================*/
ObjectInformation::ObjectInformation( const kvs::ObjectBase* object ):
    m_object( object )
{
}

/*===========================================================================*/
/**
 *  @brief  Output operator.
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const ObjectInformation& info )
{
    info.m_object->print( os );
    return os;
}

} // end of namespace kvsview
