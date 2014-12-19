/*****************************************************************************/
/**
 *  @file   ObjectInformation.cpp
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
ObjectInformation::ObjectInformation( void ):
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
/*
    // Image object.
    if ( info.m_object->objectType() == kvs::ObjectBase::Image )
    {
        os << *reinterpret_cast<const kvs::ImageObject*>( info.m_object );
    }

    // Geometry object.
    else if ( info.m_object->objectType() == kvs::ObjectBase::Geometry )
    {
        const kvs::GeometryObjectBase* geometry =
            reinterpret_cast<const kvs::GeometryObjectBase*>( info.m_object );
        if ( geometry->geometryType() == kvs::GeometryObjectBase::Point )
        {
            os << *reinterpret_cast<const kvs::PointObject*>( geometry );
        }
        else if ( geometry->geometryType() == kvs::GeometryObjectBase::Line )
        {
            os << *reinterpret_cast<const kvs::LineObject*>( geometry );
        }
        else if ( geometry->geometryType() == kvs::GeometryObjectBase::Polygon )
        {
            os << *reinterpret_cast<const kvs::PolygonObject*>( geometry );
        }
    }

    // Volume object.
    else if ( info.m_object->objectType() == kvs::ObjectBase::Volume )
    {
        const kvs::VolumeObjectBase* volume =
            reinterpret_cast<const kvs::VolumeObjectBase*>( info.m_object );

        if ( !volume->hasMinMaxValues() ) volume->updateMinMaxValues();

        if ( volume->volumeType() == kvs::VolumeObjectBase::Structured )
        {
            os << *reinterpret_cast<const kvs::StructuredVolumeObject*>( volume );
        }
        else if ( volume->volumeType() == kvs::VolumeObjectBase::Unstructured )
        {
            os << *reinterpret_cast<const kvs::UnstructuredVolumeObject*>( volume );
        }
    }
*/
    return( os );
}

} // end of namespace kvsview
